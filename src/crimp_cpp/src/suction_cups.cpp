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

using std::placeholders::_1;
using std::to_string;

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class SuctionCups : public rclcpp::Node
{
public:
SuctionCups() // constructor function
  : Node("suction_cups"), count_(0) // defines two variables
  {
    suction_force_pub = this->create_publisher<custom_msgs::msg::SuctionForce>("suction_force", 10); // creates the publisher
    timer_ = this->create_wall_timer(
      500ms, std::bind(&SuctionCups::timer_callback, this)); // creates a timer object to run timer_callback every 500ms
    activate_suction_sub = this->create_subscription<custom_msgs::msg::SuctionRequest>("activate_suction", 10, 
      std::bind(&SuctionCups::sub_callback, this, _1));
  }

private:
  void timer_callback()
  {
    count_++;
    auto msg = custom_msgs::msg::SuctionForce();
    msg.force_1 = count_;
    msg.force_2 = 10000-msg.force_1;
    RCLCPP_INFO(this->get_logger(), "Publishing: 'force_1=%u, force_2=%u'", msg.force_1, msg.force_2); // equivalent of print()
    suction_force_pub->publish(msg); // publishes the msg
  }
  void sub_callback(const custom_msgs::msg::SuctionRequest & msg) {
    RCLCPP_INFO(this->get_logger(), "I heard: 'cup_1=%hhu, cup_2=%hhu'", msg.cup_1, msg.cup_2);
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
