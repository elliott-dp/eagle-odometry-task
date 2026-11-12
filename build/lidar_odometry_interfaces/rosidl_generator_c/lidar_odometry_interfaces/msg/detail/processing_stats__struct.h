// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from lidar_odometry_interfaces:msg/ProcessingStats.idl
// generated code does not contain a copyright notice

#ifndef LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__STRUCT_H_
#define LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"

/// Struct defined in msg/ProcessingStats in the package lidar_odometry_interfaces.
/**
  * added preprocessing and registration to better analyze the stats and find botttlenecks
 */
typedef struct lidar_odometry_interfaces__msg__ProcessingStats
{
  /// stamp = timestamp della scansione
  std_msgs__msg__Header header;
  /// how many points in the raw scan
  uint32_t input_points;
  uint32_t filtered_points;
  double preprocessing_ms;
  double registration_ms;
  double total_ms;
} lidar_odometry_interfaces__msg__ProcessingStats;

// Struct for a sequence of lidar_odometry_interfaces__msg__ProcessingStats.
typedef struct lidar_odometry_interfaces__msg__ProcessingStats__Sequence
{
  lidar_odometry_interfaces__msg__ProcessingStats * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} lidar_odometry_interfaces__msg__ProcessingStats__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__STRUCT_H_
