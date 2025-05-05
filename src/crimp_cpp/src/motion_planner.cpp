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
#include "sensor_msgs/msg/joint_state.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "custom_msgs/msg/suction_request.hpp"
#include "custom_msgs/msg/suction_force.hpp"

using std::placeholders::_1;
using std::to_string;

using namespace std::chrono_literals; // allows the use of suffixes like ms, s, min

class MotionPlanner : public rclcpp::Node
{
public:
MotionPlanner() // constructor function
  : Node("simple_pubsub"), count_(0) // defines two variables
  {
    // create publishers
    joint_targets_pub = this->create_publisher<sensor_msgs::msg::JointState>("joint_targets", 10);
    robot_pose_pub = this->create_publisher<geometry_msgs::msg::Pose>("robot_pose", 10);
    activate_suction_pub = this->create_publisher<custom_msgs::msg::SuctionRequest>("activate_suction", 10);

    // FOR TESTING: calls timer_callback ever 0.5s
    timer_ = this->create_wall_timer(
      500ms, std::bind(&MotionPlanner::timer_callback, this)); 
    auto slow_timer_callback = // a lambda function for fun :)
      [this]() -> void {
        auto request_msg= custom_msgs::msg::SuctionRequest();
        request_msg.cup_1 = (this->count_)%3 - 1; // cycles through -1, 0, 1
        request_msg.cup_2 = (this->count_)%3 - 1;
        auto cup_1 = to_string(request_msg.cup_1).c_str();
        auto cup_2 = to_string(request_msg.cup_2).c_str();
        RCLCPP_INFO(this->get_logger(), "Publishing: cup_1=%hhu, cup_2=%hhu", request_msg.cup_1, request_msg.cup_2); // equivalent of print()
        this->activate_suction_pub->publish(request_msg); // publishes the message
      };
    slow_timer_ = this->create_wall_timer(5s, slow_timer_callback);
  
    // creates subscriptions
    joint_pos_sub = this->create_subscription<sensor_msgs::msg::JointState>("joint_pos", 10, 
      std::bind(&MotionPlanner::joint_pos_callback, this, _1));
    suction_force_sub = this->create_subscription<custom_msgs::msg::SuctionForce>("suction_force", 10, 
      std::bind(&MotionPlanner::suction_force_callback, this, _1));
  }

private:
  void timer_callback()
  { 
    count_++;
    auto target_msg = sensor_msgs::msg::JointState();
    target_msg.name = {"joint1"};
    target_msg.position = {count_};
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s position=%f'", target_msg.name[0].c_str(), target_msg.position[0]); // equivalent of print()
    joint_targets_pub->publish(target_msg); // publishes the message

    auto pose_msg = geometry_msgs::msg::Pose();
    pose_msg.position.x = count_;
    RCLCPP_INFO(this->get_logger(), "Publishing: 'position.x=%f'", pose_msg.position.x); // equivalent of print()
    robot_pose_pub->publish(pose_msg); // publishes the message
  }

  void joint_pos_callback(const sensor_msgs::msg::JointState & msg) {
    RCLCPP_INFO(this->get_logger(), "I heard: '%s position = %f'", msg.name[0].c_str(), msg.position[0]);
  }
  void suction_force_callback(const custom_msgs::msg::SuctionForce & msg) {
    RCLCPP_INFO(this->get_logger(), "I heard: 'force_1=%u, force_2=%u'", msg.force_1, msg.force_2);
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::TimerBase::SharedPtr slow_timer_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_targets_pub;
  rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr robot_pose_pub;
  rclcpp::Publisher<custom_msgs::msg::SuctionRequest>::SharedPtr activate_suction_pub;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_pos_sub;
  rclcpp::Subscription<custom_msgs::msg::SuctionForce>::SharedPtr suction_force_sub;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MotionPlanner>()); // alternatively use spin_some()
  rclcpp::shutdown();
  return 0;
}
