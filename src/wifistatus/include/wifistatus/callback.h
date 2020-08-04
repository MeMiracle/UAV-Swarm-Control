#ifndef __CALLBACK_H
#define __CALLBACK_H

#include <sys/time.h>
#include <fstream>

#include <ros/ros.h>
#include <geometry_msgs/Vector3Stamped.h>

#include "wifistatus/Position.h"
#include "wifistatus/CommInfo.h"

#define TO_STRING std::to_string

/*
 * This header file defines Callback function for handlering ROS_subscribe Messages
 */

void attitude_callback(geometry_msgs::Vector3 msg_attit);
void position_callback(wifistatus::Position msg_pos);
void velocity_callback(geometry_msgs::Vector3Stamped msg_velocity);

void sendmsg_callback(wifistatus::CommInfo comm_send_info);
void recvmsg_callback(wifistatus::CommInfo comm_recv_info);

static int save_wifi_uav_status(bool is_send);
static std::string getDirectory();

#endif