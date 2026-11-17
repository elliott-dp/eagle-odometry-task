#pragma once

#include <cstddef>

#include <Eigen/Geometry>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

namespace lidar_odometry {

using PointCloud = pcl::PointCloud<pcl::PointXYZ>;

// Every tunable number of the preprocessing. The values live in config/odometry.yaml,
// the node reads them as ROS parameters and fills this struct.
struct PreprocessorParams {
  // Crop (sensor frame)
  double min_range{};             // [m] closer returns hit the car itself
  double max_range{};             // [m] keep it generous: far buildings help to fix the yaw

  // Ground (base_link frame)
  double ground_band{};           // [m] look for ground only where |z| is below this
  double ground_fit_radius{};     // [m] and only this close to the car, where the scan is dense
  double ransac_distance{};       // [m] max distance from the plane to count as ground
  int ransac_max_iterations{};
  double max_ground_tilt_deg{};   // [deg] planes steeper than this are not the ground
  int min_ground_inliers{};       // with fewer inliers we do not trust the plane
  double ground_clearance{};      // [m] after levelling, drop everything below this height
};

// Result of the ground plane fit.
struct GroundFit {
  bool found = false;
  // Moves the scan into a frame where the ground is the plane z = 0.
  Eigen::Isometry3f leveling = Eigen::Isometry3f::Identity();
  double roll_deg = 0.0;       // tilt of the car measured from the ground
  double pitch_deg = 0.0;
  double height = 0.0;         // [m] height of the base_link origin above the fitted ground
  std::size_t inliers = 0;
};

// Keeps one point per voxel: the centroid of the points that fall inside it.
PointCloud::Ptr voxelDownsample(const PointCloud::ConstPtr & cloud, double voxel_size);

// Turns a raw scan into the points the scan matcher should use.
// It knows nothing about ROS: clouds in, clouds out.
class Preprocessor {
public:
  explicit Preprocessor(const PreprocessorParams & params);

  // Keeps the points whose range from the sensor is between min_range and max_range.
  // The cloud must still be in the sensor frame, so that the range is the one the LiDAR measured.
  PointCloud::Ptr cropByRange(const PointCloud & sensor_cloud) const;

  // Fits the ground plane with RANSAC. The cloud must already be in base_link.
  GroundFit fitGround(const PointCloud & base_cloud) const;

  // Levels the scan with the fitted plane and drops the ground points.
  PointCloud::Ptr levelAndRemoveGround(const PointCloud & base_cloud, const GroundFit & ground) const;

private:
  PointCloud::Ptr selectGroundCandidates(const PointCloud & base_cloud) const;

  PreprocessorParams params_;
};

}  // namespace lidar_odometry
