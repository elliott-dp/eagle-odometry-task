// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from lidar_odometry_interfaces:msg/ProcessingStats.idl
// generated code does not contain a copyright notice

#ifndef LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__STRUCT_HPP_
#define LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__lidar_odometry_interfaces__msg__ProcessingStats __attribute__((deprecated))
#else
# define DEPRECATED__lidar_odometry_interfaces__msg__ProcessingStats __declspec(deprecated)
#endif

namespace lidar_odometry_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ProcessingStats_
{
  using Type = ProcessingStats_<ContainerAllocator>;

  explicit ProcessingStats_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->input_points = 0ul;
      this->filtered_points = 0ul;
      this->crop_ms = 0.0;
      this->ground_ms = 0.0;
      this->downsample_ms = 0.0;
      this->preprocessing_ms = 0.0;
      this->registration_ms = 0.0;
      this->map_update_ms = 0.0;
      this->publish_ms = 0.0;
      this->total_ms = 0.0;
    }
  }

  explicit ProcessingStats_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->input_points = 0ul;
      this->filtered_points = 0ul;
      this->crop_ms = 0.0;
      this->ground_ms = 0.0;
      this->downsample_ms = 0.0;
      this->preprocessing_ms = 0.0;
      this->registration_ms = 0.0;
      this->map_update_ms = 0.0;
      this->publish_ms = 0.0;
      this->total_ms = 0.0;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _input_points_type =
    uint32_t;
  _input_points_type input_points;
  using _filtered_points_type =
    uint32_t;
  _filtered_points_type filtered_points;
  using _crop_ms_type =
    double;
  _crop_ms_type crop_ms;
  using _ground_ms_type =
    double;
  _ground_ms_type ground_ms;
  using _downsample_ms_type =
    double;
  _downsample_ms_type downsample_ms;
  using _preprocessing_ms_type =
    double;
  _preprocessing_ms_type preprocessing_ms;
  using _registration_ms_type =
    double;
  _registration_ms_type registration_ms;
  using _map_update_ms_type =
    double;
  _map_update_ms_type map_update_ms;
  using _publish_ms_type =
    double;
  _publish_ms_type publish_ms;
  using _total_ms_type =
    double;
  _total_ms_type total_ms;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__input_points(
    const uint32_t & _arg)
  {
    this->input_points = _arg;
    return *this;
  }
  Type & set__filtered_points(
    const uint32_t & _arg)
  {
    this->filtered_points = _arg;
    return *this;
  }
  Type & set__crop_ms(
    const double & _arg)
  {
    this->crop_ms = _arg;
    return *this;
  }
  Type & set__ground_ms(
    const double & _arg)
  {
    this->ground_ms = _arg;
    return *this;
  }
  Type & set__downsample_ms(
    const double & _arg)
  {
    this->downsample_ms = _arg;
    return *this;
  }
  Type & set__preprocessing_ms(
    const double & _arg)
  {
    this->preprocessing_ms = _arg;
    return *this;
  }
  Type & set__registration_ms(
    const double & _arg)
  {
    this->registration_ms = _arg;
    return *this;
  }
  Type & set__map_update_ms(
    const double & _arg)
  {
    this->map_update_ms = _arg;
    return *this;
  }
  Type & set__publish_ms(
    const double & _arg)
  {
    this->publish_ms = _arg;
    return *this;
  }
  Type & set__total_ms(
    const double & _arg)
  {
    this->total_ms = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    lidar_odometry_interfaces::msg::ProcessingStats_<ContainerAllocator> *;
  using ConstRawPtr =
    const lidar_odometry_interfaces::msg::ProcessingStats_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<lidar_odometry_interfaces::msg::ProcessingStats_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<lidar_odometry_interfaces::msg::ProcessingStats_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      lidar_odometry_interfaces::msg::ProcessingStats_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<lidar_odometry_interfaces::msg::ProcessingStats_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      lidar_odometry_interfaces::msg::ProcessingStats_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<lidar_odometry_interfaces::msg::ProcessingStats_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<lidar_odometry_interfaces::msg::ProcessingStats_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<lidar_odometry_interfaces::msg::ProcessingStats_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__lidar_odometry_interfaces__msg__ProcessingStats
    std::shared_ptr<lidar_odometry_interfaces::msg::ProcessingStats_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__lidar_odometry_interfaces__msg__ProcessingStats
    std::shared_ptr<lidar_odometry_interfaces::msg::ProcessingStats_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ProcessingStats_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->input_points != other.input_points) {
      return false;
    }
    if (this->filtered_points != other.filtered_points) {
      return false;
    }
    if (this->crop_ms != other.crop_ms) {
      return false;
    }
    if (this->ground_ms != other.ground_ms) {
      return false;
    }
    if (this->downsample_ms != other.downsample_ms) {
      return false;
    }
    if (this->preprocessing_ms != other.preprocessing_ms) {
      return false;
    }
    if (this->registration_ms != other.registration_ms) {
      return false;
    }
    if (this->map_update_ms != other.map_update_ms) {
      return false;
    }
    if (this->publish_ms != other.publish_ms) {
      return false;
    }
    if (this->total_ms != other.total_ms) {
      return false;
    }
    return true;
  }
  bool operator!=(const ProcessingStats_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ProcessingStats_

// alias to use template instance with default allocator
using ProcessingStats =
  lidar_odometry_interfaces::msg::ProcessingStats_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace lidar_odometry_interfaces

#endif  // LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__STRUCT_HPP_
