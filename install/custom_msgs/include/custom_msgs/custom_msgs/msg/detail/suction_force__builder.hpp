// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from custom_msgs:msg/SuctionForce.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "custom_msgs/msg/suction_force.hpp"


#ifndef CUSTOM_MSGS__MSG__DETAIL__SUCTION_FORCE__BUILDER_HPP_
#define CUSTOM_MSGS__MSG__DETAIL__SUCTION_FORCE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "custom_msgs/msg/detail/suction_force__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace custom_msgs
{

namespace msg
{

namespace builder
{

class Init_SuctionForce_force_2
{
public:
  explicit Init_SuctionForce_force_2(::custom_msgs::msg::SuctionForce & msg)
  : msg_(msg)
  {}
  ::custom_msgs::msg::SuctionForce force_2(::custom_msgs::msg::SuctionForce::_force_2_type arg)
  {
    msg_.force_2 = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_msgs::msg::SuctionForce msg_;
};

class Init_SuctionForce_force_1
{
public:
  Init_SuctionForce_force_1()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SuctionForce_force_2 force_1(::custom_msgs::msg::SuctionForce::_force_1_type arg)
  {
    msg_.force_1 = std::move(arg);
    return Init_SuctionForce_force_2(msg_);
  }

private:
  ::custom_msgs::msg::SuctionForce msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_msgs::msg::SuctionForce>()
{
  return custom_msgs::msg::builder::Init_SuctionForce_force_1();
}

}  // namespace custom_msgs

#endif  // CUSTOM_MSGS__MSG__DETAIL__SUCTION_FORCE__BUILDER_HPP_
