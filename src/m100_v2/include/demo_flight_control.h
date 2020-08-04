/** @file demo_flight_control.h
 *  @version 3.3
 *  @date May, 2017
 *
 *  @brief
 *  demo sample of how to use flight control APIs
 *
 *  @copyright 2017 DJI. All rights reserved.
 *
 */

#ifndef DEMO_FLIGHT_CONTROL_H
#define DEMO_FLIGHT_CONTROL_H

// ROS includes
#include <ros/ros.h>
#include <geometry_msgs/QuaternionStamped.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <sensor_msgs/NavSatFix.h>
#include <std_msgs/UInt8.h>

// DJI SDK includes
#include <dji_sdk/DroneTaskControl.h>
#include <dji_sdk/SDKControlAuthority.h>
#include <dji_sdk/QueryDroneVersion.h>

#include <tf/tf.h>
#include <sensor_msgs/Joy.h>


#include <uavmessage.pb.h>
#include <uavmessage.pb.cc>
#include <gcscommand.pb.h>
#include <gcscommand.pb.cc>
#include <taskmessage.pb.h>
#include <taskmessage.pb.cc>
#include <feedbackdata.pb.h>
#include <feedbackdata.pb.cc>
#include <m100_v2/Position.h>
#include <m100_v2/PubSubUavMsg.h>
#include <m100_v2/ObstacleMsg.h>
#include <errno.h>
#include <stdio.h>
#include <fstream>
#include <zhelpers.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sensor_msgs/Imu.h>

using namespace std;

#define C_EARTH (double)6378137.0
#define C_PI (double)3.141592653589793
#define DEG2RAD(DEG) ((DEG) * ((C_PI) / (180.0)))

/*!
 * @brief a bare bone state machine to track the stage of the mission
 */
class Mission
{
public:
  // The basic state transition flow is:
  // 0---> 1 ---> 2 ---> ... ---> N ---> 0
  // where state 0 means the mission is note started
  // and each state i is for the process of moving to a target point.
  int state=0;

  int inbound_counter=0;
  int outbound_counter;
  int break_counter=0;

  float target_offset_x=0;
  float target_offset_y=0;
  float target_offset_z=0;
  float target_yaw=0;
  //sensor_msgs::NavSatFix start_gps_location;
  m100_v2::Position start_pos_location;

  bool start = false;//任务开始
  bool finished = true;//完成一个航点

  Mission() : state(0), inbound_counter(0), outbound_counter(0), break_counter(0),
              target_offset_x(0.0), target_offset_y(0.0), target_offset_z(0.0),
              finished(true)
  {
  }

  void step();

  void setTarget(float x, float y, float z, float yaw)
  {
    target_offset_x = x;
    target_offset_y = y;
    target_offset_z = z;
    target_yaw      = yaw;
  }

  void reset()
  {
    inbound_counter = 0;
    outbound_counter = 0;
    break_counter = 0;
    finished = true;
  }

};

void localOffsetFromGpsOffset(geometry_msgs::Vector3&  deltaNed,
                         sensor_msgs::NavSatFix& target,
                         sensor_msgs::NavSatFix& origin);

geometry_msgs::Vector3 toEulerAngle(geometry_msgs::Quaternion quat);

bool try_to_take_action(uint8_t requestTask);
void takeoff_cmd_callback(const m100_v2::PubSubUavMsg cmd_str);//起飞
void land_cmd_callback(const m100_v2::PubSubUavMsg cmd_str);//降落
void gohome_cmd_callback(const m100_v2::PubSubUavMsg cmd_str);//返航
void move_cmd_callback(const m100_v2::PubSubUavMsg cmd_str);//订阅移动控制指令
void waypoint_cmd_callback(const m100_v2::PubSubUavMsg cmd_str);//订阅航点控制指令

void attitude_callback(const geometry_msgs::Vector3 attitude);
void gps_callback(const sensor_msgs::NavSatFix msg);
void position_callback(const m100_v2::Position pos);
void velocity_callback(const geometry_msgs::Vector3Stamped& vel);//速度
void obstacle_callback(const m100_v2::ObstacleMsg& obst);
void flight_status_callback(const std_msgs::UInt8::ConstPtr& msg);

void local_position_callback(const geometry_msgs::PointStamped::ConstPtr& msg);

void transformWP(myTaskMessage::MyPoint* mp);//航点坐标变换

bool obtain_control();

bool is_M100();


#endif // DEMO_FLIGHT_CONTROL_H
