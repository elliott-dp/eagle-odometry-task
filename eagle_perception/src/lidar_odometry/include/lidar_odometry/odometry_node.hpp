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

#include "lidar_odometry/local_map.hpp"
#include "lidar_odometry/scan_matcher.hpp"

#include <builtin_interfaces/msg/time.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <visualization_msgs/msg/marker_array.hpp>
#include "lidar_odometry_interfaces/msg/processing_stats.hpp"

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
  LocalMapParams loadLocalMapParams();
  ScanMatcherParams loadScanMatcherParams();
  Eigen::Isometry2d estimatePose(const PointCloud & scan);

  Preprocessor preprocessor_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr filtered_pub_;

    // Built in the constructor's initializer list, so keep them declared in this order.
  LocalMap local_map_;
  ScanMatcher scan_matcher_;
  double scan_voxel_size_{};  // [m] coarse voxel for the scan that gets registered

  Eigen::Isometry2d pose_ = Eigen::Isometry2d::Identity();         // odom -> base_link: start at the origin facing +x
  Eigen::Isometry2d last_motion_ = Eigen::Isometry2d::Identity();  // motion between the last two scans: start still

  std::string base_frame_;
  double max_scan_gap_s_;  // [s] a longer gap between two scans means one was lost

  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr scan_sub_;
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

  std::optional<Eigen::Isometry3d> base_T_sensor_;  // cache del TF statico
  std::optional<rclcpp::Time> last_stamp_;

  using ProcessingStats = lidar_odometry_interfaces::msg::ProcessingStats;
  using Stamp = builtin_interfaces::msg::Time;

  void createPublishers();
  void updateVelocity(const rclcpp::Time & stamp);
  void publishOdometry(const Stamp & stamp);
  void publishTrajectory(const Stamp & stamp);
  void publishMap(const Stamp & stamp);

  double map_voxel_size_{};  // [m] fine voxel for the points that go into the map
  std::vector<Eigen::Isometry2d> trajectory_;                      // every estimated pose, for the markers
  Eigen::Vector3d velocity_ = Eigen::Vector3d::Zero();             // vx, vy [m/s], yaw rate [rad/s] in base_link
  std::optional<rclcpp::Time> last_pose_stamp_;
  std::size_t scan_count_ = 0;

  std::string odom_frame_;
  int map_publish_period_{};  // publish the whole map every this many scans

  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr trajectory_pub_;
  rclcpp::Publisher<ProcessingStats>::SharedPtr timing_pub_;
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr map_pub_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
};

}  // namespace lidar_odometry