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

using namespace std::chrono_literals; // allows the use of suffixes like ms, s, min

class MotionPlanner : public rclcpp::Node
{
public:
MotionPlanner() // constructor function
  : Node("simple_pubsub"), count_(0) // defines two variables
  {
    // create publishers
    joint_targets_pub = this->create_publisher<std_msgs::msg::String>("joint_targets", 10);
    robot_pose_pub = this->create_publisher<std_msgs::msg::String>("robot_pose", 10);
    activate_suction_pub = this->create_publisher<std_msgs::msg::String>("activate_suction", 10);

    // FOR TESTING: calls timer_callback ever 0.5s
    timer_ = this->create_wall_timer(
      500ms, std::bind(&MotionPlanner::timer_callback, this)); 
    auto slow_timer_callback = // a lambda function for fun :)
      [this]() -> void {
        auto msg = std_msgs::msg::String();
        msg.data = "Hello, activate_suction! " + std::to_string(this->count_++);
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msg.data.c_str()); // equivalent of print()
        this->activate_suction_pub->publish(msg); // publishes the message
      };
    slow_timer_ = this->create_wall_timer(5s, slow_timer_callback);
  
    // creates subscriptions
    auto bound_callback = std::bind(&MotionPlanner::sub_callback, this, _1);
    joint_pos_sub = this->create_subscription<std_msgs::msg::String>("joint_pos", 10, bound_callback);
    suction_p_sub = this->create_subscription<std_msgs::msg::String>("suction_pressure", 10, bound_callback);
  }

private:
  void timer_callback()
  {
    auto msg1 = std_msgs::msg::String();
    msg1.data = "Hello, joint_targets! " + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msg1.data.c_str()); // equivalent of print()
    joint_targets_pub->publish(msg1); // publishes the message

    auto msg2 = std_msgs::msg::String();
    msg2.data = "Hello, robot_pose! " + std::to_string(count_);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msg2.data.c_str()); // equivalent of print()
    robot_pose_pub->publish(msg2); // publishes the message
  }
  void sub_callback(const std_msgs::msg::String & msg) {
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::TimerBase::SharedPtr slow_timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr joint_targets_pub;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr robot_pose_pub;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr activate_suction_pub;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr joint_pos_sub;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr suction_p_sub;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MotionPlanner>()); // alternatively use spin_some()
  rclcpp::shutdown();
  return 0;
}
