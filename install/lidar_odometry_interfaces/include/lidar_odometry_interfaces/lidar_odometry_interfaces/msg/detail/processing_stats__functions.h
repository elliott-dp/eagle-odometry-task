// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from lidar_odometry_interfaces:msg/ProcessingStats.idl
// generated code does not contain a copyright notice

#ifndef LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__FUNCTIONS_H_
#define LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "lidar_odometry_interfaces/msg/rosidl_generator_c__visibility_control.h"

#include "lidar_odometry_interfaces/msg/detail/processing_stats__struct.h"

/// Initialize msg/ProcessingStats message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * lidar_odometry_interfaces__msg__ProcessingStats
 * )) before or use
 * lidar_odometry_interfaces__msg__ProcessingStats__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_lidar_odometry_interfaces
bool
lidar_odometry_interfaces__msg__ProcessingStats__init(lidar_odometry_interfaces__msg__ProcessingStats * msg);

/// Finalize msg/ProcessingStats message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_lidar_odometry_interfaces
void
lidar_odometry_interfaces__msg__ProcessingStats__fini(lidar_odometry_interfaces__msg__ProcessingStats * msg);

/// Create msg/ProcessingStats message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * lidar_odometry_interfaces__msg__ProcessingStats__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_lidar_odometry_interfaces
lidar_odometry_interfaces__msg__ProcessingStats *
lidar_odometry_interfaces__msg__ProcessingStats__create();

/// Destroy msg/ProcessingStats message.
/**
 * It calls
 * lidar_odometry_interfaces__msg__ProcessingStats__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_lidar_odometry_interfaces
void
lidar_odometry_interfaces__msg__ProcessingStats__destroy(lidar_odometry_interfaces__msg__ProcessingStats * msg);

/// Check for msg/ProcessingStats message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_lidar_odometry_interfaces
bool
lidar_odometry_interfaces__msg__ProcessingStats__are_equal(const lidar_odometry_interfaces__msg__ProcessingStats * lhs, const lidar_odometry_interfaces__msg__ProcessingStats * rhs);

/// Copy a msg/ProcessingStats message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_lidar_odometry_interfaces
bool
lidar_odometry_interfaces__msg__ProcessingStats__copy(
  const lidar_odometry_interfaces__msg__ProcessingStats * input,
  lidar_odometry_interfaces__msg__ProcessingStats * output);

/// Initialize array of msg/ProcessingStats messages.
/**
 * It allocates the memory for the number of elements and calls
 * lidar_odometry_interfaces__msg__ProcessingStats__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_lidar_odometry_interfaces
bool
lidar_odometry_interfaces__msg__ProcessingStats__Sequence__init(lidar_odometry_interfaces__msg__ProcessingStats__Sequence * array, size_t size);

/// Finalize array of msg/ProcessingStats messages.
/**
 * It calls
 * lidar_odometry_interfaces__msg__ProcessingStats__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_lidar_odometry_interfaces
void
lidar_odometry_interfaces__msg__ProcessingStats__Sequence__fini(lidar_odometry_interfaces__msg__ProcessingStats__Sequence * array);

/// Create array of msg/ProcessingStats messages.
/**
 * It allocates the memory for the array and calls
 * lidar_odometry_interfaces__msg__ProcessingStats__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_lidar_odometry_interfaces
lidar_odometry_interfaces__msg__ProcessingStats__Sequence *
lidar_odometry_interfaces__msg__ProcessingStats__Sequence__create(size_t size);

/// Destroy array of msg/ProcessingStats messages.
/**
 * It calls
 * lidar_odometry_interfaces__msg__ProcessingStats__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_lidar_odometry_interfaces
void
lidar_odometry_interfaces__msg__ProcessingStats__Sequence__destroy(lidar_odometry_interfaces__msg__ProcessingStats__Sequence * array);

/// Check for msg/ProcessingStats message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_lidar_odometry_interfaces
bool
lidar_odometry_interfaces__msg__ProcessingStats__Sequence__are_equal(const lidar_odometry_interfaces__msg__ProcessingStats__Sequence * lhs, const lidar_odometry_interfaces__msg__ProcessingStats__Sequence * rhs);

/// Copy an array of msg/ProcessingStats messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_lidar_odometry_interfaces
bool
lidar_odometry_interfaces__msg__ProcessingStats__Sequence__copy(
  const lidar_odometry_interfaces__msg__ProcessingStats__Sequence * input,
  lidar_odometry_interfaces__msg__ProcessingStats__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__FUNCTIONS_H_
