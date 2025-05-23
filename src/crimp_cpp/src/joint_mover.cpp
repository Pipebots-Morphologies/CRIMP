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
#include <map>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "SCServo.h"

using std::placeholders::_1;
using std::to_string;

using namespace std::chrono_literals;

class JointMover : public rclcpp::Node
{
public:
JointMover() // constructor function
  : Node("joint_mover"), count_(0) // defines two variables
  {

    const char* serialPort = "/dev/ttyAMA0";
    RCLCPP_INFO(this->get_logger(), "Using serial port: %s", serialPort);

    if (!sc.begin(1000000, serialPort)) {
        RCLCPP_ERROR(this->get_logger(), "Failed to init SCSCL motor!");
    }

    if (!sm_st.begin(1000000, serialPort)) {
        RCLCPP_ERROR(this->get_logger(), "Failed to init SCSCL motor!");
    }

    joint_pos_pub = this->create_publisher<sensor_msgs::msg::JointState>("joint_pos", 10); // creates the publisher to joint_pos topic

    // FOR TESTING: calls timer_callback ever 0.5s
    timer_ = this->create_wall_timer(
      500ms, std::bind(&JointMover::get_joint_angles, this)); 
    
    joint_target_sub = this->create_subscription<sensor_msgs::msg::JointState>("joint_targets", 10, 
      std::bind(&JointMover::sub_callback, this, _1)); // creates a subscriber to joint_targets topic
  }

private:

  SCSCL sc;     // sc.WritePos(id, uint16 position, uint16 time, uint16 velocity);
  SMS_STS sm_st;   // st.WritePosEx(id, int16 position, uint16 velocity, uint8 ACC=0);

  // called when the timer triggers, gets and publishes the joint angles
  void get_joint_angles()
  {
    for(int i = 0; i < joint_names.size(); i++){
      std::string joint_name = joint_names[i];
      int id = joint_ids[joint_name];
      float pos;
      if(joint_name[0]=='w'){
        int steps = sc.ReadPos(id);
        pos = sc_steps2angle(steps);
        // NEED TO CONVERT FROM STEPS TO DEGREES
      }else if(joint_name[0]=='e'){
        int steps = sm_st.ReadPos(id);
        pos = st_steps2angle(steps);
        // NEED TO CONVERT FROM STEPS TO DEGREES
      }
      joint_angles[joint_name] = pos;
      // ROS_INFO(this->get_logger(), "Publishing to joint_pos topic") // uncomment for debugging
    }

    auto msg = sensor_msgs::msg::JointState();
    msg.name = joint_names;
    std::vector<double> joint_angle_vector;
    for(auto& pair : joint_angles){
      joint_angle_vector.push_back(pair.second);
    }
    msg.position = joint_angle_vector;
    joint_pos_pub->publish(msg);
  }
  // called when joint targets are received
  void sub_callback(const sensor_msgs::msg::JointState & msg) {
    //RCLCPP_INFO(this->get_logger(), "Message received from joint_targets topic") // uncomment for debugging
    for(int i = 0; i < msg.name.size(); i++){
      if(msg.name[i][0]=='w'){
        wrist_move(joint_ids[msg.name[i]], msg.position[i], msg.velocity[i]);
      }else if(msg.name[i][0]=='e'){
        elbow_move(joint_ids[msg.name[i]], msg.position[i], msg.velocity[i]);
      }else{
        RCLCPP_WARN(this->get_logger(), "WARNING: unknown joint name '%s'", msg.name[i].c_str());
      }
    }
  }
  // moves a wrist joint to a target position at a set velocity or over a set time
  void wrist_move(int id, float position, int velocity, int time=0){
    if(position >= 150) position = 150; // joint max limit
    if(position <= 0) position = 0; // joint min limit
    int steps = sc_angle2steps(position);
    if(velocity == 0){velocity = st_default_vel; }
    if(time==0){
      sc.WritePos(id, steps, time, velocity);
    }
  }
  // moves an elbow joint to a target position at a set velocity
  void elbow_move(int id, float position, int velocity){
    if(position >= 300) position = 300; // joint max limit
    if(position <= 60) position = 60; // joint min limit
    int steps = st_angle2steps(position);
    if(velocity == 0){velocity = st_default_vel; }
    sm_st.WritePosEx(id, steps, velocity, 50);
  }
  // convert between steps and angles
  float sc_steps2angle(int steps){
    float angle = (steps/1023.0)*150.0;
    return angle;
  }
  int sc_angle2steps(float angle){
    int steps = (angle/150.0)*1023.0;
    return steps;
  }
  float st_steps2angle(int steps){
    float angle = (steps/4095.0)*360.0;
    return angle;
  }
  int st_angle2steps(float angle){
    int steps = (angle/360.0)*4095.0;
    return steps;
  }
  // define variables
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pos_pub;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_target_sub;
  size_t count_;
  std::vector<std::string> joint_names = {"elbow_1", "elbow_2", "elbow_3", "wrist_1", "wrist_2"};
  std::map<std::string, int> joint_ids = {
    {joint_names[0], 3},
    {joint_names[1], 2},
    {joint_names[2], 1},
    {joint_names[3], 6},
    {joint_names[4], 5}
  };
  // change this to float when we switch from steps to degrees 
  std::map<std::string, float> joint_angles = {
    {joint_names[0], 180},
    {joint_names[1], 180},
    {joint_names[2], 180},
    {joint_names[3], 75},
    {joint_names[4], 75}
  };
  int sc_default_vel = 400; // default sc servo velocity in steps/second
  int st_default_vel = 1200; // default st servo velocity in steps/second
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<JointMover>()); // alternatively use spin_some()
  rclcpp::shutdown();
  return 0;
}