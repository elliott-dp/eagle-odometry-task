#include "lidar_odometry/preprocessor.hpp"

#include <cmath>

#include <pcl/ModelCoefficients.h>
#include <pcl/PointIndices.h>
#include <pcl/common/angles.h>
#include <pcl/common/transforms.h>
#include <pcl/memory.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>

#include <pcl/filters/voxel_grid.h>

namespace lidar_odometry {
  
Preprocessor::Preprocessor(const PreprocessorParams & params) : params_(params) {}

PointCloud::Ptr Preprocessor::cropByRange(const PointCloud & sensor_cloud) const
{
  // Compare squared distances, no need for a square root per point.
  const double min_sq = params_.min_range * params_.min_range;
  const double max_sq = params_.max_range * params_.max_range;

  auto cropped = pcl::make_shared<PointCloud>();
  cropped->reserve(sensor_cloud.size());
  for (const auto & p : sensor_cloud.points) {
    const double range_sq = p.x * p.x + p.y * p.y + p.z * p.z;
    if (range_sq >= min_sq && range_sq <= max_sq) {
      cropped->push_back(p);
    }
  }
  return cropped;
}

PointCloud::Ptr Preprocessor::selectGroundCandidates(const PointCloud & base_cloud) const
{
  // The ground can only be close to z = 0 in base_link. Near the car the scan is dense,
  // so fitting there is fast and walls or buildings far away cannot pull the plane.
  const double radius_sq = params_.ground_fit_radius * params_.ground_fit_radius;

  auto candidates = pcl::make_shared<PointCloud>();
  for (const auto & p : base_cloud.points) {
    const double horizontal_sq = p.x * p.x + p.y * p.y;
    if (std::abs(p.z) < params_.ground_band && horizontal_sq < radius_sq) {
      candidates->push_back(p);
    }
  }
  return candidates;
}

GroundFit Preprocessor::fitGround(const PointCloud & base_cloud) const
{
  GroundFit fit;
  const auto min_inliers = static_cast<std::size_t>(params_.min_ground_inliers);

  const auto candidates = selectGroundCandidates(base_cloud);
  if (candidates->size() < min_inliers) {
    return fit;  // not even enough points near z = 0 to try
  }

  // RANSAC on planes whose normal is close to +z, so a wall can never win.
  pcl::SACSegmentation<pcl::PointXYZ> ransac;
  ransac.setModelType(pcl::SACMODEL_PERPENDICULAR_PLANE);
  ransac.setMethodType(pcl::SAC_RANSAC);
  ransac.setAxis(Eigen::Vector3f::UnitZ());
  ransac.setEpsAngle(pcl::deg2rad(params_.max_ground_tilt_deg));
  ransac.setDistanceThreshold(params_.ransac_distance);
  ransac.setMaxIterations(params_.ransac_max_iterations);
  ransac.setOptimizeCoefficients(true);
  ransac.setInputCloud(candidates);

  pcl::PointIndices inliers;
  pcl::ModelCoefficients plane;
  ransac.segment(inliers, plane);

  fit.inliers = inliers.indices.size();
  if (fit.inliers < min_inliers) {
    return fit;
  }

  // Plane equation: a*x + b*y + c*z + d = 0. Normalise it and make the normal point up.
  Eigen::Vector3f normal(plane.values[0], plane.values[1], plane.values[2]);
  float d = plane.values[3];
  const float length = normal.norm();
  normal /= length;
  d /= length;
  if (normal.z() < 0.0f) {
    normal = -normal;
    d = -d;
  }

  // Rotate the ground normal onto +z, then the ground sits at z = -d: shift it up by d.
  const Eigen::Quaternionf rotation = Eigen::Quaternionf::FromTwoVectors(normal, Eigen::Vector3f::UnitZ());
  fit.leveling = Eigen::Translation3f(0.0f, 0.0f, d) * rotation;

  // The ground normal seen from the car tells how much the suspension tilts it.
  fit.roll_deg = pcl::rad2deg(std::atan2(normal.y(), normal.z()));
  fit.pitch_deg = pcl::rad2deg(std::atan2(-normal.x(), normal.z()));
  fit.height = d;
  fit.found = true;
  return fit;
}

PointCloud::Ptr Preprocessor::levelAndRemoveGround(
  const PointCloud & base_cloud, const GroundFit & ground) const
{
  // If no plane was found, leveling is the identity and we just cut at the clearance height.
  auto leveled = pcl::make_shared<PointCloud>();
  pcl::transformPointCloud(base_cloud, *leveled, ground.leveling.matrix());

  auto above_ground = pcl::make_shared<PointCloud>();
  above_ground->reserve(leveled->size());
  for (const auto & p : leveled->points) {
    if (p.z > params_.ground_clearance) {
      above_ground->push_back(p);
    }
  }
  return above_ground;
}

  PointCloud::Ptr voxelDownsample(const PointCloud::ConstPtr & cloud, double voxel_size)
{
  const auto size = static_cast<float>(voxel_size);
  pcl::VoxelGrid<pcl::PointXYZ> grid;
  grid.setInputCloud(cloud);
  grid.setLeafSize(size, size, size);

  auto downsampled = pcl::make_shared<PointCloud>();
  grid.filter(*downsampled);
  return downsampled;
}

}  // namespace lidar_odometry