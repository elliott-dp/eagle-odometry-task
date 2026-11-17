#include "lidar_odometry/odometry_node.hpp"

#include <pcl/common/transforms.h>
#include <pcl/filters/filter.h>
#include <pcl/make_shared.h>
#include <pcl_conversions/pcl_conversions.h>
#include <tf2_eigen/tf2_eigen.hpp>

#include <std_msgs/msg/header.hpp>
#include "lidar_odometry/preprocessor.hpp"

#include <pcl/common/angles.h>
#include "lidar_odometry/pose2d.hpp"

namespace lidar_odometry {

  // No defaults in the code: every value must come from the YAML file.
  // If one is missing, the node stops at startup and prints its name.
PreprocessorParams OdometryNode::loadPreprocessorParams()
{
  PreprocessorParams params;
  params.min_range = declare_parameter<double>("crop.min_range");
  params.max_range = declare_parameter<double>("crop.max_range");
  params.ground_band = declare_parameter<double>("ground.band");
  params.ground_fit_radius = declare_parameter<double>("ground.fit_radius");
  params.ransac_distance = declare_parameter<double>("ground.ransac_distance");
  params.ransac_max_iterations = declare_parameter<int>("ground.ransac_max_iterations");
  params.max_ground_tilt_deg = declare_parameter<double>("ground.max_tilt_deg");
  params.min_ground_inliers = declare_parameter<int>("ground.min_inliers");
  params.ground_clearance = declare_parameter<double>("ground.clearance");
  return params;
}

OdometryNode::OdometryNode() :
  rclcpp::Node("lidar_odometry"),
  preprocessor_(loadPreprocessorParams()),
  local_map_(loadLocalMapParams()),
  scan_matcher_(loadScanMatcherParams())
{
  const auto input_topic = declare_parameter<std::string>("input_topic", "/lidar/points");
  base_frame_ = declare_parameter<std::string>("base_frame", "base_link");
  expected_period_s_ = declare_parameter<double>("expected_scan_period_s", 0.1);
  const auto queue_depth = declare_parameter<int>("input_queue_depth", 50);
  scan_voxel_size_ = declare_parameter<double>("downsample.scan_voxel_size");


  tf_buffer_ = std::make_unique<tf2_ros::Buffer>(get_clock());
  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

  const auto qos = rclcpp::QoS(rclcpp::KeepLast(static_cast<size_t>(queue_depth))).reliable();
  scan_sub_ = create_subscription<sensor_msgs::msg::PointCloud2>(
    input_topic, qos,
    [this](sensor_msgs::msg::PointCloud2::ConstSharedPtr msg) { onScan(msg); });
}

// Replaces the old onScan.
void OdometryNode::onScan(const sensor_msgs::msg::PointCloud2::ConstSharedPtr & msg)
{
  if (!loadSensorTransform(msg->header.frame_id)) {
    return;  // the next scan will try again
  }
  checkForMissedScans(rclcpp::Time(msg->header.stamp));

  // 1. Crop in the sensor frame, where the range is exactly what the LiDAR measured.
  const auto raw = toPcl(*msg);
  const auto cropped = preprocessor_.cropByRange(*raw);

  // 2. Move to base_link, find the ground, level the scan and drop the ground points.
  const auto in_base = toBaseLink(*cropped);
  const GroundFit ground = preprocessor_.fitGround(*in_base);
  if (!ground.found) {
    RCLCPP_WARN(get_logger(), "No ground plane found (%zu inliers), scan not levelled",
                ground.inliers);
  }
  const auto above_ground = preprocessor_.levelAndRemoveGround(*in_base, ground);

  publishFilteredCloud(*above_ground, msg->header);

  // 4. Downsample twice: a coarse cloud keeps registration fast,
  //    a finer one goes into the map so the map stays denser than any single scan.
  const auto scan_points = voxelDownsample(above_ground, scan_voxel_size_);
  const auto map_points = voxelDownsample(above_ground, local_map_.voxelSize());

  // 5-6. Predict, then register against the local map.
  const Eigen::Isometry2d new_pose = estimatePose(*scan_points);
  last_motion_ = pose_.inverse() * new_pose;
  pose_ = new_pose;

  local_map_.addScan(*map_points, pose_);

  RCLCPP_DEBUG(get_logger(),
              "Scan: %zu raw -> %zu cropped -> %zu above ground | "
              "ground: %zu inliers, roll %.2f deg, pitch %.2f deg, height %.3f m",
              raw->size(), cropped->size(), above_ground->size(),
              ground.inliers, ground.roll_deg, ground.pitch_deg, ground.height);
}

