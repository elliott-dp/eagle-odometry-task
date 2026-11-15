#include "lidar_odometry/odometry_node.hpp"

#include <pcl/common/transforms.h>
#include <pcl/filters/filter.h>
#include <pcl/make_shared.h>
#include <pcl_conversions/pcl_conversions.h>
#include <tf2_eigen/tf2_eigen.hpp>

#include <std_msgs/msg/header.hpp>
#include "lidar_odometry/preprocessor.hpp"

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

OdometryNode::OdometryNode() : rclcpp::Node("lidar_odometry")
{
  const auto input_topic = declare_parameter<std::string>("input_topic", "/lidar/points");
  base_frame_ = declare_parameter<std::string>("base_frame", "base_link");
  expected_period_s_ = declare_parameter<double>("expected_scan_period_s", 0.1);
  const auto queue_depth = declare_parameter<int>("input_queue_depth", 50);

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

  RCLCPP_INFO(get_logger(),
              "Scan: %zu raw -> %zu cropped -> %zu above ground | "
              "ground: %zu inliers, roll %.2f deg, pitch %.2f deg, height %.3f m",
              raw->size(), cropped->size(), above_ground->size(),
              ground.inliers, ground.roll_deg, ground.pitch_deg, ground.height);
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
  PointCloud2 out;
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