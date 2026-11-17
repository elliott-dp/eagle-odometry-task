cmake_minimum_required(VERSION 3.8)
project(lidar_odometry)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_eigen REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(PCL REQUIRED COMPONENTS common filters segmentation sample_consensus features search kdtree)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(odometry_node
        src/main.cpp
        src/odometry_node.cpp
        src/preprocessor.cpp
        src/local_map.cpp
        src/scan_matcher.cpp
)
target_compile_features(odometry_node PUBLIC cxx_std_17)
target_include_directories(odometry_node PRIVATE include)
# SYSTEM: the PCL headers themselves trigger -Wpedantic warnings, this keeps our build clean.
target_include_directories(odometry_node SYSTEM PRIVATE ${PCL_INCLUDE_DIRS})
target_link_libraries(odometry_node ${PCL_LIBRARIES} Eigen3::Eigen)
ament_target_dependencies(odometry_node rclcpp sensor_msgs std_msgs tf2_ros tf2_eigen pcl_conversions)

install(TARGETS odometry_node DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch config DESTINATION share/${PROJECT_NAME})


ament_package()
