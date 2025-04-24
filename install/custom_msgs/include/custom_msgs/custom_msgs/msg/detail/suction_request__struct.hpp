// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from custom_msgs:msg/SuctionRequest.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "custom_msgs/msg/suction_request.hpp"


#ifndef CUSTOM_MSGS__MSG__DETAIL__SUCTION_REQUEST__STRUCT_HPP_
#define CUSTOM_MSGS__MSG__DETAIL__SUCTION_REQUEST__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__custom_msgs__msg__SuctionRequest __attribute__((deprecated))
#else
# define DEPRECATED__custom_msgs__msg__SuctionRequest __declspec(deprecated)
#endif

namespace custom_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct SuctionRequest_
{
  using Type = SuctionRequest_<ContainerAllocator>;

  explicit SuctionRequest_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->cup_1 = 0;
      this->cup_2 = 0;
    }
  }

  explicit SuctionRequest_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->cup_1 = 0;
      this->cup_2 = 0;
    }
  }

  // field types and members
  using _cup_1_type =
    int8_t;
  _cup_1_type cup_1;
  using _cup_2_type =
    int8_t;
  _cup_2_type cup_2;

  // setters for named parameter idiom
  Type & set__cup_1(
    const int8_t & _arg)
  {
    this->cup_1 = _arg;
    return *this;
  }
  Type & set__cup_2(
    const int8_t & _arg)
  {
    this->cup_2 = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    custom_msgs::msg::SuctionRequest_<ContainerAllocator> *;
  using ConstRawPtr =
    const custom_msgs::msg::SuctionRequest_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<custom_msgs::msg::SuctionRequest_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<custom_msgs::msg::SuctionRequest_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      custom_msgs::msg::SuctionRequest_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<custom_msgs::msg::SuctionRequest_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      custom_msgs::msg::SuctionRequest_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<custom_msgs::msg::SuctionRequest_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<custom_msgs::msg::SuctionRequest_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<custom_msgs::msg::SuctionRequest_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__custom_msgs__msg__SuctionRequest
    std::shared_ptr<custom_msgs::msg::SuctionRequest_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__custom_msgs__msg__SuctionRequest
    std::shared_ptr<custom_msgs::msg::SuctionRequest_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SuctionRequest_ & other) const
  {
    if (this->cup_1 != other.cup_1) {
      return false;
    }
    if (this->cup_2 != other.cup_2) {
      return false;
    }
    return true;
  }
  bool operator!=(const SuctionRequest_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SuctionRequest_

// alias to use template instance with default allocator
using SuctionRequest =
  custom_msgs::msg::SuctionRequest_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace custom_msgs

#endif  // CUSTOM_MSGS__MSG__DETAIL__SUCTION_REQUEST__STRUCT_HPP_
