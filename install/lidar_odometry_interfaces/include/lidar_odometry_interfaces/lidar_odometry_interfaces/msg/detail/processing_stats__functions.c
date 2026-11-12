// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from lidar_odometry_interfaces:msg/ProcessingStats.idl
// generated code does not contain a copyright notice
#include "lidar_odometry_interfaces/msg/detail/processing_stats__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"

bool
lidar_odometry_interfaces__msg__ProcessingStats__init(lidar_odometry_interfaces__msg__ProcessingStats * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    lidar_odometry_interfaces__msg__ProcessingStats__fini(msg);
    return false;
  }
  // input_points
  // filtered_points
  // preprocessing_ms
  // registration_ms
  // total_ms
  return true;
}

void
lidar_odometry_interfaces__msg__ProcessingStats__fini(lidar_odometry_interfaces__msg__ProcessingStats * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // input_points
  // filtered_points
  // preprocessing_ms
  // registration_ms
  // total_ms
}

bool
lidar_odometry_interfaces__msg__ProcessingStats__are_equal(const lidar_odometry_interfaces__msg__ProcessingStats * lhs, const lidar_odometry_interfaces__msg__ProcessingStats * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // input_points
  if (lhs->input_points != rhs->input_points) {
    return false;
  }
  // filtered_points
  if (lhs->filtered_points != rhs->filtered_points) {
    return false;
  }
  // preprocessing_ms
  if (lhs->preprocessing_ms != rhs->preprocessing_ms) {
    return false;
  }
  // registration_ms
  if (lhs->registration_ms != rhs->registration_ms) {
    return false;
  }
  // total_ms
  if (lhs->total_ms != rhs->total_ms) {
    return false;
  }
  return true;
}

bool
lidar_odometry_interfaces__msg__ProcessingStats__copy(
  const lidar_odometry_interfaces__msg__ProcessingStats * input,
  lidar_odometry_interfaces__msg__ProcessingStats * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // input_points
  output->input_points = input->input_points;
  // filtered_points
  output->filtered_points = input->filtered_points;
  // preprocessing_ms
  output->preprocessing_ms = input->preprocessing_ms;
  // registration_ms
  output->registration_ms = input->registration_ms;
  // total_ms
  output->total_ms = input->total_ms;
  return true;
}

lidar_odometry_interfaces__msg__ProcessingStats *
lidar_odometry_interfaces__msg__ProcessingStats__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  lidar_odometry_interfaces__msg__ProcessingStats * msg = (lidar_odometry_interfaces__msg__ProcessingStats *)allocator.allocate(sizeof(lidar_odometry_interfaces__msg__ProcessingStats), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(lidar_odometry_interfaces__msg__ProcessingStats));
  bool success = lidar_odometry_interfaces__msg__ProcessingStats__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
lidar_odometry_interfaces__msg__ProcessingStats__destroy(lidar_odometry_interfaces__msg__ProcessingStats * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    lidar_odometry_interfaces__msg__ProcessingStats__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
lidar_odometry_interfaces__msg__ProcessingStats__Sequence__init(lidar_odometry_interfaces__msg__ProcessingStats__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  lidar_odometry_interfaces__msg__ProcessingStats * data = NULL;

  if (size) {
    data = (lidar_odometry_interfaces__msg__ProcessingStats *)allocator.zero_allocate(size, sizeof(lidar_odometry_interfaces__msg__ProcessingStats), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = lidar_odometry_interfaces__msg__ProcessingStats__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        lidar_odometry_interfaces__msg__ProcessingStats__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
lidar_odometry_interfaces__msg__ProcessingStats__Sequence__fini(lidar_odometry_interfaces__msg__ProcessingStats__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      lidar_odometry_interfaces__msg__ProcessingStats__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

lidar_odometry_interfaces__msg__ProcessingStats__Sequence *
lidar_odometry_interfaces__msg__ProcessingStats__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  lidar_odometry_interfaces__msg__ProcessingStats__Sequence * array = (lidar_odometry_interfaces__msg__ProcessingStats__Sequence *)allocator.allocate(sizeof(lidar_odometry_interfaces__msg__ProcessingStats__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = lidar_odometry_interfaces__msg__ProcessingStats__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
lidar_odometry_interfaces__msg__ProcessingStats__Sequence__destroy(lidar_odometry_interfaces__msg__ProcessingStats__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    lidar_odometry_interfaces__msg__ProcessingStats__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
lidar_odometry_interfaces__msg__ProcessingStats__Sequence__are_equal(const lidar_odometry_interfaces__msg__ProcessingStats__Sequence * lhs, const lidar_odometry_interfaces__msg__ProcessingStats__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!lidar_odometry_interfaces__msg__ProcessingStats__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
lidar_odometry_interfaces__msg__ProcessingStats__Sequence__copy(
  const lidar_odometry_interfaces__msg__ProcessingStats__Sequence * input,
  lidar_odometry_interfaces__msg__ProcessingStats__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(lidar_odometry_interfaces__msg__ProcessingStats);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    lidar_odometry_interfaces__msg__ProcessingStats * data =
      (lidar_odometry_interfaces__msg__ProcessingStats *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!lidar_odometry_interfaces__msg__ProcessingStats__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          lidar_odometry_interfaces__msg__ProcessingStats__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!lidar_odometry_interfaces__msg__ProcessingStats__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
