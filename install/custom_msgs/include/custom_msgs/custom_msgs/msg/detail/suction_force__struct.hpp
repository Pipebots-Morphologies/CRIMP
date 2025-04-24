// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from custom_msgs:msg/SuctionForce.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "custom_msgs/msg/suction_force.hpp"


#ifndef CUSTOM_MSGS__MSG__DETAIL__SUCTION_FORCE__STRUCT_HPP_
#define CUSTOM_MSGS__MSG__DETAIL__SUCTION_FORCE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__custom_msgs__msg__SuctionForce __attribute__((deprecated))
#else
# define DEPRECATED__custom_msgs__msg__SuctionForce __declspec(deprecated)
#endif

namespace custom_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct SuctionForce_
{
  using Type = SuctionForce_<ContainerAllocator>;

  explicit SuctionForce_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->force_1 = 0;
      this->force_2 = 0;
    }
  }

  explicit SuctionForce_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->force_1 = 0;
      this->force_2 = 0;
    }
  }

  // field types and members
  using _force_1_type =
    uint16_t;
  _force_1_type force_1;
  using _force_2_type =
    uint16_t;
  _force_2_type force_2;

  // setters for named parameter idiom
  Type & set__force_1(
    const uint16_t & _arg)
  {
    this->force_1 = _arg;
    return *this;
  }
  Type & set__force_2(
    const uint16_t & _arg)
  {
    this->force_2 = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    custom_msgs::msg::SuctionForce_<ContainerAllocator> *;
  using ConstRawPtr =
    const custom_msgs::msg::SuctionForce_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<custom_msgs::msg::SuctionForce_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<custom_msgs::msg::SuctionForce_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      custom_msgs::msg::SuctionForce_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<custom_msgs::msg::SuctionForce_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      custom_msgs::msg::SuctionForce_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<custom_msgs::msg::SuctionForce_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<custom_msgs::msg::SuctionForce_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<custom_msgs::msg::SuctionForce_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__custom_msgs__msg__SuctionForce
    std::shared_ptr<custom_msgs::msg::SuctionForce_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__custom_msgs__msg__SuctionForce
    std::shared_ptr<custom_msgs::msg::SuctionForce_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SuctionForce_ & other) const
  {
    if (this->force_1 != other.force_1) {
      return false;
    }
    if (this->force_2 != other.force_2) {
      return false;
    }
    return true;
  }
  bool operator!=(const SuctionForce_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SuctionForce_

// alias to use template instance with default allocator
using SuctionForce =
  custom_msgs::msg::SuctionForce_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace custom_msgs

#endif  // CUSTOM_MSGS__MSG__DETAIL__SUCTION_FORCE__STRUCT_HPP_
