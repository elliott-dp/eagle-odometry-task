#pragma once

#include <cmath>

#include <Eigen/Geometry>
#include <pcl/point_types.h>

namespace lidar_odometry {

// The car drives on flat ground, so its pose is just x, y and yaw.
// Eigen::Isometry2d stores it and gives composition (*) and inverse() for free.

inline Eigen::Isometry2d makePose2D(double x, double y, double yaw)
{
  Eigen::Isometry2d pose = Eigen::Isometry2d::Identity();
  pose.translation() = Eigen::Vector2d(x, y);
  pose.linear() = Eigen::Rotation2Dd(yaw).toRotationMatrix();
  return pose;
}

inline double yawOf(const Eigen::Isometry2d & pose)
{
  return std::atan2(pose.linear()(1, 0), pose.linear()(0, 0));
}

// p' = R(yaw) * p + t. Only x and y change: the scan is already levelled,
// so the height of a point is the same in every frame.
inline Eigen::Vector3d transformPoint(const Eigen::Isometry2d & pose, const pcl::PointXYZ & p)
{
  const Eigen::Vector2d xy = pose * Eigen::Vector2d(p.x, p.y);
  return Eigen::Vector3d(xy.x(), xy.y(), p.z);
}

}  // namespace lidar_odometry
