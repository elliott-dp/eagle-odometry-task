#include "lidar_odometry/local_map.hpp"

#include <algorithm>
#include <cmath>

#include <Eigen/Eigenvalues>
#include <pcl/memory.h>

#include "lidar_odometry/pose2d.hpp"

namespace lidar_odometry {

LocalMap::LocalMap(const LocalMapParams & params) : params_(params) {}

bool LocalMap::empty() const { return num_points_ == 0; }

std::size_t LocalMap::size() const { return num_points_; }

std::size_t LocalMap::VoxelHash::operator()(const Voxel & voxel) const
{
  // Multiply each index by a large prime and mix them: the classic spatial hash.
  // Unsigned arithmetic, so negative indices and overflow are fine.
  return static_cast<std::size_t>(voxel.x()) * 73856093u ^
         static_cast<std::size_t>(voxel.y()) * 19349669u ^
         static_cast<std::size_t>(voxel.z()) * 83492791u;
}

LocalMap::Voxel LocalMap::voxelOf(const Eigen::Vector3d & point) const
{
  return (point / params_.voxel_size).array().floor().cast<int>();
}

void LocalMap::addScan(const PointCloud & scan, const Eigen::Isometry2d & pose)
{
  const auto max_points = static_cast<std::size_t>(params_.max_points_per_voxel);
  for (const auto & p : scan.points) {
    const Eigen::Vector3d point = transformPoint(pose, p);
    auto & voxel_points = voxels_[voxelOf(point)];

    // A full voxel keeps its old points. Where the car has already been, the map stays
    // what it saw the first time, so coming back pulls the drift back towards it.
    if (voxel_points.size() < max_points) {
      voxel_points.push_back(point);
      ++num_points_;
    }
  }
}

std::vector<LocalMap::Candidate> LocalMap::pointsAround(const Eigen::Vector3d & query) const
{
  // Look in the query's voxel and in the 26 around it. Voxels are at least as big as the
  // correspondence distance, so no point that close can be missed.
  const Voxel centre = voxelOf(query);
  std::vector<Candidate> candidates;
  candidates.reserve(27 * static_cast<std::size_t>(params_.max_points_per_voxel));
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      for (int dz = -1; dz <= 1; ++dz) {
        const auto voxel = voxels_.find(centre + Voxel(dx, dy, dz));
        if (voxel == voxels_.end()) {
          continue;
        }
        for (const auto & point : voxel->second) {
          candidates.push_back({(point - query).squaredNorm(), point});
        }
      }
    }
  }
  return candidates;
}

std::optional<MapNeighbour> LocalMap::closestPoint(
  const Eigen::Vector3d & query, double max_distance) const
{
  std::vector<Candidate> candidates = pointsAround(query);
  if (candidates.empty()) {
    return std::nullopt;
  }

  // Bring the closest points to the front. Only the first few matter for the plane.
  const std::size_t count =
    std::min(candidates.size(), static_cast<std::size_t>(params_.normal_neighbours));
  std::partial_sort(candidates.begin(), candidates.begin() + count, candidates.end(),
                    [](const Candidate & a, const Candidate & b) {
                      return a.squared_distance < b.squared_distance;
                    });

  const Candidate & closest = candidates.front();
  if (closest.squared_distance > max_distance * max_distance) {
    return std::nullopt;  // too far: probably not the same surface
  }

  // The plane goes through the points closest to the query, q included.
  const auto normal = fitPlaneNormal(candidates, count);
  if (!normal) {
    return std::nullopt;
  }
  return MapNeighbour{closest.point, *normal};
}

std::optional<Eigen::Vector3d> LocalMap::fitPlaneNormal(
  const std::vector<Candidate> & points, std::size_t count)
{
  if (count < 3) {
    return std::nullopt;  // fewer than three points do not define a plane
  }

  Eigen::Vector3d mean = Eigen::Vector3d::Zero();
  for (std::size_t i = 0; i < count; ++i) {
    mean += points[i].point;
  }
  mean /= static_cast<double>(count);

  Eigen::Matrix3d covariance = Eigen::Matrix3d::Zero();
  for (std::size_t i = 0; i < count; ++i) {
    const Eigen::Vector3d d = points[i].point - mean;
    covariance += d * d.transpose();
  }

  // The normal is the direction in which the points spread the least:
  // the eigenvector of the smallest eigenvalue (Eigen sorts them in increasing order).
  Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver;
  solver.computeDirect(covariance);  // closed-form solution for 3x3, much faster than iterating
  return solver.eigenvectors().col(0);
}

PointCloud::Ptr LocalMap::toPointCloud() const
{
  auto cloud = pcl::make_shared<PointCloud>();
  cloud->reserve(num_points_);
  for (const auto & voxel : voxels_) {
    for (const auto & p : voxel.second) {
      cloud->push_back(pcl::PointXYZ(static_cast<float>(p.x()), static_cast<float>(p.y()),
                                     static_cast<float>(p.z())));
    }
  }
  return cloud;
}

}  // namespace lidar_odometry