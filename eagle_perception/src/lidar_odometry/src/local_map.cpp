//
// Created by Saif Edine Safi on 16/11/2026.
//

#include "lidar_odometry/local_map.hpp"

#include <pcl/features/normal_3d.h>
#include <pcl/memory.h>

#include "lidar_odometry/pose2d.hpp"

namespace lidar_odometry {

LocalMap::LocalMap(const LocalMapParams & params)
: params_(params),
  points_(pcl::make_shared<PointCloud>()),
  search_tree_(pcl::make_shared<pcl::search::KdTree<pcl::PointXYZ>>())
{
}

bool LocalMap::empty() const { return points_->empty(); }

std::size_t LocalMap::size() const { return points_->size(); }

double LocalMap::voxelSize() const { return params_.voxel_size; }

void LocalMap::addScan(const PointCloud & scan, const Eigen::Isometry2d & pose)
{
  auto in_odom = pcl::make_shared<PointCloud>();
  in_odom->reserve(scan.size());
  for (const auto & p : scan.points) {
    const Eigen::Vector3d q = transformPoint(pose, p);
    in_odom->push_back(pcl::PointXYZ(static_cast<float>(q.x()), static_cast<float>(q.y()),
                                     static_cast<float>(q.z())));
  }

  // Keep a sliding window: the oldest scan leaves when a new one arrives.
  scans_.push_back(in_odom);
  if (scans_.size() > static_cast<std::size_t>(params_.max_scans)) {
    scans_.pop_front();
  }
  rebuild();
}

void LocalMap::rebuild()
{
  // Merge the window, then downsample again: where scans overlap,
  // the same wall would otherwise be stored once per scan.
  auto merged = pcl::make_shared<PointCloud>();
  for (const auto & scan : scans_) {
    *merged += *scan;
  }
  points_ = voxelDownsample(merged, params_.voxel_size);
  search_tree_->setInputCloud(points_);

  // The old normals belong to the old points: start with an empty cache.
  normals_.assign(points_->size(), CachedNormal{});
}

std::optional<MapNeighbour> LocalMap::closestPoint(
  const Eigen::Vector3d & point, double max_distance) const
{
  const pcl::PointXYZ query(static_cast<float>(point.x()), static_cast<float>(point.y()),
                            static_cast<float>(point.z()));
  pcl::Indices index(1);
  std::vector<float> squared_distance(1);
  if (search_tree_->nearestKSearch(query, 1, index, squared_distance) == 0) {
    return std::nullopt;
  }
  if (squared_distance[0] > max_distance * max_distance) {
    return std::nullopt;  // too far: probably not the same surface
  }

  const CachedNormal & cached = normalAt(index[0]);
  if (!cached.valid) {
    return std::nullopt;  // the points around here do not form a plane
  }
  const auto & q = points_->points[index[0]];
  return MapNeighbour{Eigen::Vector3d(q.x, q.y, q.z), cached.normal};
}

const LocalMap::CachedNormal & LocalMap::normalAt(int index) const
{
  CachedNormal & cached = normals_[index];
  if (!cached.computed) {
    // Fit a plane to the closest map points; its normal is the surface normal here.
    pcl::Indices neighbours;
    std::vector<float> squared_distances;
    search_tree_->nearestKSearch(points_->points[index], params_.normal_neighbours, neighbours,
                                 squared_distances);
    Eigen::Vector4f plane;
    float curvature = 0.0f;
    cached.valid = pcl::computePointNormal(*points_, neighbours, plane, curvature);
    cached.normal = plane.head<3>().cast<double>();
    cached.computed = true;
  }
  return cached;
}

}  // namespace lidar_odometry
