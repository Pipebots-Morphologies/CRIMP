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
#include "std_msgs/msg/bool.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "custom_msgs/srv/suction_control.hpp"

using std::placeholders::_1;
using std::to_string;

using namespace std::chrono_literals; // allows the use of suffixes like ms, s, min

class MotionPlanner : public rclcpp::Node
{
public:
MotionPlanner() // constructor function
  : Node("motion_planner") 
  {
    // create publishers
    joint_targets_pub = this->create_publisher<sensor_msgs::msg::JointState>("joint_targets", 10);
    robot_pose_pub = this->create_publisher<geometry_msgs::msg::Pose>("robot_pose", 10);

    suction_control_client = this->create_client<custom_msgs::srv::SuctionControl>("suction_control");
  
    // creates subscriptions
    joint_pos_sub = this->create_subscription<sensor_msgs::msg::JointState>("joint_pos", 10, 
      std::bind(&MotionPlanner::joint_pos_callback, this, _1));
    start_sub = this->create_subscription<std_msgs::msg::Bool>("start", 1, 
      std::bind(&MotionPlanner::begin, this, _1));
  }

private:

  void joint_pos_callback(const sensor_msgs::msg::JointState & msg) {
    RCLCPP_INFO(this->get_logger(), "I heard: '%s position = %f'", msg.name[0].c_str(), msg.position[0]);
  }

  void begin(const std_msgs::msg::Bool &msg){
    if(msg.data == false) return;

    closed_position();

    int back_foot = 2;
    while(true){
      for(int back_foot = 1; back_foot < 3; back_foot++)
        // attach back foot
        
        // detach front foot
        // calc global to local transform
        for(int stage = 0; stage < 3; stage++){
          // set target pose
          // calculate current pose
          // publish target joint angles
        }
        // do
          // push up
          // measure load
          // while(load < threshhold)
    }
  }

  void beginIK(const std_msgs::msg::Bool &msg){
    if(msg.data == false) return;

    closed_position();

    // start loop
      // attach foot
      // detach front foot
      // calc global to local transform
      for(int stage = 0; stage < 3; stage++){
        // set target pose
        // measure foot tilt
        // calculate current pose
        // perform inverse kinematics
        // calculate velocity profiles
        // publish IK solutions
      }
      // do
        // push up
        // measure load
        // while(load < threshhold)
    //end loop
  }

  void set_position(float elbow_1, float elbow_2, float elbow_3){
    auto msg = sensor_msgs::msg::JointState();

    msg.name = {"elbow_1", "elbow_2", "elbow_3"};
    msg.position = {elbow_1, elbow_2, elbow_3};

    joint_targets_pub->publish(msg);
  }

  void closed_position(){
    set_position(150, 60, 210);
  }

  void open_position(){
    set_position(105, 150, 255);
  }


  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_targets_pub;
  rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr robot_pose_pub;

  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_pos_sub;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr start_sub;

  rclcpp::Client<custom_msgs::srv::SuctionControl>::SharedPtr suction_control_client;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MotionPlanner>()); // alternatively use spin_some()
  rclcpp::shutdown();
  return 0;
}
