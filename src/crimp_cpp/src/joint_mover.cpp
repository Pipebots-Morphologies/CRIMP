// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

using std::placeholders::_1;
using std::to_string;

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class JointMover : public rclcpp::Node
{
public:
JointMover() // constructor function
  : Node("joint_mover"), count_(0) // defines two variables
  {
    joint_pos_pub = this->create_publisher<sensor_msgs::msg::JointState>("joint_pos", 10); // creates the publisher to joint_pos topic

    // FOR TESTING: calls timer_callback ever 0.5s
    timer_ = this->create_wall_timer(
      500ms, std::bind(&JointMover::timer_callback, this)); 
    
    joint_target_sub = this->create_subscription<sensor_msgs::msg::JointState>("joint_targets", 10, 
      std::bind(&JointMover::sub_callback, this, _1)); // creates a subscriber to joint_targets topic
  }

private:
  // called when the timer triggers
  void timer_callback()
  {
    count_++;
    auto msg = sensor_msgs::msg::JointState();
    msg.name = {"joint2"};
    msg.position = {count_*10};
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s position=%f'", msg.name[0].c_str(), msg.position[0]); // equivalent of print()
    joint_pos_pub->publish(msg); // publishes the message
  }
  // called when joint targets are received
  void sub_callback(const sensor_msgs::msg::JointState & msg) {
    auto pos = to_string(msg.position[0]).c_str();
    RCLCPP_INFO(this->get_logger(), "I heard: '%s position = %f'", msg.name[0].c_str(), msg.position[0]);
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pos_pub;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_target_sub;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<JointMover>()); // alternatively use spin_some()
  rclcpp::shutdown();
  return 0;
}
