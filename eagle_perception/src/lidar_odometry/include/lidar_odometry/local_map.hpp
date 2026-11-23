#pragma once

#include <cstddef>
#include <optional>
#include <unordered_map>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include "lidar_odometry/preprocessor.hpp"

namespace lidar_odometry {

struct LocalMapParams {
  double voxel_size{};          // [m] side of a map voxel, >= the max correspondence distance
  int max_points_per_voxel{};   // a full voxel keeps its old points and ignores new ones
  int normal_neighbours{};      // map points used to fit the plane behind each normal
};

// A map point and the normal of the surface it lies on.
struct MapNeighbour {
  Eigen::Vector3d point;
  Eigen::Vector3d normal;
};

// The points seen so far, in the odom frame, stored in a hash of voxels.
// Normals are fitted on demand from the points closest to each query.
// It knows nothing about ROS.
class LocalMap {
public:
  explicit LocalMap(const LocalMapParams & params);

  bool empty() const;
  std::size_t size() const;

  // Moves the scan into odom with the car pose and adds it to the voxels that are not full.
  void addScan(const PointCloud & scan, const Eigen::Isometry2d & pose);

  // Closest map point to `query` (in odom). Empty if it is farther than max_distance
  // or if its normal could not be fitted.
  std::optional<MapNeighbour> closestPoint(const Eigen::Vector3d & query, double max_distance) const;

  // Every map point, for publishing.
  PointCloud::Ptr toPointCloud() const;

private:
  // Integer coordinates of a voxel: floor(point / voxel_size).
  using Voxel = Eigen::Vector3i;

  struct VoxelHash {
    std::size_t operator()(const Voxel & voxel) const;
  };

  // A map point near the query, with its squared distance to it.
  struct Candidate {
    double squared_distance;
    Eigen::Vector3d point;
  };

  Voxel voxelOf(const Eigen::Vector3d & point) const;

  // Every point in the query's voxel and in the 26 around it.
  std::vector<Candidate> pointsAround(const Eigen::Vector3d & query) const;

  // Normal of the plane through the first `count` points. Empty if count < 3.
  static std::optional<Eigen::Vector3d> fitPlaneNormal(
    const std::vector<Candidate> & points, std::size_t count);

  LocalMapParams params_;
  std::unordered_map<Voxel, std::vector<Eigen::Vector3d>, VoxelHash> voxels_;
  std::size_t num_points_ = 0;  // across all voxels, so size() does not have to count them
};

}  // namespace lidar_odometry
