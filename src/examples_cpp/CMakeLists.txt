cmake_minimum_required(VERSION 3.8)
project(examples_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(talker src/simple_pub.cpp)
ament_target_dependencies(talker rclcpp std_msgs)

add_executable(listener src/simple_sub.cpp)
ament_target_dependencies(listener rclcpp std_msgs)

add_executable(pubsub src/simple_pubsub.cpp)
ament_target_dependencies(pubsub rclcpp std_msgs)

install(TARGETS
  talker
  listener
  pubsub
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
