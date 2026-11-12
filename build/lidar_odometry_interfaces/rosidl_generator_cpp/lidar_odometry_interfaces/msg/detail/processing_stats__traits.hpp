// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from lidar_odometry_interfaces:msg/ProcessingStats.idl
// generated code does not contain a copyright notice

#ifndef LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__TRAITS_HPP_
#define LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "lidar_odometry_interfaces/msg/detail/processing_stats__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace lidar_odometry_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const ProcessingStats & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: input_points
  {
    out << "input_points: ";
    rosidl_generator_traits::value_to_yaml(msg.input_points, out);
    out << ", ";
  }

  // member: filtered_points
  {
    out << "filtered_points: ";
    rosidl_generator_traits::value_to_yaml(msg.filtered_points, out);
    out << ", ";
  }

  // member: preprocessing_ms
  {
    out << "preprocessing_ms: ";
    rosidl_generator_traits::value_to_yaml(msg.preprocessing_ms, out);
    out << ", ";
  }

  // member: registration_ms
  {
    out << "registration_ms: ";
    rosidl_generator_traits::value_to_yaml(msg.registration_ms, out);
    out << ", ";
  }

  // member: total_ms
  {
    out << "total_ms: ";
    rosidl_generator_traits::value_to_yaml(msg.total_ms, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ProcessingStats & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: input_points
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "input_points: ";
    rosidl_generator_traits::value_to_yaml(msg.input_points, out);
    out << "\n";
  }

  // member: filtered_points
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "filtered_points: ";
    rosidl_generator_traits::value_to_yaml(msg.filtered_points, out);
    out << "\n";
  }

  // member: preprocessing_ms
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "preprocessing_ms: ";
    rosidl_generator_traits::value_to_yaml(msg.preprocessing_ms, out);
    out << "\n";
  }

  // member: registration_ms
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "registration_ms: ";
    rosidl_generator_traits::value_to_yaml(msg.registration_ms, out);
    out << "\n";
  }

  // member: total_ms
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "total_ms: ";
    rosidl_generator_traits::value_to_yaml(msg.total_ms, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ProcessingStats & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace lidar_odometry_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use lidar_odometry_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const lidar_odometry_interfaces::msg::ProcessingStats & msg,
  std::ostream & out, size_t indentation = 0)
{
  lidar_odometry_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use lidar_odometry_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const lidar_odometry_interfaces::msg::ProcessingStats & msg)
{
  return lidar_odometry_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<lidar_odometry_interfaces::msg::ProcessingStats>()
{
  return "lidar_odometry_interfaces::msg::ProcessingStats";
}

template<>
inline const char * name<lidar_odometry_interfaces::msg::ProcessingStats>()
{
  return "lidar_odometry_interfaces/msg/ProcessingStats";
}

template<>
struct has_fixed_size<lidar_odometry_interfaces::msg::ProcessingStats>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<lidar_odometry_interfaces::msg::ProcessingStats>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<lidar_odometry_interfaces::msg::ProcessingStats>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // LIDAR_ODOMETRY_INTERFACES__MSG__DETAIL__PROCESSING_STATS__TRAITS_HPP_
