#pragma once

#include <vector>

#include <Eigen/Geometry>
#include <std_msgs/msg/header.hpp>
#include <visualization_msgs/msg/marker_array.hpp>

namespace lidar_odometry {

// The path driven so far as a line, plus an arrow showing where the car points now.
// The header gives the frame (odom) and the stamp of the latest scan.
// The trajectory must not be empty.
visualization_msgs::msg::MarkerArray makeTrajectoryMarkers(
  const std::vector<Eigen::Isometry2d> & trajectory, const std_msgs::msg::Header & header);

}  // namespace lidar_odometry
