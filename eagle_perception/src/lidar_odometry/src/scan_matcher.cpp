#include "lidar_odometry/scan_matcher.hpp"

#include "lidar_odometry/pose2d.hpp"

namespace lidar_odometry {

ScanMatcher::ScanMatcher(const ScanMatcherParams & params) : params_(params) {}

RegistrationResult ScanMatcher::align(
  const PointCloud & scan, const LocalMap & map, const Eigen::Isometry2d & initial_guess) const
{
  // The three numbers we optimise.
  double x = initial_guess.translation().x();
  double y = initial_guess.translation().y();
  double yaw = yawOf(initial_guess);

  RegistrationResult result;
  for (int iteration = 1; iteration <= params_.max_iterations; ++iteration) {
    const NormalEquations equations = buildNormalEquations(scan, map, makePose2D(x, y, yaw));
    result.iterations = iteration;
    result.correspondences = equations.correspondences;

    if (equations.correspondences < params_.min_correspondences) {
      result.pose = initial_guess;  // not enough pairs: do not trust anything we computed
      return result;
    }

    // Solve (sum w J^T J) delta = -(sum w J^T r) and move the pose by delta.
    const Eigen::Vector3d delta = equations.H.ldlt().solve(-equations.b);
    x += delta.x();
    y += delta.y();
    yaw += delta.z();

    if (delta.norm() < params_.convergence_epsilon) {
      result.converged = true;
      break;
    }
  }

  result.valid = true;
  result.pose = makePose2D(x, y, yaw);
  return result;
}

ScanMatcher::NormalEquations ScanMatcher::buildNormalEquations(
  const PointCloud & scan, const LocalMap & map, const Eigen::Isometry2d & pose) const
{
  const Eigen::Vector2d t = pose.translation();
  NormalEquations equations;

  for (const auto & point : scan.points) {
    // 1. Move the scan point into odom with the current guess.
    const Eigen::Vector3d p = transformPoint(pose, point);

    // 2. Pair it with the closest map point, if it is close enough.
    const auto neighbour = map.closestPoint(p, params_.max_correspondence_distance);
    if (!neighbour) {
      continue;
    }

    // 3. The normal of the map surface at that point.
    const Eigen::Vector3d & n = neighbour->normal;

    // 4. Distance from the plane, and how it changes with x, y and yaw.
    const double r = n.dot(p - neighbour->point);
    const Eigen::Vector3d J(n.x(), n.y(), n.y() * (p.x() - t.x()) - n.x() * (p.y() - t.y()));

    // 5. Add the pair to the 3x3 system, trusting large residuals less.
    const double w = robustWeight(r);
    equations.H += w * J * J.transpose();
    equations.b += w * J * r;
    ++equations.correspondences;
  }
  return equations;
}

// Cauchy kernel: weight 1 for a perfect pair, 0.5 when the residual equals the scale,
// and close to 0 for pairs that are clearly wrong.
double ScanMatcher::robustWeight(double residual) const
{
  const double ratio = residual / params_.robust_kernel_scale;
  return 1.0 / (1.0 + ratio * ratio);
}

}  // namespace lidar_odometry
