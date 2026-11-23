#include "lidar_odometry/markers.hpp"

#include <geometry_msgs/msg/point.hpp>
#include <std_msgs/msg/color_rgba.hpp>
#include <tf2_eigen/tf2_eigen.hpp>

#include "lidar_odometry/pose2d.hpp"

namespace lidar_odometry {

namespace {

// How the markers look in RViz. Purely cosmetic: nothing in the algorithm depends on these.
constexpr double kPathWidth = 0.15;   // [m]
constexpr double kArrowLength = 2.0;  // [m]
constexpr double kArrowWidth = 0.4;   // [m]

std_msgs::msg::ColorRGBA makeColor(float r, float g, float b)
{
  std_msgs::msg::ColorRGBA color;
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = 1.0f;
  return color;
}

visualization_msgs::msg::Marker makePathMarker(
  const std::vector<Eigen::Isometry2d> & trajectory, const std_msgs::msg::Header & header)
{
  visualization_msgs::msg::Marker path;
  path.header = header;
  path.ns = "trajectory";
  path.id = 0;
  path.type = visualization_msgs::msg::Marker::LINE_STRIP;
  path.action = visualization_msgs::msg::Marker::ADD;
  path.pose.orientation.w = 1.0;  // the points are already in odom
  path.scale.x = kPathWidth;
  path.color = makeColor(0.1f, 0.6f, 1.0f);

  for (const auto & pose : trajectory) {
    geometry_msgs::msg::Point point;
    point.x = pose.translation().x();
    point.y = pose.translation().y();
    point.z = 0.0;  // base_link is on the ground
    path.points.push_back(point);
  }
  return path;
}

visualization_msgs::msg::Marker makeHeadingMarker(
  const Eigen::Isometry2d & pose, const std_msgs::msg::Header & header)
{
  visualization_msgs::msg::Marker heading;
  heading.header = header;
  heading.ns = "heading";
  heading.id = 0;
  heading.type = visualization_msgs::msg::Marker::ARROW;
  heading.action = visualization_msgs::msg::Marker::ADD;
  heading.pose = tf2::toMsg(to3D(pose));
  heading.scale.x = kArrowLength;
  heading.scale.y = kArrowWidth;
  heading.scale.z = kArrowWidth;
  heading.color = makeColor(1.0f, 0.4f, 0.0f);
  return heading;
}

}  // namespace

visualization_msgs::msg::MarkerArray makeTrajectoryMarkers(
  const std::vector<Eigen::Isometry2d> & trajectory, const std_msgs::msg::Header & header)
{
  visualization_msgs::msg::MarkerArray markers;
  markers.markers.push_back(makeHeadingMarker(trajectory.back(), header));

  // A line needs at least two points.
  if (trajectory.size() >= 2) {
    markers.markers.push_back(makePathMarker(trajectory, header));
  }
  return markers;
}

}  // namespace lidar_odometry
