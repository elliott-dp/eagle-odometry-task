# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target lidar_odometry_interfaces::lidar_odometry_interfaces
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${lidar_odometry_interfaces_TARGETS}.
if(lidar_odometry_interfaces_TARGETS AND NOT TARGET lidar_odometry_interfaces::lidar_odometry_interfaces)
  add_library(lidar_odometry_interfaces::lidar_odometry_interfaces INTERFACE IMPORTED)
  set_target_properties(lidar_odometry_interfaces::lidar_odometry_interfaces PROPERTIES
    INTERFACE_LINK_LIBRARIES "${lidar_odometry_interfaces_TARGETS}")
endif()
