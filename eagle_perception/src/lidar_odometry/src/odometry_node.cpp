#include "lidar_odometry/odometry_node.hpp"

#include <pcl/common/transforms.h>
#include <pcl/filters/filter.h>
#include <pcl/make_shared.h>
#include <pcl_conversions/pcl_conversions.h>
#include <tf2_eigen/tf2_eigen.hpp>

namespace lidar_odometry {

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

void OdometryNode::onScan(const sensor_msgs::msg::PointCloud2::ConstSharedPtr & msg)
{
  if (!loadSensorTransform(msg->header.frame_id)) {
    return;  // il prossimo scan riprova
  }
  checkForMissedScans(rclcpp::Time(msg->header.stamp));

  const auto cloud = toBaseLink(*msg);
  RCLCPP_DEBUG(get_logger(), "Scan in %s: %zu punti", base_frame_.c_str(), cloud->size());
  // prossimo passo: preprocessor -> scan matcher -> publish
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
    RCLCPP_INFO(get_logger(), "TF statico %s -> %s caricato", sensor_frame.c_str(),
                base_frame_.c_str());
    return true;
  } catch (const tf2::TransformException & ex) {
    RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000, "TF statico non ancora disponibile: %s",
                         ex.what());
    return false;
  }
}

OdometryNode::PointCloud::Ptr OdometryNode::toBaseLink(
  const sensor_msgs::msg::PointCloud2 & msg) const
{
  auto raw = pcl::make_shared<PointCloud>();
  pcl::fromROSMsg(msg, *raw);

  std::vector<int> kept_indices;
  pcl::removeNaNFromPointCloud(*raw, *raw, kept_indices);

  auto in_base = pcl::make_shared<PointCloud>();
  pcl::transformPointCloud(*raw, *in_base, base_T_sensor_->matrix().cast<float>());
  return in_base;
}

void OdometryNode::checkForMissedScans(const rclcpp::Time & stamp)
{
  if (last_stamp_ && (stamp - *last_stamp_).seconds() > 1.5 * expected_period_s_) {
    RCLCPP_WARN(get_logger(), "Possibile scan perso: %.0f ms dal precedente",
                (stamp - *last_stamp_).seconds() * 1000.0);
  }
  last_stamp_ = stamp;
}

}  // namespace lidar_odometry