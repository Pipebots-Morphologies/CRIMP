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
const int front_leg_id = 7;
const int rear_leg_id = 4;

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

    suction_force_pub = this->create_publisher<custom_msgs::msg::SuctionForce>("suction_force", 10); // creates the publisher
    timer_ = this->create_wall_timer(
      500ms, std::bind(&SuctionCups::timer_callback, this)); // creates a timer object to run timer_callback every 500ms
    activate_suction_sub = this->create_subscription<custom_msgs::msg::SuctionRequest>("activate_suction", 10, 
      std::bind(&SuctionCups::sub_callback, this, _1));
  }

private:

  SCSCL sc;

  void centre(int id){
    sc.writeByte(id, 0x30, 0);
    sc.writeWord(id, 0x09, 20);     // CW angle limit
    sc.writeWord(id, 0x0B, 1003);
    sc.writeByte(id, 0x30, 0);

    sc.WritePos(id, 510, 0, 800);
    usleep(1000 * millis);
  }

  void attach(int id){

    centre(id);

    int rotations = 4;
    int Counter = 0;
    bool Rotation = false;
    int StrtPos = sc.ReadPos(id);
    int Pos = sc.ReadPos(id);

    RCLCPP_INFO(this->get_logger(), "attaching ID: %u", id);
    RCLCPP_INFO(this->get_logger(), "starting position: %u ", Pos);

    sc.PWMMode(id);
    sc.WritePWM(id, 800);
    usleep(250 * millis);

    while(Counter < rotations){
        Pos = sc.ReadPos(id);
        if(Pos >= StrtPos - 5 && Pos <= StrtPos + 5){
            Counter += 1;    
            Rotation = true;
                  
        }
        if(Rotation){
            if(Counter == rotations) usleep(millis);      
            else usleep(250 * millis);
            RCLCPP_INFO(this->get_logger(), "Rotations: %u ", Counter);
            Rotation = false;          
        }
        if(!Rotation){
          usleep(millis);
        }
    }
    sc.WritePWM(id, 0);

    RCLCPP_INFO(this->get_logger(), "final position: %u ", sc.ReadPos(id));
  }

  void detatch(int id){

    centre(id);

    int rotations = 4;
    int Counter = 0;
    bool Rotation = false;
    int StrtPos = sc.ReadPos(id);
    int Pos = sc.ReadPos(id);

    RCLCPP_INFO(this->get_logger(), "attaching ID: %u", id);
    RCLCPP_INFO(this->get_logger(), "starting position: %u ", Pos);

    sc.PWMMode(id);
    sc.WritePWM(id, -800);
    usleep(250 * millis);

    while(Counter < rotations){
        Pos = sc.ReadPos(id);
        if(Pos >= StrtPos - 5 && Pos <= StrtPos + 5){
            Counter += 1;    
            Rotation = true;
            RCLCPP_INFO(this->get_logger(), "Rotations: %u ", Counter);      
        }
        if(Rotation){
            if(Counter == rotations) usleep(millis);      
            else usleep(250 * millis);
            Rotation = false;          
        }
        if(!Rotation){
          usleep(millis);
        }
    }
    sc.WritePWM(id, 0);

    RCLCPP_INFO(this->get_logger(), "final position: %u ", sc.ReadPos(id));
  }

  void timer_callback(){
    count_++;
    auto msg = custom_msgs::msg::SuctionForce();
    msg.force_1 = count_;
    msg.force_2 = 10000-msg.force_1;
    //RCLCPP_INFO(this->get_logger(), "Publishing: 'force_1=%u, force_2=%u'", msg.force_1, msg.force_2); // equivalent of print()
    suction_force_pub->publish(msg); // publishes the msg
  }

  void sub_callback(const custom_msgs::msg::SuctionRequest & msg) {

    RCLCPP_INFO(this->get_logger(), "I heard: 'cup_1=%hhu, cup_2=%hhu'", msg.cup_1, msg.cup_2);

    if(msg.cup_1 == 1) attach(front_leg_id);
    if(msg.cup_1 == -1) detatch(front_leg_id);
    if(msg.cup_2 == 1) attach(rear_leg_id);
    if(msg.cup_2 == -1) detatch(rear_leg_id);

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
