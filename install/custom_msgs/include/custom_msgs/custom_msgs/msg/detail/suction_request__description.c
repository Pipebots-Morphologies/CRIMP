// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from custom_msgs:msg/SuctionRequest.idl
// generated code does not contain a copyright notice

#include "custom_msgs/msg/detail/suction_request__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_custom_msgs
const rosidl_type_hash_t *
custom_msgs__msg__SuctionRequest__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xa2, 0x94, 0x08, 0x97, 0xfc, 0x1e, 0x53, 0xda,
      0x0b, 0x3d, 0xf9, 0x4c, 0xda, 0xe9, 0xef, 0xdf,
      0xbc, 0xa9, 0xe6, 0xb2, 0xf5, 0x0c, 0x4b, 0xc0,
      0x28, 0x58, 0x1f, 0xc9, 0xb3, 0x54, 0x19, 0x57,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char custom_msgs__msg__SuctionRequest__TYPE_NAME[] = "custom_msgs/msg/SuctionRequest";

// Define type names, field names, and default values
static char custom_msgs__msg__SuctionRequest__FIELD_NAME__cup_1[] = "cup_1";
static char custom_msgs__msg__SuctionRequest__FIELD_NAME__cup_2[] = "cup_2";

static rosidl_runtime_c__type_description__Field custom_msgs__msg__SuctionRequest__FIELDS[] = {
  {
    {custom_msgs__msg__SuctionRequest__FIELD_NAME__cup_1, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {custom_msgs__msg__SuctionRequest__FIELD_NAME__cup_2, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
custom_msgs__msg__SuctionRequest__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {custom_msgs__msg__SuctionRequest__TYPE_NAME, 30, 30},
      {custom_msgs__msg__SuctionRequest__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "int8 cup_1\n"
  "int8 cup_2";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
custom_msgs__msg__SuctionRequest__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {custom_msgs__msg__SuctionRequest__TYPE_NAME, 30, 30},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 22, 22},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
custom_msgs__msg__SuctionRequest__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *custom_msgs__msg__SuctionRequest__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
