cmake_minimum_required(VERSION 3.8)
project(crimp_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(custom_msgs REQUIRED)

# Include directories
include_directories(
  include
  ${custom_msgs_DIR}/../../../include
)


file(GLOB_RECURSE SERVO_SOURCES "src/lib/*.cpp")
add_library(servo_lib ${SERVO_SOURCES})


# create executables
add_executable(joint_mover src/joint_mover.cpp)
ament_target_dependencies(joint_mover rclcpp sensor_msgs custom_msgs)
target_link_libraries(joint_mover servo_lib)

add_executable(motion_planner src/motion_planner.cpp)
ament_target_dependencies(motion_planner rclcpp sensor_msgs geometry_msgs custom_msgs std_msgs)
target_link_libraries(motion_planner servo_lib)

add_executable(suction_cups src/suction_cups.cpp)
ament_target_dependencies(suction_cups rclcpp custom_msgs)
target_link_libraries(suction_cups servo_lib)

add_executable(servo_node src/servo_node.cpp)
ament_target_dependencies(servo_node rclcpp sensor_msgs custom_msgs)
target_link_libraries(servo_node servo_lib)



install(TARGETS
  joint_mover
  motion_planner
  suction_cups
  servo_node
  DESTINATION lib/${PROJECT_NAME})


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
