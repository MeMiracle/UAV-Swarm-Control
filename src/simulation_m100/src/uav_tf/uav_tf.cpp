#include <ros/ros.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <zconf.h>
#include <zmq.hpp>
#include <stdlib.h>
#include <zhelpers.hpp>
#include <sensor_msgs/NavSatFix.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <m100Location/uwblps.h>
#include <m100Location/uwblpsmessage.pb.h>
#include <m100Location/uwblpsmessage.pb.cc>
#include <simulation_m100/PubSubUavMsg.h>
#include <simulation_m100/Position.h>
#include <string>


#include <tf/transform_broadcaster.h>


using namespace std;


string uav_name("");

sensor_msgs::NavSatFix current_gps;//当前gps
sensor_msgs::NavSatFix home_gps;//home点

ros::Publisher local_pos_pub;
ros::Publisher global_pos_pub;

#define C_PI (double)3.141592653589793
#define C_EARTH (double)6378137.0
const float deg2rad = C_PI/180.0;
const float rad2deg = 180.0/C_PI;

//实验场地坐标系与自然全局坐标系的转换关系：
//实验场地坐标系在自然全局坐标系原点以东为正x、以北为正y、北偏东夹角为负（度）
double axisOffsetX = 0;
double axisOffsetY = 0;
double axisOffsetZ = 0;
double axisOffsetAngle = 0;
double sinOffsetAngle = 0;
double cosOffsetAngle = 1;
/*! Very simple calculation of local NED offset between two pairs of GPS
/coordinates. Accurate when distances are small.
!*/
//根据两对gps坐标换算出距离坐标（针对gazebo的经度和纬度转置做了计算调整）
void localOffsetFromGpsOffset(geometry_msgs::Vector3&  deltaNed,
                         sensor_msgs::NavSatFix& target,
                         sensor_msgs::NavSatFix& origin)
{
  double deltaLon = target.longitude - origin.longitude;
  double deltaLat = target.latitude - origin.latitude;

  deltaNed.x = deltaLon * deg2rad * C_EARTH;
  deltaNed.y = -deltaLat * deg2rad * C_EARTH * cos(deg2rad*target.longitude);
  deltaNed.z = target.altitude - origin.altitude;
}

void gps_callback(const sensor_msgs::NavSatFix::ConstPtr& msg)
{
    current_gps = *msg;//按回调频率更新数据

    if(home_gps.longitude < 0 && home_gps.latitude < 0 && home_gps.altitude < 0)
    {   
        //如果没有手动设置home点，则在开机初始化时获取当前位置作为家的位置
        home_gps = current_gps;
    }
    //
    static tf::TransformBroadcaster global_br;
    static tf::TransformBroadcaster local_br;

    tf::Transform global_transform;
    tf::Transform local_transform;

    global_transform.setOrigin(tf::Vector3(axisOffsetX, axisOffsetY, axisOffsetZ));
    global_transform.setRotation(tf::Quaternion(0,0,0,1));

    global_br.sendTransform(tf::StampedTransform(global_transform, ros::Time::now(), "world", "_home_point"));

    //gps经纬度转换为全局距离坐标
    geometry_msgs::Vector3 gps_point;
    localOffsetFromGpsOffset(gps_point, current_gps, home_gps);

    local_transform.setOrigin(tf::Vector3(gps_point.x, gps_point.y, gps_point.z));
    local_transform.setRotation(tf::Quaternion(0,0,0,1));

    local_br.sendTransform(tf::StampedTransform(local_transform, ros::Time::now(), "_home_point", "_local_point"));

/*
    static ros::Time count_time = ros::Time::now();
    // if(ros::Time::now() - count_time >= ros::Duration(0.02))
    // {
        if(gps_str)
        {
            gps_str << ros::Time::now() << " " << current_gps.longitude << " " << current_gps.latitude << " " << current_gps.altitude << endl;      
        }


        if(dist_str)
        {
            dist_str << ros::Time::now() << " " << local_pos.x << " " << local_pos.y << " " << local_pos.z << endl;                  
        }

    //     count_time = ros::Time::now();
    // }
*/
    
}


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "uav_tf");
    ros::NodeHandle nh;

    ROS_INFO("uav_tf started!");

    home_gps.longitude = -1;
    home_gps.latitude = -1;
    home_gps.altitude = -1;

    ros::param::get("~longitude",home_gps.longitude);
    ros::param::get("~latitude",home_gps.latitude);
    ros::param::get("~altitude",home_gps.altitude);

    ros::param::get("~offset_x",axisOffsetX);
    ros::param::get("~offset_y",axisOffsetY);
    ros::param::get("~offset_z",axisOffsetZ);
    ros::param::get("~offset_angle",axisOffsetAngle);

    //转换为：北偏西+0～+180，北偏东-0～~180
    if(abs(axisOffsetAngle) > 180)
    {
        int n = abs(axisOffsetAngle) / 360 + 1;
        n = axisOffsetAngle>0?-n:n;
        axisOffsetAngle = axisOffsetAngle + 360 * n;//旋转角度控制
    }

    sinOffsetAngle = -sin(axisOffsetAngle*deg2rad);
    cosOffsetAngle = cos(axisOffsetAngle*deg2rad);

    ros::param::get("~uav_name",uav_name);

    ros::Subscriber gps_sub = nh.subscribe(string(uav_name + "/gps").c_str(), 10, &gps_callback);

    ros::spin();

    return 0;
}