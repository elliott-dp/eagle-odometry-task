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

#include <chrono>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include "lidar_odometry/markers.hpp"

namespace lidar_odometry {

namespace {
// Measures how long each stage of a scan takes, in milliseconds.
class StageTimer {
public:
  StageTimer() : start_(Clock::now()), last_(start_) {}

  // Time since the previous lap (or since the start). Then the next lap begins.
  double lap()
  {
    const auto now = Clock::now();
    const double ms = toMs(now - last_);
    last_ = now;
    return ms;
  }

  double total() const { return toMs(Clock::now() - start_); }

private:
  using Clock = std::chrono::steady_clock;

  static double toMs(Clock::duration duration)
  {
    return std::chrono::duration<double, std::milli>(duration).count();
  }

  Clock::time_point start_;
  Clock::time_point last_;
  };
}  // namespace


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

  LocalMapParams OdometryNode::loadLocalMapParams()
{
  LocalMapParams params;
  params.voxel_size = declare_parameter<double>("local_map.voxel_size");
  params.max_points_per_voxel = declare_parameter<int>("local_map.max_points_per_voxel");
  params.normal_neighbours = declare_parameter<int>("local_map.normal_neighbours");
  return params;
}

OdometryNode::OdometryNode() :
  rclcpp::Node("lidar_odometry"),
  base_frame_(declare_parameter<std::string>("base_frame")),
  max_scan_gap_s_(declare_parameter<double>("max_scan_gap_s")),
  preprocessor_(loadPreprocessorParams()),
  local_map_(loadLocalMapParams()),
  scan_matcher_(loadScanMatcherParams())
{
  const auto input_topic = declare_parameter<std::string>("input_topic", "/lidar/points");
  const auto queue_depth = declare_parameter<int>("input_queue_depth", 50);
  scan_voxel_size_ = declare_parameter<double>("downsample.scan_voxel_size");
  map_voxel_size_ = declare_parameter<double>("downsample.map_voxel_size");

  tf_buffer_ = std::make_unique<tf2_ros::Buffer>(get_clock());
  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

  const auto filtered_topic = declare_parameter<std::string>("filtered_topic");
  filtered_pub_ = create_publisher<sensor_msgs::msg::PointCloud2>(filtered_topic, 10);
  createPublishers();

  const auto qos = rclcpp::QoS(rclcpp::KeepLast(static_cast<size_t>(queue_depth))).reliable();
  scan_sub_ = create_subscription<sensor_msgs::msg::PointCloud2>(
    input_topic, qos,
    [this](sensor_msgs::msg::PointCloud2::ConstSharedPtr msg) { onScan(msg); });


    RCLCPP_INFO(get_logger(), "Listening on %s, publishing the filtered scan on %s",
              input_topic.c_str(), filtered_topic.c_str());
}

void OdometryNode::onScan(const sensor_msgs::msg::PointCloud2::ConstSharedPtr & msg)
{
  StageTimer timer;
  if (!loadSensorTransform(msg->header.frame_id)) {
    return;  // the next scan will try again
  }
  const rclcpp::Time stamp(msg->header.stamp);
  checkForMissedScans(stamp);
  ++scan_count_;

  ProcessingStats timing;
  timing.header.stamp = msg->header.stamp;
  timing.header.frame_id = base_frame_;

  // 2. Crop in the sensor frame, where the range is exactly what the LiDAR measured.
  const auto raw = toPcl(*msg);
  const auto cropped = preprocessor_.cropByRange(*raw);
  timing.crop_ms = timer.lap();

  // 3. Move to base_link, find the ground, level the scan and drop the ground points.
  const auto in_base = toBaseLink(*cropped);
  const GroundFit ground = preprocessor_.fitGround(*in_base);
  if (!ground.found) {
    RCLCPP_WARN(get_logger(), "No ground plane found (%zu inliers), scan not levelled",
                ground.inliers);
  }
  const auto above_ground = preprocessor_.levelAndRemoveGround(*in_base, ground);
  timing.ground_ms = timer.lap();

  RCLCPP_DEBUG(get_logger(),
               "Scan: %zu raw -> %zu cropped -> %zu above ground | "
               "ground: %zu inliers, roll %.2f deg, pitch %.2f deg, height %.3f m",
               raw->size(), cropped->size(), above_ground->size(),
               ground.inliers, ground.roll_deg, ground.pitch_deg, ground.height);

  // 4. Downsample twice: a coarse cloud keeps registration fast,
  //    a finer one goes into the map so the map stays denser than any single scan.
  const auto scan_points = voxelDownsample(above_ground, scan_voxel_size_);
  const auto map_points = voxelDownsample(above_ground, map_voxel_size_);
  timing.downsample_ms = timer.lap();

  // 5-6. Predict, then register against the map.
  const Eigen::Isometry2d new_pose = estimatePose(*scan_points);
  last_motion_ = pose_.inverse() * new_pose;
  pose_ = new_pose;
  trajectory_.push_back(pose_);
  updateVelocity(stamp);
  timing.registration_ms = timer.lap();

  // 7. Add the fine cloud to the map at the new pose.
  local_map_.addScan(*map_points, pose_);
  timing.map_update_ms = timer.lap();

  // 8. Publish. Everything carries the scan's stamp, not the time we finished.
  publishOdometry(msg->header.stamp);
  publishTrajectory(msg->header.stamp);
  publishFilteredCloud(*above_ground, msg->header);
  publishMap(msg->header.stamp);
  timing.publish_ms = timer.lap();

  timing.total_ms = timer.total();
  timing_pub_->publish(timing);

  RCLCPP_INFO(get_logger(), "Scan %zu | x %.2f m, y %.2f m, yaw %.1f deg | %.1f m/s | %.1f ms",
              scan_count_, pose_.translation().x(), pose_.translation().y(),
              pcl::rad2deg(yawOf(pose_)), velocity_.head<2>().norm(), timing.total_ms);
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

  RCLCPP_DEBUG(get_logger(),
               "Registration: %d iterations, %d of %zu points matched, map %zu points",
               result.iterations, result.correspondences, scan.size(), local_map_.size());
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
  if (last_stamp_ && (stamp - *last_stamp_).seconds() > max_scan_gap_s_) {
    RCLCPP_WARN(get_logger(), "Possibile scan perso: %.0f ms dal precedente",
                (stamp - *last_stamp_).seconds() * 1000.0);
  }
  last_stamp_ = stamp;
}


