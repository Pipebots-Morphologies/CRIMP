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
#include "SCServo.h"

#include "custom_msgs/srv/write_data.hpp"
#include "custom_msgs/srv/sc_write_pos.hpp"
#include "custom_msgs/srv/id.hpp"
#include "custom_msgs/srv/sc_write_pwm.hpp"
#include "custom_msgs/srv/st_write_pos.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

class Serial : public rclcpp::Node
{
public:
Serial() // constructor function
    : Node("serial")
    {
        const char* serialPort = "/dev/ttyAMA0";
        RCLCPP_INFO(this->get_logger(), "Using serial port: %s", serialPort);

        if (!sc.begin(1000000, serialPort)) {
            RCLCPP_ERROR(this->get_logger(), "Failed to init SCSCL motor!");
        }

        if (!st.begin(1000000, serialPort)) {
            RCLCPP_ERROR(this->get_logger(), "Failed to init ST motor!");
        }
        
        sc_WriteData_srv = this->create_service<custom_msgs::srv::WriteData>(
            "sc_WriteData", std::bind(&Serial::sc_WriteData, this, _1, _2));

        sc_WritePos_srv = this->create_service<custom_msgs::srv::SCWritePos>(
            "sc_WritePos", std::bind(&Serial::sc_WritePos, this, _1, _2));

        sc_PWMMode_srv = this->create_service<custom_msgs::srv::ID>(
            "sc_PWMMode", std::bind(&Serial::sc_PWMMode, this, _1, _2));

        sc_WritePWM_srv = this->create_service<custom_msgs::srv::SCWritePWM>(
            "sc_WritePWM", std::bind(&Serial::sc_WritePWM, this, _1, _2));
        
        sc_ReadPos_srv = this->create_service<custom_msgs::srv::ID>(
            "sc_ReadPos", std::bind(&Serial::sc_ReadPos, this, _1, _2));
        
        st_WritePos_srv = this->create_service<custom_msgs::srv::STWritePos>(
            "st_WritePosEx", std::bind(&Serial::st_WritePos, this, _1, _2));

        st_ReadPos_srv = this->create_service<custom_msgs::srv::ID>(
            "st_ReadPos", std::bind(&Serial::st_ReadPos, this, _1, _2));
    }

private:

    SCSCL sc; 
    SMS_STS st;

    void sc_WriteData(
        const std::shared_ptr<custom_msgs::srv::WriteData::Request> request,
        std::shared_ptr<custom_msgs::srv::WriteData::Response> response) 
    {
        int id = request->id;
        int addr = request->addr;
        int val = request->val;
        if(request->word) response->result = sc.writeWord(id, addr, val); 
        else response->result = sc.writeByte(id, addr, val);
    }

    void sc_WritePos(
        const std::shared_ptr<custom_msgs::srv::SCWritePos::Request> request,
        std::shared_ptr<custom_msgs::srv::SCWritePos::Response> response) 
    {
        int id = request->id;
        int position = request->position;
        int time = request->time;
        int speed = request->speed;
        response->result = sc.WritePos(id, position, time, speed);
    }

    void sc_PWMMode(
        const std::shared_ptr<custom_msgs::srv::ID::Request> request,
        std::shared_ptr<custom_msgs::srv::ID::Response> response) 
    {
        response->result = sc.PWMMode(request->id);
    }

    void sc_WritePWM(
        const std::shared_ptr<custom_msgs::srv::SCWritePWM::Request> request,
        std::shared_ptr<custom_msgs::srv::SCWritePWM::Response> response) 
    {
        response->result = sc.WritePWM(request->id, request->speed);
    }

    void sc_ReadPos(
        const std::shared_ptr<custom_msgs::srv::ID::Request> request,
        std::shared_ptr<custom_msgs::srv::ID::Response> response) 
    {
        response->result = sc.ReadPos(request->id);
    }

    void st_WritePos(
        const std::shared_ptr<custom_msgs::srv::STWritePos::Request> request,
        std::shared_ptr<custom_msgs::srv::STWritePos::Response> response) 
    {
        int id = request->id;
        int position = request->position;
        int speed = request->speed;
        int acc = request->acc;
        response->result = st.WritePosEx(id, position, speed, acc);
    }

    void st_ReadPos(
        const std::shared_ptr<custom_msgs::srv::ID::Request> request,
        std::shared_ptr<custom_msgs::srv::ID::Response> response) 
    {
        response->result = st.ReadPos(request->id);
    }

    rclcpp::Service<custom_msgs::srv::WriteData>::SharedPtr sc_WriteData_srv;
    rclcpp::Service<custom_msgs::srv::SCWritePos>::SharedPtr sc_WritePos_srv;
    rclcpp::Service<custom_msgs::srv::ID>::SharedPtr sc_PWMMode_srv;
    rclcpp::Service<custom_msgs::srv::SCWritePWM>::SharedPtr sc_WritePWM_srv;
    rclcpp::Service<custom_msgs::srv::ID>::SharedPtr sc_ReadPos_srv;
    rclcpp::Service<custom_msgs::srv::STWritePos>::SharedPtr st_WritePos_srv;
    rclcpp::Service<custom_msgs::srv::ID>::SharedPtr st_ReadPos_srv;
};

int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Serial>());
    rclcpp::shutdown();
    return 0;
}