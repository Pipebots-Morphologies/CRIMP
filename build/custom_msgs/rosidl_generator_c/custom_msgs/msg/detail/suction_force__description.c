// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from custom_msgs:msg/SuctionForce.idl
// generated code does not contain a copyright notice

#include "custom_msgs/msg/detail/suction_force__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_custom_msgs
const rosidl_type_hash_t *
custom_msgs__msg__SuctionForce__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xb6, 0x35, 0x8e, 0xda, 0x51, 0x14, 0x2a, 0x90,
      0x55, 0xe4, 0x73, 0xbf, 0x75, 0xf2, 0x84, 0x7f,
      0xc5, 0x20, 0xfa, 0x03, 0x58, 0xea, 0x58, 0xa5,
      0x5c, 0xfa, 0xfd, 0x6f, 0xa6, 0xdd, 0x24, 0x32,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char custom_msgs__msg__SuctionForce__TYPE_NAME[] = "custom_msgs/msg/SuctionForce";

// Define type names, field names, and default values
static char custom_msgs__msg__SuctionForce__FIELD_NAME__force_1[] = "force_1";
static char custom_msgs__msg__SuctionForce__FIELD_NAME__force_2[] = "force_2";

static rosidl_runtime_c__type_description__Field custom_msgs__msg__SuctionForce__FIELDS[] = {
  {
    {custom_msgs__msg__SuctionForce__FIELD_NAME__force_1, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {custom_msgs__msg__SuctionForce__FIELD_NAME__force_2, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT16,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
custom_msgs__msg__SuctionForce__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {custom_msgs__msg__SuctionForce__TYPE_NAME, 28, 28},
      {custom_msgs__msg__SuctionForce__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "uint16 force_1\n"
  "uint16 force_2";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
custom_msgs__msg__SuctionForce__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {custom_msgs__msg__SuctionForce__TYPE_NAME, 28, 28},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 30, 30},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
custom_msgs__msg__SuctionForce__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *custom_msgs__msg__SuctionForce__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
