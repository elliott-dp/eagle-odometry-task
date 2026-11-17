#pragma once

#include <Eigen/Geometry>

#include "lidar_odometry/local_map.hpp"
#include "lidar_odometry/preprocessor.hpp"

namespace lidar_odometry {

struct ScanMatcherParams {
  int max_iterations{};
  double max_correspondence_distance{};  // [m] pairs farther apart than this are dropped
  double robust_kernel_scale{};          // [m] a pair with this residual gets half the weight
  double convergence_epsilon{};          // stop when the update (m and rad) is smaller than this
  int min_correspondences{};             // with fewer pairs the result is not trusted
};

struct RegistrationResult {
  Eigen::Isometry2d pose = Eigen::Isometry2d::Identity();
  bool valid = false;      // enough pairs were found to trust the pose
  bool converged = false;  // the update became tiny before the iteration cap
  int iterations = 0;
  int correspondences = 0;
};

// Point-to-plane ICP solved with Gauss-Newton. Only x, y and yaw are estimated:
// the scan is levelled, so the flat ground already fixes z, roll and pitch.
class ScanMatcher {
public:
  explicit ScanMatcher(const ScanMatcherParams & params);

  RegistrationResult align(const PointCloud & scan, const LocalMap & map,
                           const Eigen::Isometry2d & initial_guess) const;

private:
  // Sums of w * J^T J and w * J^T r over all point-to-plane pairs.
  struct NormalEquations {
    Eigen::Matrix3d H = Eigen::Matrix3d::Zero();
    Eigen::Vector3d b = Eigen::Vector3d::Zero();
    int correspondences = 0;
  };

  NormalEquations buildNormalEquations(const PointCloud & scan, const LocalMap & map,
                                       const Eigen::Isometry2d & pose) const;
  double robustWeight(double residual) const;

  ScanMatcherParams params_;
};

}  // namespace lidar_odometry
