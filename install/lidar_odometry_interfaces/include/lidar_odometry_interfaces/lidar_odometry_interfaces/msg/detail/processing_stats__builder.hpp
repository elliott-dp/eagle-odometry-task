// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from lidar_odometry_interfaces:msg/ProcessingStats.idl
// generated code does not contain a copyright notice

#ifndef LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__BUILDER_HPP_
#define LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "lidar_odometry_interfaces/msg/detail/processing_stats__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace lidar_odometry_interfaces
{

namespace msg
{

namespace builder
{

class Init_ProcessingStats_total_ms
{
public:
  explicit Init_ProcessingStats_total_ms(::lidar_odometry_interfaces::msg::ProcessingStats & msg)
  : msg_(msg)
  {}
  ::lidar_odometry_interfaces::msg::ProcessingStats total_ms(::lidar_odometry_interfaces::msg::ProcessingStats::_total_ms_type arg)
  {
    msg_.total_ms = std::move(arg);
    return std::move(msg_);
  }

private:
  ::lidar_odometry_interfaces::msg::ProcessingStats msg_;
};

class Init_ProcessingStats_publish_ms
{
public:
  explicit Init_ProcessingStats_publish_ms(::lidar_odometry_interfaces::msg::ProcessingStats & msg)
  : msg_(msg)
  {}
  Init_ProcessingStats_total_ms publish_ms(::lidar_odometry_interfaces::msg::ProcessingStats::_publish_ms_type arg)
  {
    msg_.publish_ms = std::move(arg);
    return Init_ProcessingStats_total_ms(msg_);
  }

private:
  ::lidar_odometry_interfaces::msg::ProcessingStats msg_;
};

class Init_ProcessingStats_map_update_ms
{
public:
  explicit Init_ProcessingStats_map_update_ms(::lidar_odometry_interfaces::msg::ProcessingStats & msg)
  : msg_(msg)
  {}
  Init_ProcessingStats_publish_ms map_update_ms(::lidar_odometry_interfaces::msg::ProcessingStats::_map_update_ms_type arg)
  {
    msg_.map_update_ms = std::move(arg);
    return Init_ProcessingStats_publish_ms(msg_);
  }

private:
  ::lidar_odometry_interfaces::msg::ProcessingStats msg_;
};

class Init_ProcessingStats_registration_ms
{
public:
  explicit Init_ProcessingStats_registration_ms(::lidar_odometry_interfaces::msg::ProcessingStats & msg)
  : msg_(msg)
  {}
  Init_ProcessingStats_map_update_ms registration_ms(::lidar_odometry_interfaces::msg::ProcessingStats::_registration_ms_type arg)
  {
    msg_.registration_ms = std::move(arg);
    return Init_ProcessingStats_map_update_ms(msg_);
  }

private:
  ::lidar_odometry_interfaces::msg::ProcessingStats msg_;
};

class Init_ProcessingStats_preprocessing_ms
{
public:
  explicit Init_ProcessingStats_preprocessing_ms(::lidar_odometry_interfaces::msg::ProcessingStats & msg)
  : msg_(msg)
  {}
  Init_ProcessingStats_registration_ms preprocessing_ms(::lidar_odometry_interfaces::msg::ProcessingStats::_preprocessing_ms_type arg)
  {
    msg_.preprocessing_ms = std::move(arg);
    return Init_ProcessingStats_registration_ms(msg_);
  }

private:
  ::lidar_odometry_interfaces::msg::ProcessingStats msg_;
};

class Init_ProcessingStats_downsample_ms
{
public:
  explicit Init_ProcessingStats_downsample_ms(::lidar_odometry_interfaces::msg::ProcessingStats & msg)
  : msg_(msg)
  {}
  Init_ProcessingStats_preprocessing_ms downsample_ms(::lidar_odometry_interfaces::msg::ProcessingStats::_downsample_ms_type arg)
  {
    msg_.downsample_ms = std::move(arg);
    return Init_ProcessingStats_preprocessing_ms(msg_);
  }

private:
  ::lidar_odometry_interfaces::msg::ProcessingStats msg_;
};

class Init_ProcessingStats_ground_ms
{
public:
  explicit Init_ProcessingStats_ground_ms(::lidar_odometry_interfaces::msg::ProcessingStats & msg)
  : msg_(msg)
  {}
  Init_ProcessingStats_downsample_ms ground_ms(::lidar_odometry_interfaces::msg::ProcessingStats::_ground_ms_type arg)
  {
    msg_.ground_ms = std::move(arg);
    return Init_ProcessingStats_downsample_ms(msg_);
  }

private:
  ::lidar_odometry_interfaces::msg::ProcessingStats msg_;
};

class Init_ProcessingStats_crop_ms
{
public:
  explicit Init_ProcessingStats_crop_ms(::lidar_odometry_interfaces::msg::ProcessingStats & msg)
  : msg_(msg)
  {}
  Init_ProcessingStats_ground_ms crop_ms(::lidar_odometry_interfaces::msg::ProcessingStats::_crop_ms_type arg)
  {
    msg_.crop_ms = std::move(arg);
    return Init_ProcessingStats_ground_ms(msg_);
  }

private:
  ::lidar_odometry_interfaces::msg::ProcessingStats msg_;
};

class Init_ProcessingStats_filtered_points
{
public:
  explicit Init_ProcessingStats_filtered_points(::lidar_odometry_interfaces::msg::ProcessingStats & msg)
  : msg_(msg)
  {}
  Init_ProcessingStats_crop_ms filtered_points(::lidar_odometry_interfaces::msg::ProcessingStats::_filtered_points_type arg)
  {
    msg_.filtered_points = std::move(arg);
    return Init_ProcessingStats_crop_ms(msg_);
  }

private:
  ::lidar_odometry_interfaces::msg::ProcessingStats msg_;
};

class Init_ProcessingStats_input_points
{
public:
  explicit Init_ProcessingStats_input_points(::lidar_odometry_interfaces::msg::ProcessingStats & msg)
  : msg_(msg)
  {}
  Init_ProcessingStats_filtered_points input_points(::lidar_odometry_interfaces::msg::ProcessingStats::_input_points_type arg)
  {
    msg_.input_points = std::move(arg);
    return Init_ProcessingStats_filtered_points(msg_);
  }

private:
  ::lidar_odometry_interfaces::msg::ProcessingStats msg_;
};

class Init_ProcessingStats_header
{
public:
  Init_ProcessingStats_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ProcessingStats_input_points header(::lidar_odometry_interfaces::msg::ProcessingStats::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_ProcessingStats_input_points(msg_);
  }

private:
  ::lidar_odometry_interfaces::msg::ProcessingStats msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::lidar_odometry_interfaces::msg::ProcessingStats>()
{
  return lidar_odometry_interfaces::msg::builder::Init_ProcessingStats_header();
}

}  // namespace lidar_odometry_interfaces

#endif  // LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__BUILDER_HPP_
