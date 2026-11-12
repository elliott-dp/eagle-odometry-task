#include <memory>
#include <rclcpp/rclcpp.hpp>
#include "lidar_odometry/odometry_node.hpp"

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<lidar_odometry::OdometryNode>());
  rclcpp::shutdown();
  return 0;
}