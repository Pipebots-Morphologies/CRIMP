
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <map>
#include <unistd.h>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "custom_msgs/srv/suction_control.hpp"
#include "custom_msgs/srv/report_pose.hpp"
#include "SCServo.h"

using std::placeholders::_1;
using std::placeholders::_2;
using namespace std::chrono_literals;
using std::string;


class ServoController : public rclcpp::Node
{
public:
  ServoController() : Node("servo_controller"), count_(0)
  {
    const char* serialPort = "/dev/ttyAMA0";
    RCLCPP_INFO(this->get_logger(), "Using serial port: %s", serialPort);

    if (!sc.begin(1000000, serialPort)) {
      RCLCPP_ERROR(this->get_logger(), "Failed to init SCSCL motor!");
    }

    if (!sm_st.begin(1000000, serialPort)) {
      RCLCPP_ERROR(this->get_logger(), "Failed to init SMS_STS motor!");
    }

    joint_pos_pub = this->create_publisher<sensor_msgs::msg::JointState>("joint_pos", 10);
    joint_target_sub = this->create_subscription<sensor_msgs::msg::JointState>(
      "joint_targets", 10, std::bind(&ServoController::sub_callback, this, _1));

    suction_service = this->create_service<custom_msgs::srv::SuctionControl>(
      "suction_control", std::bind(&ServoController::handle_suction_request, this, _1, _2));

    report_pose_service = this->create_service<custom_msgs::srv::ReportPose>(
      "report_pose", std::bind(&ServoController::handle_report_pose, this, _1, _2));

    timer_ = this->create_wall_timer(500ms, std::bind(&ServoController::get_joint_angles, this));

    foot.front = {7, 0};
    foot.rear = {4, 0};
  }


private:
  struct cupParams { int id; int state; };
  struct suctionCupStates { cupParams front; cupParams rear; };
  suctionCupStates foot;
  int home_pos = 500;

  SCSCL sc;
  SMS_STS sm_st;

  std::map<string, double> get_modifiers(const sensor_msgs::msg::JointState & msg){
    get_joint_angles();

    std::map<string, double> modifiers = {
      {"elbow_1", 1}, {"elbow_2", 1}, {"elbow_3", 1}
    };

    std::map<string, double> differences = {
      {"elbow_1", 1}, {"elbow_2", 1}, {"elbow_3", 1}
    };
    float max_dif = 0;

    int msg_length = msg.name.size();
    for(int i = 0; i < msg_length; i++){
      string joint_name = msg.name[i];
      if(joint_name[0] == 'w') continue;
      float dif = msg.position[i]-joint_angles[joint_name];
      if(dif<0) dif = -dif;
      differences[joint_name] = dif;
      if(dif > max_dif) max_dif = dif;
    }

    if(max_dif == 0) {
      return modifiers;
    }
    for( int i = 0; i<msg_length; i++){
      string joint_name = msg.name[i];
      if(joint_name[0] == 'e'){
        modifiers[joint_name]  = differences[joint_name]/max_dif;
      }
    }

    return modifiers;
  }

  void sub_callback(const sensor_msgs::msg::JointState & msg) {
    RCLCPP_INFO(this->get_logger(), "Sub callback");
    
    auto modifiers = get_modifiers(msg);

    RCLCPP_INFO(this->get_logger(), "Sub callback 2");
    
    int v = msg.velocity[0];
    int a = msg.effort[0];

    for (int i = 0; i < msg.name.size(); i++) {
      float target_angle = msg.position[i]; 

      string joint_name = msg.name[i];
      target_angle -= joint_offsets[joint_name];
      if(joint_name == "elbow_1") target_angle = 360 - target_angle;

      requested_angles[joint_name] = target_angle;

      if (joint_name[0] == 'w') {
        wrist_move(joint_ids[joint_name], target_angle, v, 0);
      } else if (joint_name[0] == 'e') {
        elbow_move(joint_ids[joint_name], target_angle, v*modifiers[joint_name], a*modifiers[joint_name]);
      }
    }
  }


  void wrist_move(int id, float position, int velocity, int time = 0) {
    if (position >= 150) position = 150;
    if (position <= 0) position = 0;
    int steps = sc_angle2steps(position);
    if (velocity == 0) { velocity = st_default_vel; }
    sc.WritePos(id, steps, time, velocity);
  }


  void elbow_move(int id, float position, int velocity, int acc) {
    if(id == 2){
      if (position >= 300) position = 300;
      else if (position <= 60) position = 60;
    }
    else{
      if (position >= 255) position = 255;
      else if (position <= 105) position = 105;
    } 
    
    int steps = st_angle2steps(position);
    if (velocity == 0) velocity = st_default_vel; 
    if (acc ==  0 ) acc = 50;
    sm_st.WritePosEx(id, steps, velocity, acc);
  }


  void get_joint_angles() {
    for (const auto& joint_name : joint_names) {
      int id = joint_ids[joint_name];
      float pos;
      if (joint_name[0] == 'w') {
        int steps = sc.ReadPos(id);
        pos = sc_steps2angle(steps);
      } else {
        int steps = sm_st.ReadPos(id);
        pos = st_steps2angle(steps);
        if(joint_name == "elbow_1") pos = 360 - pos;
      }
      pos += joint_offsets[joint_name];
      joint_angles[joint_name] = pos;
    }

    auto msg = sensor_msgs::msg::JointState();
    msg.name = joint_names;
    for (auto& pair : joint_angles) {
      msg.position.push_back(pair.second);
    }
    joint_pos_pub->publish(msg);
  }


