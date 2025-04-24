// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from custom_msgs:msg/SuctionForce.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "custom_msgs/msg/suction_force.h"


#ifndef CUSTOM_MSGS__MSG__DETAIL__SUCTION_FORCE__STRUCT_H_
#define CUSTOM_MSGS__MSG__DETAIL__SUCTION_FORCE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Struct defined in msg/SuctionForce in the package custom_msgs.
typedef struct custom_msgs__msg__SuctionForce
{
  uint16_t force_1;
  uint16_t force_2;
} custom_msgs__msg__SuctionForce;

// Struct for a sequence of custom_msgs__msg__SuctionForce.
typedef struct custom_msgs__msg__SuctionForce__Sequence
{
  custom_msgs__msg__SuctionForce * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} custom_msgs__msg__SuctionForce__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CUSTOM_MSGS__MSG__DETAIL__SUCTION_FORCE__STRUCT_H_