  LocalMapParams OdometryNode::loadLocalMapParams()
{
  LocalMapParams params;
  params.voxel_size = declare_parameter<double>("downsample.map_voxel_size");
  params.max_scans = declare_parameter<int>("local_map.max_scans");
  params.normal_neighbours = declare_parameter<int>("local_map.normal_neighbours");
  return params;
}

ScanMatcherParams OdometryNode::loadScanMatcherParams()
{
  ScanMatcherParams params;
  params.max_iterations = declare_parameter<int>("registration.max_iterations");
  params.max_correspondence_distance =
    declare_parameter<double>("registration.max_correspondence_distance");
  params.robust_kernel_scale = declare_parameter<double>("registration.robust_kernel_scale");
  params.convergence_epsilon = declare_parameter<double>("registration.convergence_epsilon");
  params.min_correspondences = declare_parameter<int>("registration.min_correspondences");
  return params;
}

Eigen::Isometry2d OdometryNode::estimatePose(const PointCloud & scan)
{
  // 5. Predict. ICP only converges from a nearby start, and at 10 m/s the car moves
  //    a full metre between scans, so assume it repeats its last motion.
  const Eigen::Isometry2d predicted = pose_ * last_motion_;

  // The first scan has nothing to align to: the car starts at the origin of odom.
  if (local_map_.empty()) {
    RCLCPP_INFO(get_logger(), "First scan: the map starts at the origin of odom");
    return predicted;
  }

  // 6. Register: refine the prediction against the local map.
  const RegistrationResult result = scan_matcher_.align(scan, local_map_, predicted);
  if (!result.valid) {
    RCLCPP_WARN(get_logger(), "Registration failed (%d correspondences), keeping the prediction",
                result.correspondences);
    return predicted;
  }
  if (!result.converged) {
    RCLCPP_WARN(get_logger(), "Registration hit the cap of %d iterations, using the last estimate",
                result.iterations);
  }

  RCLCPP_INFO(get_logger(),
              "Pose x %.2f m, y %.2f m, yaw %.1f deg | %d iterations, %d of %zu points matched, "
              "map %zu points",
              result.pose.translation().x(), result.pose.translation().y(),
              pcl::rad2deg(yawOf(result.pose)), result.iterations, result.correspondences,
              scan.size(), local_map_.size());
  return result.pose;
}

bool OdometryNode::loadSensorTransform(const std::string & sensor_frame)
{
  if (base_T_sensor_) {
    return true;
  }
  try {
    // lookupTransform(target, source): trasforma punti da source a target
    const auto tf = tf_buffer_->lookupTransform(base_frame_, sensor_frame, tf2::TimePointZero);
    base_T_sensor_ = tf2::transformToEigen(tf);
    RCLCPP_INFO(get_logger(), "Static TF %s -> %s loaded", sensor_frame.c_str(),
                base_frame_.c_str());
    return true;
  } catch (const tf2::TransformException & ex) {
    RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000, "Static TF still not available: %s",
                         ex.what());
    return false;
  }
}

// New: the conversion and NaN removal that used to live in toBaseLink.
PointCloud::Ptr OdometryNode::toPcl(const sensor_msgs::msg::PointCloud2 & msg) const
{
  // Only x, y, z are copied: intensity and ring are not needed for odometry.
  auto cloud = pcl::make_shared<PointCloud>();
  pcl::fromROSMsg(msg, *cloud);

  // A single NaN inside the scan matcher would ruin the estimate.
  pcl::Indices kept;
  pcl::removeNaNFromPointCloud(*cloud, *cloud, kept);
  return cloud;
}

// Replaces the old toBaseLink(msg): it now only applies the static TF.
PointCloud::Ptr OdometryNode::toBaseLink(const PointCloud & sensor_cloud) const
{
  auto in_base = pcl::make_shared<PointCloud>();
  pcl::transformPointCloud(sensor_cloud, *in_base, base_T_sensor_->matrix().cast<float>());
  return in_base;
}

  void OdometryNode::publishFilteredCloud(
  const PointCloud & cloud, const std_msgs::msg::Header & scan_header)
{
  // The levelled frame differs from base_link only by the small suspension tilt,
  // so it is shown in base_link. The stamp is the scan's, not the current time.
  sensor_msgs::msg::PointCloud2 out;
  pcl::toROSMsg(cloud, out);
  out.header.stamp = scan_header.stamp;
  out.header.frame_id = base_frame_;
  filtered_pub_->publish(out);
}

void OdometryNode::checkForMissedScans(const rclcpp::Time & stamp) // per debug!!!
{
  if (last_stamp_ && (stamp - *last_stamp_).seconds() > 1.5 * expected_period_s_) {
    RCLCPP_WARN(get_logger(), "Possibile scan perso: %.0f ms dal precedente",
                (stamp - *last_stamp_).seconds() * 1000.0);
  }
  last_stamp_ = stamp;
}

}  // namespace lidar_odometry