  void driveLogic(cupParams &cup, int request) {
    if (request == 1 && cup.state == 0) {
      rotate(cup.id, 1, 6);
      cup.state = 1;
    } else if (request == -1 && cup.state == 1) {
      rotate(cup.id, -1, 8);
      cup.state = -1;
    } else if (request == 0 && cup.state == -1) {
      rotate(cup.id, 1, 2);
      cup.state = 0;
    } else {
      RCLCPP_WARN(this->get_logger(), "Invalid transition for cup id %d", cup.id);
    }
  }


  void rotate(int id, int direction, int turns) { 

    int counter = 0, speed = 800, grad_threshhold = 30, low_grad_counter = 0;
    bool steep_flag = false, last_steep_flag = steep_flag;
    int pos = sc.ReadPos(id), prev_pos = pos;

    sc.PWMMode(id);
    sc.WritePWM(id, speed * direction);

    while (counter < turns) {
      rclcpp::sleep_for(5ms);
      pos = sc.ReadPos(id);
      if (pos == -1) continue;  

      int grad = std::abs(pos - prev_pos);
      RCLCPP_INFO(this->get_logger(), "Grad: %d ", grad);

      if (grad > 1200) continue;
      if (grad > grad_threshhold) {
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


  void centre(int id){

    sc.writeByte(id, 0x30, 0); //unlock eprom
    sc.writeWord(id, 0x09, 20); //set lower angle limit
    sc.writeWord(id, 0x0B, 1003); //set upper angle limit
    sc.writeByte(id, 0x30, 0); // lock eprom

    // go to home position
    sc.WritePos(id, home_pos, 0, 800);
    usleep(1000 * 1000);
  }


  void handle_suction_request(
    const std::shared_ptr<custom_msgs::srv::SuctionControl::Request> request,
    std::shared_ptr<custom_msgs::srv::SuctionControl::Response> response)
  {
    driveLogic(foot.front, request->cup_1);
    driveLogic(foot.rear, request->cup_2);
    response->success = true;
  }


  void forward_kinematics(string base, float t1, float t2, float t3, float& x, float& y, float& theta) {
    float l1 = 122, l2 = 152, l3 = 152, l4 = 122; // Replace with your real link lengths (meters)
    float rad1, rad2, rad3;
    rad2 = t2 * M_PI / 180.0f;

    if(base == "rear"){
      rad1 = t1 * M_PI / 180.0f;
      rad3 = t3 * M_PI / 180.0f;
    }

    else if (base == "front") {
      rad1 = t3 * M_PI / 180.0f;
      rad3 = t1 * M_PI / 180.0f;
    }

    else{
      RCLCPP_WARN(this->get_logger(), "Unknown base '%s'. Defaulting to 'front'.", base.c_str());
      rad1 = t3 * M_PI / 180.0f;
      rad3 = t1 * M_PI / 180.0f;
    }
  
    float total_angle = rad1 + rad2 + rad3;
  
    x = l2 * sin(rad1) - l3 * sin(rad1 + rad2) + l4 * sin(total_angle);
    y = -l1 + l2 * cos(rad1) - l3 * cos(rad1 + rad2) + l4 * cos(total_angle);

    theta = 360 - total_angle * 180.0 / M_PI; // Orientation of the end effector
  }  


  void handle_report_pose(
    const std::shared_ptr<custom_msgs::srv::ReportPose::Request> request,
    std::shared_ptr<custom_msgs::srv::ReportPose::Response> response)
  {
    get_joint_angles(); // Updates joint_angles map
    string base = request->base;
  
    float t1 = joint_angles["elbow_1"];
    float t2 = joint_angles["elbow_2"];
    float t3 = joint_angles["elbow_3"];
    float x, y, theta;
  
    forward_kinematics(base, t1, t2, t3, x, y, theta);
  
    response->elbow_1 = t1;
    response->elbow_2 = t2;
    response->elbow_3 = t3;
    response->x = x;
    response->y = y;
    response->theta = theta;
  
    RCLCPP_INFO(this->get_logger(), "[%s base] \n Current Joint Angles: [%.2f, %.2f, %.2f] , \n Current pose X: %.2f, Y: %.2f, Theta: %.2f deg", base.c_str(), t1, t2, t3, x, y, theta);
  }
  

  float sc_steps2angle(int steps) { return (steps / 1023.0f) * 150.0f; }
  int sc_angle2steps(float angle) { return (angle / 150.0f) * 1023.0f; }
  float st_steps2angle(int steps) { return (steps / 4095.0f) * 360.0f; }
  int st_angle2steps(float angle) { return (angle / 360.0f) * 4095.0f; }


  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pos_pub;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_target_sub;
  rclcpp::Service<custom_msgs::srv::SuctionControl>::SharedPtr suction_service;
  rclcpp::Service<custom_msgs::srv::ReportPose>::SharedPtr report_pose_service;


  std::vector<string> joint_names = {"elbow_1", "elbow_2", "elbow_3", "wrist_1", "wrist_2"};
  std::map<string, int> joint_ids = {
    {"elbow_1", 3}, {"elbow_2", 2}, {"elbow_3", 1},
    {"wrist_1", 6}, {"wrist_2", 5}
  };
  std::map<string, float> joint_angles = {
    {"elbow_1", 180}, {"elbow_2", 180}, {"elbow_3", 180},
    {"wrist_1", 75}, {"wrist_2", 75}
  };
  std::map<string, float> requested_angles = {
    {"elbow_1", 180}, {"elbow_2", 180}, {"elbow_3", 180},
    {"wrist_1", 75}, {"wrist_2", 75}
  };
  std::map<string, float> joint_offsets = {
    {"elbow_1", 2}, {"elbow_2", 0.5}, {"elbow_3", 2},
    {"wrist_1", 0}, {"wrist_2", 0}
  };

  int sc_default_vel = 400;
  int st_default_vel = 1200;
  size_t count_;
};


int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ServoController>());
  rclcpp::shutdown();
  return 0;
}
