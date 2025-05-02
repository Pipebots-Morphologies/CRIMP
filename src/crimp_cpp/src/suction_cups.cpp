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
#include "custom_msgs/msg/suction_request.hpp"
#include "custom_msgs/msg/suction_force.hpp"
#include "SCServo.h"

using std::placeholders::_1;
using std::to_string;

using namespace std::chrono_literals;

const int millis = 1000;

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

    const char* serialPort = "/dev/ttyAMA0";
    RCLCPP_INFO(this->get_logger(), "Using serial port: %s", serialPort);

    if (!sc.begin(1000000, serialPort)) {
      RCLCPP_ERROR(this->get_logger(), "Failed to init SCSCL motor!");
    }

    foot.front = {7,0};
    foot.rear = {4,0};

    suction_force_pub = this->create_publisher<custom_msgs::msg::SuctionForce>("suction_force", 10); // creates the publisher
    timer_ = this->create_wall_timer(
      500ms, std::bind(&SuctionCups::timer_callback, this)); // creates a timer object to run timer_callback every 500ms
    activate_suction_sub = this->create_subscription<custom_msgs::msg::SuctionRequest>("activate_suction", 10, 
      std::bind(&SuctionCups::sub_callback, this, _1));

  }

private:

  SCSCL sc;
  suctionCupStates foot;
  int home_pos = 500;


  void timer_callback(){
    ; // currently does nothing
  }


  //funciton to re-centre the suction cup servo on 510 (150 degrees) between rotations to prevent cumulative error 
  void centre(int id){

    sc.writeByte(id, 0x30, 0); //unlock eprom
    sc.writeWord(id, 0x09, 20); //set lower angle limit
    sc.writeWord(id, 0x0B, 1003); //set upper angle limit
    sc.writeByte(id, 0x30, 0); // lock eprom

    // go to home position
    sc.WritePos(id, home_pos, 0, 800);
    usleep(1000 * millis);

  }


  //function to rotate the servo a given number of times in a given direction
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
  }

  void rotate(int id, int direction, int turns){
    
    int counter = 0;
    bool steep_flag = false;
    bool last_steep_flag = steep_flag;
    int speed = 800;

    
    int pos = sc.ReadPos(id);
    int prev_pos = pos;

    int grad_threshhold = 30;
    int low_grad_counter = 0;

    int absurd_number = 1200;

    sc.PWMMode(id);
    sc.WritePWM(id, speed * direction);

    while(counter < turns){

      rclcpp::sleep_for(5ms);

      pos = sc.ReadPos(id);
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

    sc.WritePWM(id, 0);
    centre(id);
  }


  void driveLogic(cupParams &cup, int request){
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
