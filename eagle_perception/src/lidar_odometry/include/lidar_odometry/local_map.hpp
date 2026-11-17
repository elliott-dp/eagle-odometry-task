#pragma once

#include <cstddef>
#include <deque>
#include <optional>
#include <vector>

#include <Eigen/Geometry>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/search/kdtree.h>

#include "lidar_odometry/preprocessor.hpp"

namespace lidar_odometry {

struct LocalMapParams {
  double voxel_size{};        // [m] map resolution, finer than the scan that gets registered
  int max_scans{};            // the map is made of the most recent scans only
  int normal_neighbours{};    // map points used to fit the plane behind each normal
};

// A map point and the normal of the surface it lies on.
struct MapNeighbour {
  Eigen::Vector3d point;
  Eigen::Vector3d normal;
};

// The last few scans, merged in the odom frame, with a normal for every point.
// It knows nothing about ROS.
class LocalMap {
public:
  explicit LocalMap(const LocalMapParams & params);

  bool empty() const;
  std::size_t size() const;
  double voxelSize() const;

  // Moves the scan into odom with the car pose and rebuilds the map.
  void addScan(const PointCloud & scan, const Eigen::Isometry2d & pose);

  // Closest map point to `point` (in odom). Empty if it is farther than max_distance
  // or if its normal could not be fitted.
  std::optional<MapNeighbour> closestPoint(const Eigen::Vector3d & point, double max_distance) const;

private:
  struct CachedNormal {
    bool computed = false;
    bool valid = false;  // false when the neighbours do not define a plane
    Eigen::Vector3d normal = Eigen::Vector3d::Zero();
  };

  void rebuild();
  const CachedNormal & normalAt(int index) const;

  LocalMapParams params_;
  std::deque<PointCloud::Ptr> scans_;  // recent scans, already in the odom frame
  PointCloud::Ptr points_;             // all of them merged and downsampled
  pcl::search::KdTree<pcl::PointXYZ>::Ptr search_tree_;

  // normals_[i] belongs to points_[i]. Fitting all of them takes longer than the whole
  // scan budget, so each one is fitted the first time registration needs it.
  // mutable: filling this cache does not change what the map contains.
  mutable std::vector<CachedNormal> normals_;
};

}  // namespace lidar_odometry
