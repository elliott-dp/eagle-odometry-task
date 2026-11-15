#pragma once

#include <memory>
#include <optional>
#include <string>

#include <Eigen/Geometry>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>

#include <std_msgs/msg/header.hpp>
#include "lidar_odometry/preprocessor.hpp"

namespace lidar_odometry {

class OdometryNode : public rclcpp::Node {
public:
  OdometryNode();

private:
  using PointCloud = pcl::PointCloud<pcl::PointXYZ>;

  void onScan(const sensor_msgs::msg::PointCloud2::ConstSharedPtr & msg);
  bool loadSensorTransform(const std::string & sensor_frame);
  void checkForMissedScans(const rclcpp::Time & stamp);
  PreprocessorParams loadPreprocessorParams();
  PointCloud::Ptr toPcl(const sensor_msgs::msg::PointCloud2 & msg) const;
  PointCloud::Ptr toBaseLink(const PointCloud & sensor_cloud) const;  // replaces toBaseLink(msg)
  void publishFilteredCloud(const PointCloud & cloud, const std_msgs::msg::Header & scan_header);

  Preprocessor preprocessor_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr filtered_pub_;

  std::string base_frame_;
  double expected_period_s_;

  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr scan_sub_;
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

  std::optional<Eigen::Isometry3d> base_T_sensor_;  // cache del TF statico
  std::optional<rclcpp::Time> last_stamp_;
};

}  // namespace lidar_odometry