  // Step 7-8 Functions
  // Everything step 8 sends out. Topic and frame names come from the YAML file too.
void OdometryNode::createPublishers()
{
  odom_frame_ = declare_parameter<std::string>("odom_frame");
  map_publish_period_ = declare_parameter<int>("map_publish_every_n_scans");
  const auto odometry_topic = declare_parameter<std::string>("odometry_topic");
  const auto trajectory_topic = declare_parameter<std::string>("trajectory_topic");
  const auto timing_topic = declare_parameter<std::string>("processing_time_topic");
  const auto map_topic = declare_parameter<std::string>("map_topic");

  odom_pub_ = create_publisher<nav_msgs::msg::Odometry>(odometry_topic, 10);
  trajectory_pub_ = create_publisher<visualization_msgs::msg::MarkerArray>(trajectory_topic, 10);
  timing_pub_ = create_publisher<ProcessingStats>(timing_topic, 10);
  // Transient local: a viewer that starts late still gets the latest map.
  map_pub_ = create_publisher<sensor_msgs::msg::PointCloud2>(map_topic, rclcpp::QoS(1).transient_local());
  tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
}


// Speed and turn rate: the last motion divided by the time between the two scans.
// The motion is expressed in the car's previous frame, so x is forward and y is left.
void OdometryNode::updateVelocity(const rclcpp::Time & stamp)
{
  if (last_pose_stamp_) {
    const double dt = (stamp - *last_pose_stamp_).seconds();
    if (dt > 0.0) {
      velocity_ = Eigen::Vector3d(last_motion_.translation().x(), last_motion_.translation().y(),
                                  yawOf(last_motion_)) / dt;
    }
  }
  last_pose_stamp_ = stamp;
}


void OdometryNode::publishOdometry(const Stamp & stamp)
{
  // The car stays on the ground: z = 0, no roll, no pitch.
  const Eigen::Isometry3d pose_3d = to3D(pose_);

  nav_msgs::msg::Odometry odometry;
  odometry.header.stamp = stamp;
  odometry.header.frame_id = odom_frame_;
  odometry.child_frame_id = base_frame_;
  odometry.pose.pose = tf2::toMsg(pose_3d);
  // The twist is expressed in the child frame (base_link): x forward, y left.
  odometry.twist.twist.linear.x = velocity_.x();
  odometry.twist.twist.linear.y = velocity_.y();
  odometry.twist.twist.angular.z = velocity_.z();
  odom_pub_->publish(odometry);

  // The same pose on TF, so RViz can place the car and its scans in odom.
  geometry_msgs::msg::TransformStamped transform = tf2::eigenToTransform(pose_3d);
  transform.header.stamp = stamp;
  transform.header.frame_id = odom_frame_;
  transform.child_frame_id = base_frame_;
  tf_broadcaster_->sendTransform(transform);
}


void OdometryNode::publishTrajectory(const Stamp & stamp)
{
  std_msgs::msg::Header header;
  header.stamp = stamp;
  header.frame_id = odom_frame_;
  trajectory_pub_->publish(makeTrajectoryMarkers(trajectory_, header));
}


// The map of the whole track (nice-to-have of path B). It grows to tens of thousands of
// points, so it goes out only every few scans instead of at 10 Hz.
void OdometryNode::publishMap(const Stamp & stamp)
{
  // 0 turns the map off.
  if (map_publish_period_ <= 0 || scan_count_ % static_cast<std::size_t>(map_publish_period_) != 0) {
    return;
  }
  sensor_msgs::msg::PointCloud2 out;
  pcl::toROSMsg(*local_map_.toPointCloud(), out);
  out.header.stamp = stamp;
  out.header.frame_id = odom_frame_;
  map_pub_->publish(out);
}
}  // namespace lidar_odometry