// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from custom_msgs:msg/SuctionRequest.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "custom_msgs/msg/suction_request.hpp"


#ifndef CUSTOM_MSGS__MSG__DETAIL__SUCTION_REQUEST__BUILDER_HPP_
#define CUSTOM_MSGS__MSG__DETAIL__SUCTION_REQUEST__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "custom_msgs/msg/detail/suction_request__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace custom_msgs
{

namespace msg
{

namespace builder
{

class Init_SuctionRequest_cup_2
{
public:
  explicit Init_SuctionRequest_cup_2(::custom_msgs::msg::SuctionRequest & msg)
  : msg_(msg)
  {}
  ::custom_msgs::msg::SuctionRequest cup_2(::custom_msgs::msg::SuctionRequest::_cup_2_type arg)
  {
    msg_.cup_2 = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_msgs::msg::SuctionRequest msg_;
};

class Init_SuctionRequest_cup_1
{
public:
  Init_SuctionRequest_cup_1()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SuctionRequest_cup_2 cup_1(::custom_msgs::msg::SuctionRequest::_cup_1_type arg)
  {
    msg_.cup_1 = std::move(arg);
    return Init_SuctionRequest_cup_2(msg_);
  }

private:
  ::custom_msgs::msg::SuctionRequest msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_msgs::msg::SuctionRequest>()
{
  return custom_msgs::msg::builder::Init_SuctionRequest_cup_1();
}

}  // namespace custom_msgs

#endif  // CUSTOM_MSGS__MSG__DETAIL__SUCTION_REQUEST__BUILDER_HPP_
