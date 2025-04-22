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

class MinimalPubSub : public rclcpp::Node
{
public:
  MinimalPubSub() // constructor function
  : Node("simple_pubsub"), count_(0) // defines two variables
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic1", 10); // creates the publisher
    timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPubSub::timer_callback, this)); // creates a timer object to run timer_callback every 500ms
    subscription_ = this->create_subscription<std_msgs::msg::String>("topic2", 10, 
      std::bind(&MinimalPubSub::sub_callback, this, _1));
  }

private:
  void timer_callback()
  {
    auto message = std_msgs::msg::String();
    message.data = "Hello, world! " + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str()); // equivalent of print()
    publisher_->publish(message); // publishes the message
  }
  void sub_callback(const std_msgs::msg::String & msg) {
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPubSub>()); // alternatively use spin_some()
  rclcpp::shutdown();
  return 0;
}
