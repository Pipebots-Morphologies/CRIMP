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

using std::placeholders::_1;

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class JointMover : public rclcpp::Node
{
public:
JointMover() // constructor function
  : Node("joint_mover"), count_(0) // defines two variables
  {
    joint_pos_pub = this->create_publisher<std_msgs::msg::String>("joint_pos", 10); // creates the publisher to joint_pos topic

    // FOR TESTING: calls timer_callback ever 0.5s
    timer_ = this->create_wall_timer(
      500ms, std::bind(&JointMover::timer_callback, this)); 
    
    joint_target_sub = this->create_subscription<std_msgs::msg::String>("joint_targets", 10, 
      std::bind(&JointMover::sub_callback, this, _1)); // creates a subscriber to joint_targets topic
  }

private:
  // called when the timer triggers
  void timer_callback()
  {
    auto message = std_msgs::msg::String();
    message.data = "Hello, joint_pos! " + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str()); // equivalent of print()
    joint_pos_pub->publish(message); // publishes the message
  }
  // called when joint targets are received
  void sub_callback(const std_msgs::msg::String & msg) {
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr joint_pos_pub;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr joint_target_sub;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<JointMover>()); // alternatively use spin_some()
  rclcpp::shutdown();
  return 0;
}
