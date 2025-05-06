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
#include <iostream>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "custom_msgs/msg/suction_request.hpp"
#include "custom_msgs/msg/suction_force.hpp"

#include "custom_msgs/srv/write_data.hpp"
#include "custom_msgs/srv/sc_write_pos.hpp"
#include "custom_msgs/srv/id.hpp"
#include "custom_msgs/srv/sc_write_pwm.hpp"

using std::placeholders::_1;
using std::to_string;

using namespace std::chrono_literals;

const int millis = 1000;
std::vector<int> posData;

struct cupParams{
  int id;
  int state;
};

struct suctionCupStates{
  cupParams front;
  cupParams rear;
};


/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class SuctionCups : public rclcpp::Node
{
public:
SuctionCups() // constructor function
  : Node("suction_cups"), count_(0) // defines two variables
  {
    foot.front = {7,0};
    foot.rear = {4,0};

    suction_force_pub = this->create_publisher<custom_msgs::msg::SuctionForce>("suction_force", 10); // creates the publisher
    timer_ = this->create_wall_timer(
      500ms, std::bind(&SuctionCups::timer_callback, this)); // creates a timer object to run timer_callback every 500ms
    activate_suction_sub = this->create_subscription<custom_msgs::msg::SuctionRequest>("activate_suction", 10, 
      std::bind(&SuctionCups::sub_callback, this, _1));
    
    sc_WriteData_client = this->create_client<custom_msgs::srv::WriteData>("sc_WriteData");
    sc_PWMMode_client = this->create_client<custom_msgs::srv::ID>("sc_PWMMode");
    sc_WritePWM_client = this->create_client<custom_msgs::srv::SCWritePWM>("sc_WritePWM");
    sc_ReadPos_client = this->create_client<custom_msgs::srv::ID>("sc_ReadPos");
    sc_WritePos_client = this->create_client<custom_msgs::srv::SCWritePos>("sc_WritePos");

  }

private:

  suctionCupStates foot;
  int home_pos = 500;


  void timer_callback(){
    ; // currently does nothing
  }


  //funciton to re-centre the suction cup servo on 510 (150 degrees) between rotations to prevent cumulative error 
  void centre(int id){

    sc_WriteData(id, 0x30, 0); //unlock eprom
    sc_WriteData(id, 0x09, 20, true); //set lower angle limit
    sc_WriteData(id, 0x0B, 1003, true); //set upper angle limit
    sc_WriteData(id, 0x30, 1); // lock eprom

    // go to home position
    sc_WritePos(id, home_pos, 0, 800);
    usleep(1000 * millis);

  }


  //function to rotate the servo a given number of times in a given direction -- DEPRECATED
  /*
  void rotate2(int id, int direction, int turns){

    int counter = 0;
    int flagPoint = home_pos;
    int buffer = 25;
    bool flag = true;
    bool prevFlag = true;
    int speed = 800;
    auto switch_time = std::chrono::steady_clock::now();
    double min_circle_time = 0; // in seconds

    centre(id);
    sc.PWMMode(id);
    sc.WritePWM(id, speed * direction);
    usleep(200*millis);

    while(counter < 2 * turns){
      
      usleep(millis);

      int pos = sc.ReadPos(id);
      auto now = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration<double>(now - switch_time).count();
      flag = pos >= (flagPoint - 100 * direction);

      if(flag != prevFlag  && elapsed >= min_circle_time){
        counter ++;
        if(counter % 2 == 0) {
          RCLCPP_INFO(this->get_logger(), "Rotations: %d ", counter/2);
        }
        switch_time = std::chrono::steady_clock::now(); // could just sleep here tbh
      }
      
      prevFlag = flag; 
      
    }

    sc.WritePWM(id, 0);
  }*/

  void rotate(int id, int direction, int turns){
    ROS_INFO(this->get_logger(), "rotate method called");
    
    int counter = 0;
    bool steep_flag = false;
    bool last_steep_flag = steep_flag;
    int speed = 800;

    
    int pos = sc_ReadPos(id);
    int prev_pos = pos;

    int grad_threshhold = 30;
    int low_grad_counter = 0;

    int absurd_number = 1200;

    sc_PWMMode(id);
    sc_WritePWM(id, speed * direction);

    while(counter < turns){

      rclcpp::sleep_for(5ms);

      pos = sc_ReadPos(id);
      posData.push_back(pos);
      if (pos == -1 || pos >= 1500) continue;

      int grad = pos-prev_pos;
      RCLCPP_INFO(this->get_logger(), "Grad: %d ", grad);
      if(grad < 0) grad = -grad;

      if(grad > absurd_number) continue;

      if(grad > grad_threshhold){
        steep_flag = true;
        low_grad_counter = 0;
      }
      else low_grad_counter++;

      if(low_grad_counter >= 8) steep_flag = false;

      if(!steep_flag && last_steep_flag){
        counter++;
        RCLCPP_INFO(this->get_logger(), "Rotations: %u ", counter);
      }

      prev_pos = pos;
      last_steep_flag = steep_flag;
    }

    sc_WritePWM(id, 0);
    centre(id);

    for (const auto & val : posData) {
      std::cout << val << " ";
    }
    std::cout << std::endl;
  }


  void driveLogic(cupParams &cup, int request){
    ROS_INFO(this->get_logger(), "driveLogic method called");

    if(request == 1 && cup.state == 0){
      rotate(cup.id, 1, 6);
      cup.state = 1;
    }
    else if(request == -1 && cup.state == 1){
      rotate(cup.id, -1, 8);
      cup.state = -1;
    }
    else if(request == 0 && cup.state == -1){
      rotate(cup.id, 1, 2);
      cup.state = 0;
    }
    else{
      RCLCPP_ERROR(this->get_logger(), "Unsafe to move foot id %d in this way ", cup.id);
    }
  }
  

  void sub_callback(const custom_msgs::msg::SuctionRequest & msg) {

    RCLCPP_INFO(this->get_logger(), "I heard: 'cup_1=%hhu, cup_2=%hhu'", msg.cup_1, msg.cup_2);

    if(msg.cup_1 != foot.front.state){
      driveLogic(foot.front, msg.cup_1);
    }
    if(msg.cup_2 != foot.rear.state){
      driveLogic(foot.rear, msg.cup_2);
    }
    
  }

  // Service calling functions
  int sc_WriteData(int id, int addr, int val, bool word = false, bool async = true){
    auto request = std::make_shared<custom_msgs::srv::WriteData::Request>();
    request->id = id;
    request->addr = addr;
    request->val = val;
    request->word = word;
    
    auto future = sc_WriteData_client->async_send_request(request);

    if(async) return 0;
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future) ==
      rclcpp::FutureReturnCode::SUCCESS)
    {
      return future.get()->result;
    } else {
      RCLCPP_ERROR(this->get_logger(), "sc_WriteData call failed");
      return -1;
    }
  }

  int sc_PWMMode(int id, bool async = true){
    auto request = std::make_shared<custom_msgs::srv::ID::Request>();
    request->id = id;

    auto future = sc_ReadPos_client->async_send_request(request);

    if(async) return 0;
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future) ==
      rclcpp::FutureReturnCode::SUCCESS)
    {
      return future.get()->result;
    } else {
      RCLCPP_ERROR(this->get_logger(), "sc_PWMMode call failed");
      return -1;
    }
  }

  int sc_WritePWM(int id, int speed, bool async = true){
    auto request = std::make_shared<custom_msgs::srv::SCWritePos::Request>();
    request->id = id;
    request->speed = speed;
    
    auto future = sc_WritePos_client->async_send_request(request);

    if(async) return 0;
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future) ==
      rclcpp::FutureReturnCode::SUCCESS)
    {
      return future.get()->result;
    } else {
      RCLCPP_ERROR(this->get_logger(), "sc_WritePos call failed");
      return -1;
    }
  }

  int sc_ReadPos(int id){
    auto request = std::make_shared<custom_msgs::srv::ID::Request>();
    request->id = id;

    auto future = sc_ReadPos_client->async_send_request(request);

    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future) ==
      rclcpp::FutureReturnCode::SUCCESS)
    {
      return future.get()->result;
    } else {
      RCLCPP_ERROR(this->get_logger(), "sc_ReadPos call failed");
      return -1;
    }
  }

  int sc_WritePos(int id, int pos, int time, int speed = 0, int async = true){
    auto request = std::make_shared<custom_msgs::srv::SCWritePos::Request>();
    request->id = id;
    request->position = pos;
    request->time = time;
    request->speed = speed;
    
    auto future = sc_WritePos_client->async_send_request(request);

    if(async) return 0;
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future) ==
      rclcpp::FutureReturnCode::SUCCESS)
    {
      return future.get()->result;
    } else {
      RCLCPP_ERROR(this->get_logger(), "sc_WritePos call failed");
      return -1;
    }
  }

  // declare service clients
  rclcpp::Client<custom_msgs::srv::WriteData>::SharedPtr sc_WriteData_client;
  rclcpp::Client<custom_msgs::srv::ID>::SharedPtr sc_PWMMode_client;
  rclcpp::Client<custom_msgs::srv::SCWritePWM>::SharedPtr sc_WritePWM_client;
  rclcpp::Client<custom_msgs::srv::ID>::SharedPtr sc_ReadPos_client;
  rclcpp::Client<custom_msgs::srv::SCWritePos>::SharedPtr sc_WritePos_client;


  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<custom_msgs::msg::SuctionForce>::SharedPtr suction_force_pub;
  rclcpp::Subscription<custom_msgs::msg::SuctionRequest>::SharedPtr activate_suction_sub;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SuctionCups>()); // alternatively use spin_some()
  rclcpp::shutdown();
  return 0;
}
