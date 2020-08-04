//
// Created by stone on 18-6-20.
//
#include <m100Formation/Spline.h>
#include <ros/ros.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <random>
#include <stack>
#include <stdlib.h>
#include <simulation_m100/Position.h>
#include <simulation_m100/PubSubUavMsg.h>
#include <gcscommand.pb.h>
#include <gcscommand.pb.cc>
#include <taskmessage.pb.h>
#include <taskmessage.pb.cc>
#include <mapbuffer.pb.h>
#include <mapbuffer.pb.cc>
#include <m100Location/uwblpsmessage.pb.h>
#include <m100Location/uwblpsmessage.pb.cc>
#include <m100Formation/formationmessage.pb.h>
#include <m100Formation/formationmessage.pb.cc>
#include <simulation_m100/CmdResult.h>


#define SPEED (float)0.4//速度
#define STEP_TIME 0.4//一次步进时间
#define SENSE 0.03
#define VEHICLE_NUMBER 2
#define MAPXISHU 0.5 //相对于10*10米而言的地图系数
#define MAXLENGTH 5
#define MAPLENGTH 20
#define DIAMETER 0.6
#define PATHNUMBER 50 //通过三次B样条插补出来的路径点的个数
#define macro 100

int uavID = 1;
string uavName("uav1");
int axisID = 1;
int moveCtrlMode = 1;//速度控制模式：0为机身速度控制，1为自然全局坐标系内速度控制，2为实验局部坐标系内速度控制
bool cmd_status = true;//命令执行状态反馈

string other_uav_id_config("simulation_m100/configure/other_uav_id_config_");
int other_uav_id[100];
int other_uav_count=0;

Target myFormationTarget;//编队形成时的目标点（通常只有一个，用于本机参与形成队形）
vector<Target> thePathTargets;//编队形成后的运动路径目标点序列（一个或多个）
vector<Point3D> swarmPositionBuf;
vector<int> uavIndexBuf;
vector<ros::Time> posUpdateTime;
pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER;

ros::Publisher formation_info_pub;
ros::Publisher move_cmd_pub;
string move_cmd = "move_cmd";
float Formation_expansion_factor =  1;//
const float Unmanned_system_diameter = 1;
const float Deviation = 0.6;
const float runtime = 0.2;
const float runspeed = 0.4;
bool ready = false;

typedef map<int,FormationPoint> formationType;

int leaderid = 1;
const float Radius = 0.6;//uav's Radius (mile)


uav_zyl uav(0, 0, 0, uavID);;
uav_zyl vl(0.0,0.0,0.0,100);
int movecount = 0;
map<int,Point3D> swarmlocation;


formationType formationPoints;//用于记录编队控制点信息
behavoir myBehavior;
vector<int> carids;//存放集群内部无人机的ID号码，第一个为本机，后面的为其他无人系统的ID号码
bool formationover = false;
bool flag1 = false;
bool flag2 = false;
bool flag3 = false;


string component = "m100Formation";
int swarm_intel_priority = 94;

void uav_zyl::moveTowardDir(direction adjDir, int moveCtrlMode,float coefficient )
{
    gcsCommand::Twist twist;
    twist.Clear();

    switch (adjDir)
    {
    case (direction)0://left
        twist.set_linear_x(-speed*coefficient);
        break;
    case (direction)1://left_up
        twist.set_linear_x(-x_speed*coefficient);
        twist.set_linear_y(y_speed*coefficient);
        break;
    case (direction)2://up
        twist.set_linear_y(speed*coefficient);
        break;
    case (direction)3://right_up
        twist.set_linear_x(x_speed*coefficient);
        twist.set_linear_y(y_speed*coefficient);
        break;
    case (direction)4://right
        twist.set_linear_x(speed*coefficient*coefficient);
        break;
    case (direction)5://right_down
        twist.set_linear_x(x_speed*coefficient);
        twist.set_linear_y(-y_speed*coefficient);
        break;
    case (direction)6://down
        twist.set_linear_y(-speed*coefficient);
        break;
    case (direction)7://left_down
        twist.set_linear_x(-x_speed*coefficient);
        twist.set_linear_y(-y_speed*coefficient);
        break;
    case (direction)9://rise
		twist.set_linear_z(z_speed);
		break;
	case (direction)10://descend
		twist.set_linear_z(-z_speed);
		break;
    default:
        break;
    }

    twist.set_angular_x(moveCtrlMode);

    simulation_m100::PubSubUavMsg move_cmd_msg;
    if (!twist.SerializeToString(&move_cmd_msg.playload))
    {
        perror("serialize error:");
    }

    else
    {
        move_cmd_msg.component = component;
        move_cmd_msg.cmd_priority = swarm_intel_priority;
        move_cmd_msg.start_time = ros::Time::now().toSec();
        move_cmd_pub.publish(move_cmd_msg);

        ros::Time start_time = ros::Time::now();
        while (ros::ok() && true == cmd_status)
        {
            ros::Duration(0.1).sleep();
            if (ros::Time::now() - start_time > ros::Duration(STEP_TIME))
            {
                break;
            }
        }
    }
}


//获取集群其他无人系统位置
void getSwarmPosition(std::vector<Point3D> &swarmPosition)
{
	pthread_mutex_lock(&buf_mutex);
	swarmPosition = swarmPositionBuf;
	pthread_mutex_unlock(&buf_mutex);
}

bool isApproimate(Point3D p1, Point3D p2, float VEHICLE_RADIUS)
{
    if ((p1.x - p2.x) <= VEHICLE_RADIUS && (p1.y - p2.y) <= VEHICLE_RADIUS)
    {
        return ((p1.x - p2.x) >= -VEHICLE_RADIUS) && ((p1.y - p2.y) >= -VEHICLE_RADIUS);
    }
    else
        return false;
}

int JudgeEdge(float rx1, float rx2, float ry1, float ry2, Point3D point, int clock, float VEHICLE_RADIUS)//
{

    Point3D Bedge;
    Bedge.z = point.z;

    //×ó±ß
    if ((point.y <= ry2*MAXLENGTH) && (point.y >= ry1*MAXLENGTH))
    {
        //if (clock == 2)
        //	clock = 1;
        //else if (clock == 1)
        //	clock = 2;
        Bedge.x = rx1*MAXLENGTH;
        Bedge.y = point.y;
        if (abs(Bedge - point) <= VEHICLE_RADIUS)
            return clock;
        //ÓÒ±ß
        Bedge.x = rx2*MAXLENGTH;
        if (abs(Bedge - point) <= VEHICLE_RADIUS)
            return clock;

    }
    //ÏÂ±ß
    if ((point.x <= rx2*MAXLENGTH) && (point.x >= rx1*MAXLENGTH))
    {
        Bedge.y = ry1*MAXLENGTH;
        Bedge.x = point.x;
        if (abs(Bedge - point) <= VEHICLE_RADIUS)
            return clock;
        //ÉÏ±ß
        Bedge.y = ry2*MAXLENGTH;
        if (abs(Bedge - point) <= VEHICLE_RADIUS)
            return clock;
    }

    //ËÄ½Ç
    Bedge.x = rx1*MAXLENGTH;
    Bedge.y = ry1*MAXLENGTH;
    
    if (abs(Bedge - point) <= VEHICLE_RADIUS) 
    {
        //if (clock == 1)
        //	clock = 2;
        //else if (clock == 2)
        //	clock = 1;
        return clock;
    }

    Bedge.x = rx1*MAXLENGTH;
    Bedge.y = ry2*MAXLENGTH;
    if (abs(Bedge - point) <= VEHICLE_RADIUS)
        return clock;
    Bedge.x = rx2*MAXLENGTH;
    Bedge.y = ry1*MAXLENGTH;

    if (abs(Bedge - point) <= VEHICLE_RADIUS)
        return clock;
    Bedge.x = rx2*MAXLENGTH;
    Bedge.y = ry2*MAXLENGTH;

    if (abs(Bedge - point) <= VEHICLE_RADIUS)
        return clock;
    return 0;
}

int JudgeBarrier(Point3D point, float VEHICLE_RADIUS)
{
    return 0;

    //µÚÒ»¸öÕÏ°­Îï
    if (JudgeEdge(0.8, 0.9, 0.2, 0.3, point, 1, VEHICLE_RADIUS))
        return JudgeEdge(0.8, 0.9, 0.2, 0.3, point, 1, VEHICLE_RADIUS);

    //µÚ¶þ¸öÕÏ°­Îï
    if (JudgeEdge(0.6, 0.7, 0.4, 0.5, point, 1, VEHICLE_RADIUS))
        return JudgeEdge(0.6, 0.7, 0.4, 0.5, point, 1, VEHICLE_RADIUS);

    //µÚÈý¸öÕÏ°­Îï
    if (JudgeEdge(0.2, 0.6, 0.65, 0.75, point, 1, VEHICLE_RADIUS))
        return JudgeEdge(0.2, 0.6, 0.65, 0.75, point, 1, VEHICLE_RADIUS);

    //µÚËÄ¸öÕÏ°­Îï
    if (JudgeEdge(0.2, 0.6, 0.9, 1, point, 2, VEHICLE_RADIUS))
        return JudgeEdge(0.2, 0.6, 0.9, 1, point, 2, VEHICLE_RADIUS);

    return false;
}


direction calculateDirection(Point3D targetPosition, Point3D nowPosition, float VEHICLE_RADIUS)
{
    // if (targetPosition.z - nowPosition.z >= 1.5*VEHICLE_RADIUS)
	// {
	// 	return direction::rise;
	// }
	// if (targetPosition.z - nowPosition.z <= 1.5*VEHICLE_RADIUS)
	// {
	// 	return direction::descend;
	// }
    if (targetPosition.x - nowPosition.x >= VEHICLE_RADIUS)
    {
        if (targetPosition.y - nowPosition.y >= VEHICLE_RADIUS)
            return right_up;
        else if (targetPosition.y - nowPosition.y <= -VEHICLE_RADIUS)
            return right_down;
        else
            return direction::right;
    }
    else if (targetPosition.x - nowPosition.x <= -VEHICLE_RADIUS)
    {
        if (targetPosition.y - nowPosition.y >= VEHICLE_RADIUS)
            return left_up;
        else if (targetPosition.y - nowPosition.y <= -VEHICLE_RADIUS)
            return left_down;
        else return direction::left;
    }
    else if (targetPosition.y - nowPosition.y >= VEHICLE_RADIUS)
        return up;
    else if (targetPosition.y - nowPosition.y <= -VEHICLE_RADIUS)
        return down;
    else return direction(int(7 * (rand() / (RAND_MAX + 1.0))));
}

int calp2pdir(Point3D nowPosition, Point3D tarPosition)
/*

返回值：上，右，下，左分别对应1,2,3,4
首先计算90度和270度的方向
然后计算0度和180度的方向

*/
{
	int ret = 0;
	float tx = tarPosition.x;
	float ty = tarPosition.y;
	float nx = nowPosition.x;
	float ny = nowPosition.y;

	if (abs(abs(tx) - abs(nx)) <= 0.5)
	{
		if (ty > ny)
		{
			ret = 1;
			cout << "up" << endl;
		}
		else
		{
			ret = 3;
			cout << "down" << endl;
		}
	}
	if(abs(abs(ty) - abs(ny)) < 0.5)
	{
		if (tx > nx)
		{
			ret = 2;
			cout << "right" << endl;
		}
		else
		{
			ret = 4;
			cout << "left" << endl;
		}
	}
	if (ret == 0)
	{
		float a = ty - ny;
		float b = tx - nx;
		float c = sqrt(a*a + b*b);
		//cout << "a:" << a << ",b:" << b << ",c:" << c << endl;
		float cosangle = 0.0;
		cosangle = ((b*b + c*c - a*a) / (2 * b*c));
		auto arccos = acos(cosangle);
		auto jiaodu = 180 * arccos / 3.141592653;
		//cout << cosangle << endl;
		//cout << arccos << endl;
		//cout << jiaodu << endl;
		if (ty > ny)
		{
			if (jiaodu >= 0 && jiaodu < 45)
				ret = 2;
			else if (jiaodu >= 45 && jiaodu < 135)
				ret = 1;
			else if (jiaodu >= 135 && jiaodu < 180)
				ret = 4;
		}
		else
		{
			if (jiaodu >= 135 && jiaodu < 180)
				ret = 4;
			else if (jiaodu >= 45 && jiaodu < 135)
				ret = 3;
			else if (jiaodu >= 0 && jiaodu < 45)
				ret = 2;
		}
	}

	return ret;
}

formationType calFormationPoint(vector<int> ids,Point3D formationCenter,int formationnumber = 0,int movedir = 1,float Formation_expansion_factor = 1)
/*
该方法只会由队长无人机所调用
设计队形库 用nums标识无人系统的数目选定队形，然后根据formationnumber 选择队形编号

*/
{
    int nums = ids.size();
    map<int ,FormationPoint> res;
    ROS_INFO("num of uav is %d",nums);
    if(nums == 0)
    {
        ROS_INFO("Error number of uavs");
        exit(0);
    }
    if(nums == 1)
    {
        res.insert(make_pair(ids[0],FormationPoint(leaderid,Point3D(formationCenter.x,static_cast<float>(formationCenter.y),formationCenter.z))));
    }
    if(nums ==2)
    {

        switch(formationnumber)
        {
            case 0:
            {
                res.insert(make_pair(ids[0],FormationPoint(leaderid,Point3D(formationCenter.x,static_cast<float>(formationCenter.y),formationCenter.z))));
                res.insert(make_pair(ids[0],FormationPoint(other_uav_id[0],Point3D(formationCenter.x,static_cast<float>(formationCenter.y+2*Formation_expansion_factor),formationCenter.z))));
                break;
            }
            case 1:
            {
                res.insert(make_pair(ids[0],FormationPoint(leaderid,Point3D(formationCenter.x,static_cast<float>(formationCenter.y),formationCenter.z))));
                res.insert(make_pair(ids[0],FormationPoint(other_uav_id[0],Point3D(formationCenter.x+2*Formation_expansion_factor,static_cast<float>(formationCenter.y),formationCenter.z))));
                break;
            }
            default:
                break;
        }
    }
    if(nums == 3)
    {
        
        switch(formationnumber)
        {
            case 0:
            /*
            队形1 三角形队形
            */
            {
                switch(movedir)
                {
                    case 1:
                    {
                        ROS_INFO("formationdir is %d",movedir);
                        res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                            static_cast<float>(formationCenter.y), formationCenter.z))));
                        res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x - 2*Formation_expansion_factor),
                                                                            static_cast<float>(formationCenter.y - 2*Formation_expansion_factor), formationCenter.z))));
                        res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x + 2*Formation_expansion_factor),
                                                                        static_cast<float>(formationCenter.y - 2*Formation_expansion_factor), formationCenter.z))));
                        break;
                    }
                    case 2:
                    {
                        ROS_INFO("formationdir is %d",movedir);
                        res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                            static_cast<float>(formationCenter.y), formationCenter.z))));
                        res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x - 2*Formation_expansion_factor),
                                                                            static_cast<float>(formationCenter.y + 2*Formation_expansion_factor), formationCenter.z))));
                        res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x - 2*Formation_expansion_factor),
                                                                        static_cast<float>(formationCenter.y - 2*Formation_expansion_factor), formationCenter.z))));
                        break;
                    }
                    case 3:
                    {
                        ROS_INFO("formationdir is %d",movedir);
                        res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                            static_cast<float>(formationCenter.y), formationCenter.z))));
                        res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x + 2*Formation_expansion_factor),
                                                                            static_cast<float>(formationCenter.y + 2*Formation_expansion_factor), formationCenter.z))));
                        res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x - 2*Formation_expansion_factor),
                                                                        static_cast<float>(formationCenter.y + 2*Formation_expansion_factor), formationCenter.z))));
                        break;

                    }
                    case 4:
                    {
                        ROS_INFO("formationdir is %d",movedir);
                        res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                            static_cast<float>(formationCenter.y), formationCenter.z))));
                        res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x + 2*Formation_expansion_factor),
                                                                            static_cast<float>(formationCenter.y - 2*Formation_expansion_factor), formationCenter.z))));
                        res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x + 2*Formation_expansion_factor),
                                                                        static_cast<float>(formationCenter.y + 2*Formation_expansion_factor), formationCenter.z))));
                        break;
                    }
                }
                break;

            }
            case 1:
            /*
            队形2 一字型队形
            */
            {
                switch(movedir)
                {
                    case 1:
                    {
                        ROS_INFO("formationdir is %d",movedir);
                        res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                            static_cast<float>(formationCenter.y), formationCenter.z))));
                        res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x),
                                                                            static_cast<float>(formationCenter.y - 3*Formation_expansion_factor), formationCenter.z))));
                        res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x),
                                                                        static_cast<float>(formationCenter.y - 6*Formation_expansion_factor), formationCenter.z))));
                        break;
                    }
                    case 2:
                    {
                        ROS_INFO("formationdir is %d",movedir);
                        res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x ,
                                                                            static_cast<float>(formationCenter.y), formationCenter.z))));
                        res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x- 3*Formation_expansion_factor),
                                                                            static_cast<float>(formationCenter.y), formationCenter.z))));
                        res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x - 6*Formation_expansion_factor),
                                                                        static_cast<float>(formationCenter.y), formationCenter.z))));
                        break;
                    }
                    case 3:
                    {
                        ROS_INFO("formationdir is %d",movedir);
                        res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                            static_cast<float>(formationCenter.y), formationCenter.z))));
                        res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x),
                                                                            static_cast<float>(formationCenter.y + 3*Formation_expansion_factor), formationCenter.z))));
                        res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x),
                                                                        static_cast<float>(formationCenter.y + 6*Formation_expansion_factor), formationCenter.z))));
                        break;
                    }
                    case 4:
                    {
                        ROS_INFO("formationdir is %d",movedir);
                        res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                            static_cast<float>(formationCenter.y), formationCenter.z))));
                        res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x + 3*Formation_expansion_factor),
                                                                            static_cast<float>(formationCenter.y ), formationCenter.z))));
                        res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x + 6*Formation_expansion_factor),
                                                                        static_cast<float>(formationCenter.y ), formationCenter.z))));
                        break;
                    }
                    
                }
                break;
            }
            case 2:
            /*
            队形3 竖直直线形
            */
            {
                res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                      static_cast<float>(formationCenter.y), formationCenter.z))));
                res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x ),
                                                                      static_cast<float>(formationCenter.y - 4*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x ),
                                                                 static_cast<float>(formationCenter.y - 2*Formation_expansion_factor), formationCenter.z))));
            }
            default:
            {
                 break;
            }
        }
    }

    if(nums == 4 )
    {
        switch(formationnumber)
        {
            case 0:
            /*hao
            队形1:菱形队形
            */
            {
                res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                      static_cast<float>(formationCenter.y), formationCenter.z))));
                res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x + 2*Formation_expansion_factor),
                                                                      static_cast<float>(formationCenter.y - 2.5*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x - 2*Formation_expansion_factor),
                                                                 static_cast<float>(formationCenter.y - 2.5*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[3],FormationPoint(other_uav_id[2],Point3D(static_cast<float>(formationCenter.x),
                                                                 static_cast<float>(formationCenter.y - 5*Formation_expansion_factor), formationCenter.z))));
                break;

            }
            case 1:
            /*
            竖直队形
            */
            {
                res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                      static_cast<float>(formationCenter.y), formationCenter.z))));
                res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x ),
                                                                      static_cast<float>(formationCenter.y - 2*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x ),
                                                                 static_cast<float>(formationCenter.y - 4*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[3],FormationPoint(other_uav_id[2],Point3D(static_cast<float>(formationCenter.x),
                                                                 static_cast<float>(formationCenter.y - 6*Formation_expansion_factor), formationCenter.z))));
                break;

            }
            case 2:
            /*
            水平队形
            */
            {
                res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                      static_cast<float>(formationCenter.y), formationCenter.z))));
                res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x + 2*Formation_expansion_factor),
                                                                      static_cast<float>(formationCenter.y ), formationCenter.z))));
                res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x + 4*Formation_expansion_factor),
                                                                 static_cast<float>(formationCenter.y ), formationCenter.z))));
                res.insert(make_pair(ids[3],FormationPoint(other_uav_id[2],Point3D(static_cast<float>(formationCenter.x + 6*Formation_expansion_factor),
                                                                 static_cast<float>(formationCenter.y ), formationCenter.z))));
                break;
            }
            default:
            {
                break;
            }
        }
    }

    if(nums == 5)
    {
        switch(formationnumber)
        {
            case 0:
            /*
            队形1:扩散三角队形
            */
            {
                res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                      static_cast<float>(formationCenter.y), formationCenter.z))));
                res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x + 2*Formation_expansion_factor ),
                                                                      static_cast<float>(formationCenter.y - 2*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x - 2*Formation_expansion_factor),
                                                                 static_cast<float>(formationCenter.y - 2*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[3],FormationPoint(other_uav_id[2],Point3D(static_cast<float>(formationCenter.x + 4*Formation_expansion_factor),
                                                                 static_cast<float>(formationCenter.y - 4*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[4],FormationPoint(other_uav_id[3],Point3D(static_cast<float>(formationCenter.x-4*Formation_expansion_factor),
                                                                static_cast<float>(formationCenter.y -4*Formation_expansion_factor),formationCenter.z))));

                break;

            }
            case 1:
            /*
            竖直队形
            */
            {
                res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                      static_cast<float>(formationCenter.y), formationCenter.z))));
                res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x  ),
                                                                      static_cast<float>(formationCenter.y - 2*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x ),
                                                                 static_cast<float>(formationCenter.y + 2*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[3],FormationPoint(other_uav_id[2],Point3D(static_cast<float>(formationCenter.x),
                                                                 static_cast<float>(formationCenter.y - 4*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[4],FormationPoint(other_uav_id[3],Point3D(static_cast<float>(formationCenter.x),
                                                                static_cast<float>(formationCenter.y + 4*Formation_expansion_factor),formationCenter.z))));
                break;

            }
            case 2:
            /*`
            水平队形
            */
            {
                res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                      static_cast<float>(formationCenter.y), formationCenter.z))));
                res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x  ),
                                                                      static_cast<float>(formationCenter.y - 2*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x ),
                                                                 static_cast<float>(formationCenter.y + 2*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[3],FormationPoint(other_uav_id[2],Point3D(static_cast<float>(formationCenter.x),
                                                                 static_cast<float>(formationCenter.y - 4*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[4],FormationPoint(other_uav_id[3],Point3D(static_cast<float>(formationCenter.x),
                                                                static_cast<float>(formationCenter.y + 4*Formation_expansion_factor),formationCenter.z))));
                break;
            }
            case 3:
            /*
            星型队形
            */
            {
                res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
                                                                      static_cast<float>(formationCenter.y), formationCenter.z))));
                res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x  ),
                                                                      static_cast<float>(formationCenter.y - 2*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x ),
                                                                 static_cast<float>(formationCenter.y + 2*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[3],FormationPoint(other_uav_id[2],Point3D(static_cast<float>(formationCenter.x),
                                                                 static_cast<float>(formationCenter.y - 4*Formation_expansion_factor), formationCenter.z))));
                res.insert(make_pair(ids[4],FormationPoint(other_uav_id[3],Point3D(static_cast<float>(formationCenter.x),
                                                                static_cast<float>(formationCenter.y + 4*Formation_expansion_factor),formationCenter.z))));
                break;
            }
            default:
            {
                break;
            }
        }
    }

    
    // switch(nums)
    // {
    //     case 3:
    //     {
    //         res.insert(make_pair(ids[0],FormationPoint(uav.getUavID(),Point3D(formationCenter.x,
    //                                                                   static_cast<float>(formationCenter.y), formationCenter.z))));

    //         res.insert(make_pair(ids[1],FormationPoint(other_uav_id[0],Point3D(static_cast<float>(formationCenter.x - 2),
    //                                                                   static_cast<float>(formationCenter.y - 2), formationCenter.z))));

    //         res.insert(make_pair(ids[2],FormationPoint(other_uav_id[1],Point3D(static_cast<float>(formationCenter.x + 2),
    //                                                                   static_cast<float>(formationCenter.y - 2), formationCenter.z))));

    //         break;
    //     }
    //     case 4:
    //         break;
    //     default:
    //         break;
    // }
    return res;
}

Point3D calculateSwarCenter(uav_zyl& _uav)
{
    int carid = _uav.getUavID();
    Point3D res = _uav.getPosition();
//    cout<<"wo de tian a "<<res.x<<res.y<<res.z<<endl;
    float x = 0,y = 0,z = 0;
    x += _uav.getPosition().x;
    y += _uav.getPosition().y;
    z += _uav.getPosition().z;
    int num = 0;
    if(other_uav_count == 0)
        return _uav.getPosition();
    for(int i = 1;i<=other_uav_count + 1;i++)
    {
        if(i == _uav.getUavID())
            continue;
        x += swarmlocation[i].x;
        y += swarmlocation[i].y;
        z += swarmlocation[i].z;
    }
    x /= (other_uav_count + 1);
    y /= (other_uav_count + 1);
    z /= (other_uav_count + 1);
    res.x = x;
    res.y = y;
    res.z = z;
    return res;

}


bool findFormationPoint(uav_zyl& car,formationType& formationPoints)
{
    bool res = false;
    formationType::iterator iter;
    if(formationPoints.find(car.getUavID()) != formationPoints.end())
    {
        iter = formationPoints.find(car.getUavID());
        car.setFormationPoint(iter->second.point);
        res = true;
    } else
        res = false;

    return res;
}

direction findDir(float speed, Point3D nowPosition, direction& moveDirection, const vector<Point3D>& swarmPosition, float VEHICLE_DIAMETER)
{
    int barrierflg = 0;
    int swarmflg = 0;
    int count = 0;
    int randomseed = 0;
    Point3D attemptPosition;
    int flg = 1;
    while (flg && cmd_status==true)
    {
        attemptPosition = nowPosition;
        switch (moveDirection)
        {
            case (direction)0://left
                attemptPosition.x = nowPosition.x - speed;
                break;
            case (direction)1://left_up
                attemptPosition.x = nowPosition.x - speed;
                attemptPosition.y = nowPosition.y + speed;
                break;
            case (direction)2://up
                attemptPosition.y = nowPosition.y + speed;
                break;
            case (direction)3://right_up
                attemptPosition.x = nowPosition.x + speed;
                attemptPosition.y = nowPosition.y + speed;
                break;
            case (direction)4://right
                attemptPosition.x = nowPosition.x + speed;
                break;
            case (direction)5://right_down
                attemptPosition.x = nowPosition.x + speed;
                attemptPosition.y = nowPosition.y - speed;
                break;
            case (direction)6://down
                attemptPosition.y = nowPosition.y - speed;
                break;
            case (direction)7://left_down
                attemptPosition.x = nowPosition.x - speed;
                attemptPosition.y = nowPosition.y - speed;
                break;
            default:
                break;
        }
        barrierflg = JudgeBarrier(attemptPosition,VEHICLE_DIAMETER/2);
        if (!barrierflg)
        {
            for (auto it = swarmPosition.begin(); it != swarmPosition.end(); it++)
            {
                
                if (abs(attemptPosition - (*it)) < VEHICLE_DIAMETER)
                    swarmflg = 1;
            }
            if (!swarmflg)
                return moveDirection;//
        }

        if(swarmflg == 1)//chuli jiqunbizhang de wenti 
        {
            ROS_INFO("dealwith swarm CA");

            return direction((moveDirection + 2 ) % 8);
        }
        if (barrierflg == 1)
        {
            moveDirection = (direction)((moveDirection + 1) % 8);
        }
        else if (barrierflg == 2)
            moveDirection = (direction)(abs(moveDirection + 7) % 8);
        count++;
        if (count == 8)
            flg = 0;

        ros::Duration(0.1).sleep();
    }
    moveDirection = here;
    ROS_INFO("I just stay here!");
    return moveDirection;//
}

/*
 *发送编队形成信号
 */
bool sendformationMsg(int uavID,bool formationflag)
{
    formationmsg::FormationMsg FM_msg;
    FM_msg.set_uavid(static_cast<google::protobuf::uint32>(uavID));
    FM_msg.set_formationflag(formationflag);
    FM_msg.set_changeformation(false);
    ROS_INFO("Send formation is :%d ",formationflag);
    simulation_m100::PubSubUavMsg FM_msg_pub;
    if(!FM_msg.SerializeToString(&FM_msg_pub.playload))
    {
        perror("serial error");
        return false;
    }

    formation_info_pub.publish(FM_msg_pub);

    return true;
}

/*
 *发送控制点信号
 */
bool sendformationMsg(int targetUavID,Point3D points)
{
    formationmsg::FormationMsg FM_msg;
    FM_msg.set_uavid(static_cast<google::protobuf::uint32>(targetUavID));
    FM_msg.set_changeformation(true);
    FM_msg.set_formationpointx(points.x);
    FM_msg.set_formationpointy(points.y);
    FM_msg.set_formationpointz(points.z);
    //ROS_INFO("I(uav%d) send to uav%d point is: %f , %f , %f",uav.getUavID(),targetUavID,points.x,points.y,points.z);
    
    simulation_m100::PubSubUavMsg FM_msg_pub;
    if(!FM_msg.SerializeToString(&FM_msg_pub.playload))
    {
        perror("serial error");
        return false;
    }

    formation_info_pub.publish(FM_msg_pub);

    return true;
}



void onestep(uav_zyl& theuav,Point3D movePoint,fstream& recordcar,fstream& recordinput,float speedgain = 1)
{
    struct timeval tv;
    struct timezone tz;
    
    float controlconfficient = 1;
    float distence = 0.0;

    struct tm *p;

    distence = abs(movePoint - theuav.getPosition());
    // if(distence<0.5)
    // {
    //     // ROS_INFO("No need to move");
    //     return ;
    // }
    if(distence  <= 3.0)
    {
        controlconfficient = 1;
    }else if(distence > 3.0 && distence <= 5.0)
    {
        controlconfficient = 1.2;
    }else if(distence > 5.0)
    {
        controlconfficient = 1.6;
    }


    vector<Point3D> otherSwarmPosition;
    getSwarmPosition(otherSwarmPosition);
    direction  moveDir;
    direction tempDir,tempDir2;
    tempDir = calculateDirection(movePoint,theuav.getPosition(),Radius);
    tempDir2 = tempDir;
    
    moveDir = findDir(runspeed,uav.getPosition(),tempDir,otherSwarmPosition,Unmanned_system_diameter);
    if(tempDir2 != tempDir)
    {
        myBehavior = avoidobstacles;
    }

    theuav.moveTowardDir(moveDir,moveCtrlMode,controlconfficient*speedgain);
    gettimeofday(&tv, &tz);
    p = localtime(&tv.tv_sec);

    recordcar<<movecount++<<","<<p->tm_hour<<","<< p->tm_min<<","<< p->tm_sec<<","
             << tv.tv_usec<<"," <<uav.x_speed<<","<<uav.y_speed<<","<<theuav.z_speed<<","<<theuav.getPosition().x
             <<","<<theuav.getPosition().y<<","<<theuav.getPosition().z<<","<<movePoint.x - theuav.getPosition().x<<","
             <<movePoint.y - theuav.getPosition().y<<","
             <<movePoint.z - theuav.getPosition().z<<myBehavior<<endl;
    recordinput<<movecount<<","<<moveDir<<","<<controlconfficient<<",("<<theuav.getPosition().x
             <<","<<theuav.getPosition().y<<","<<theuav.getPosition().z<<"),("<<movePoint.x <<","
             <<movePoint.y <<","
             <<movePoint.z <<")"<<endl;
    usleep(50000);
    // cout<<movecount<<","<<p->tm_hour<<","<< p->tm_min<<","<< p->tm_sec<<","
    //     << tv.tv_usec<<"," <<uav.x_speed<<","<<uav.y_speed<<","<<uav.getPosition().x
    //     <<","<<uav.getPosition().y<<movePoint.x - theuav.getPosition().x<<","
    //     <<movePoint.y - theuav.getPosition().y<<","
    //     <<movePoint.z - theuav.getPosition().z<<endl;
    //此处为需要记录的信息

}

string getDirectory()
{
	char abs_path[1024];
	int cnt = readlink("/proc/self/exe", abs_path, 1024);//可执行程序绝对路径
	if (cnt < 0 || cnt >= 1024)
	{
		return NULL;
	}

	//清除部分路径，回退至工作空间下
	int count = 0;
	for (int i = cnt; i >= 0; --i)
	{
		if (abs_path[i] == '/')
		{
			abs_path[i + 1] = '\0';
			count++;

			if (count == 4)
			{
				break;
			}
		}
	}

	string path(abs_path);

	path = path + "src/";

	return path;
}

void formation(stack<Point3D> leaderPath)
/*
 * 信息交互在形成队形之前交互formationflag，以及位置信息
 * 之后交互位置信息即可。
 */
{
    ROS_INFO("STAR FORMATION !!!!");
    if(other_uav_count == 0)
    {
        leaderid = uav.getUavID();
    }
    bool* thisflag = (bool*)malloc(sizeof(bool));
    int formationnumber = 0;//队形编号，初始队形默认为0

    Target myTarget;
    switch(uav.getUavID())
        /*
         * 这边无人系统的ID定义为10,11,12可以保持一致也可以修改
         */
    {
        case 1:
            // myTarget = target[0];
            // uav.setPosition(0.6,0,0);
            thisflag = &flag1;
            break;
        case 2:
            // myTarget = target[1];
            // uav.setPosition(1.8,0,0);
            thisflag = &flag2;
            break;
        case 3:
            // myTarget = target[2];
            // uav.setPosition(3.0,0,0);
            thisflag = &flag3;
            break;
        default:
            break;
    }

    
    Point3D lastTarget(0,0,0);
    Point3D targetPoint(0,0,0);
    Point3D swarmCenter;
    direction moveDir;
    vector<Point3D> swarmPosition;

    targetPoint = leaderPath.top();
    lastTarget = targetPoint;
    leaderPath.pop();

    carids.push_back(uav.getUavID());
    ROS_INFO("I have %d other uavs",other_uav_count);
    for(int i = 0;i < other_uav_count;i++)
    {
        carids.push_back(other_uav_id[i]);
    }
    

    // fstream recordFormation1;
    // fstream recordFormation2;
    fstream recordFormation3;
    fstream recordSwarmCenter;
    fstream recordControlInput;
    fstream recordFormationPoint;
    fstream recordLeaderPath;

    // string fm_file1(getDirectory() + "simulation_m100/log/beforeFormation" + uavName.c_str() + ".txt");
    // string fm_file2(getDirectory() + "simulation_m100/log/cometoFormation" + uavName.c_str() + ".txt");
    string fm_file3(getDirectory() + "simulation_m100/log/Formationrecord" + uavName.c_str() + ".txt");
    string fm_file4(getDirectory() + "simulation_m100/log/FormationCenter" + uavName.c_str() + ".txt");
    string fm_file5(getDirectory() + "simulation_m100/log/ControlInput" + uavName.c_str() + ".txt");
    string fm_file6(getDirectory() + "simulation_m100/log/FormationPoint" + uavName.c_str() + ".txt");
    string fm_file8(getDirectory() + "simulation_m100/log/LeaderPath" + uavName.c_str() + ".txt");

    // recordFormation1.open(fm_file1.c_str(),ios::app);
    // recordFormation2.open(fm_file2.c_str(),ios::app);
    recordFormation3.open(fm_file3.c_str(),ios::app);
    recordSwarmCenter.open(fm_file4.c_str(),ios::app);
    recordControlInput.open(fm_file5.c_str(),ios::app);
    recordFormationPoint.open(fm_file6.c_str(),ios::app);
    recordLeaderPath.open(fm_file8.c_str(),ios::app);

    // recordFormation1<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    // recordFormation2<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    recordFormation3<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    recordSwarmCenter<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    recordControlInput<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    recordFormationPoint<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    recordLeaderPath<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    // flag2 = true;
    // flag3 = true;

    switch(other_uav_count)
    {
        case 0:
        {
            while(!(*thisflag))
            {
                if(isApproimate(uav.getPosition(),myFormationTarget.point,Unmanned_system_diameter*0.5))
                {
                    usleep(10000);
                    ROS_INFO("Our group have %d uavs",other_uav_count + 1);
                    ROS_INFO("uav%d reached %lf,%lf,%lf",uav.getUavID(),myFormationTarget.point.x,myFormationTarget.point.y,myFormationTarget.point.z);
                    *thisflag = true;
                    sendformationMsg(uav.getUavID(),*thisflag);
                    uav.moveTowardDir(here,moveCtrlMode);
                    
                }else
                {
                    direction tempDir;
                    onestep(uav,myFormationTarget.point,recordFormation3,recordControlInput);
                    // getSwarmPosition(swarmPosition);
                    // tempDir = calculateDirection(myFormationTarget.point,uav.getPosition(),0.3);
                    // moveDir = findDir(runspeed,uav.getPosition(),tempDir,swarmPosition,Unmanned_system_diameter*0.3);
                    // uav.moveTowardDir(tempDir,moveCtrlMode);
                    // recordFormation1<<"Before cometo formation"<<uav.x_speed<<","<<uav.y_speed<<","<<uav.z_speed<<uav.getPosition().x<<","<<uav.getPosition().y<<","<<uav.getPosition().z<<endl;
                    ROS_INFO("Before cometo formation: %f,%f,%f  %d",uav.getPosition().x,uav.getPosition().y,uav.getPosition().z,moveDir);
                    // cout<<"Before cometo formation"<<uav.x_speed<<","<<uav.y_speed<<","<<uav.getPosition().x<<","<<uav.getPosition().y<<endl;
                    sendformationMsg(uav.getUavID(),*thisflag);//发送编队形成消息//
                }
                ros::Duration(0.1).sleep();
            }
            break;
            
        }
        case 1:
        {
            while(!(flag1&flag2)&&cmd_status==true)
            {
                if(isApproimate(uav.getPosition(),myFormationTarget.point,Unmanned_system_diameter*0.5))
                {
                    usleep(10000);
                    ROS_INFO("Our group have %d uavs",other_uav_count + 1);
                    ROS_INFO("uav%d reached %lf,%lf,%lf",uav.getUavID(),myFormationTarget.point.x,myFormationTarget.point.y,myFormationTarget.point.z);
                    switch (uav.getUavID())
                    {
                        case 1:
                        {
                            flag1 = true;
                            break;
                        }
                        default:
                        {
                            flag2 = true;
                            break;
                        }
                        sendformationMsg(uav.getUavID(),*thisflag);
                        uav.moveTowardDir(here,moveCtrlMode);//停止
                    }
                }else
                {
                    direction tempDir;
                    onestep(uav,myFormationTarget.point,recordFormation3,recordControlInput);
                    // getSwarmPosition(swarmPosition);
                    // tempDir = calculateDirection(myFormationTarget.point,uav.getPosition(),0.3);
                    // moveDir = findDir(runspeed,uav.getPosition(),tempDir,swarmPosition,Unmanned_system_diameter*0.3);
                    // uav.moveTowardDir(tempDir,moveCtrlMode);
                    // recordFormation1<<"Before cometo formation"<<uav.x_speed<<","<<uav.y_speed<<","<<uav.z_speed<<uav.getPosition().x<<","<<uav.getPosition().y<<","<<uav.getPosition().z<<endl;
                    // ROS_INFO("Before cometo formation: %f,%f,%f  %d",uav.getPosition().x,uav.getPosition().y,uav.getPosition().z,moveDir);
                    // cout<<"Before cometo formation"<<uav.x_speed<<","<<uav.y_speed<<","<<uav.getPosition().x<<","<<uav.getPosition().y<<endl;
                    sendformationMsg(uav.getUavID(),*thisflag);//发送编队形成消息///////////////////////
                }
                ros::Duration(0.1).sleep();
            }
            break;
        }
        case 2:
        {
            while(!(flag1&flag2&flag3) && cmd_status==true)
            {
                    
                if(isApproimate(uav.getPosition(),myFormationTarget.point,Unmanned_system_diameter*0.5))
                {
                    usleep(10000);
                    ROS_INFO("Our group have %d uavs ,my flag is %d",other_uav_count + 1,*thisflag);
                    ROS_INFO("Formation flag are %d , %d , %d ",flag1,flag2,flag3);
                    ROS_INFO("uav%d reached %lf,%lf,%lf",uav.getUavID(),myFormationTarget.point.x,myFormationTarget.point.y,myFormationTarget.point.z);
                    switch (uav.getUavID())
                    {
                        case 1:
                            flag1 = true;
                            break;
                        case 2:
                            flag2 = true;
                            break;
                        case 3:
                            flag3 = true;
                            break;
                        default:
                            break;
                    }
                        
                    sendformationMsg(uav.getUavID(),*thisflag);
                    uav.moveTowardDir(here,moveCtrlMode);//停止
                }
                else
                {
                    direction tempDir;
                    onestep(uav,myFormationTarget.point,recordFormation3,recordControlInput);
                    // getSwarmPosition(swarmPosition);
                    // tempDir = calculateDirection(myFormationTarget.point,uav.getPosition(),0.3);
                    // moveDir = findDir(runspeed,uav.getPosition(),tempDir,swarmPosition,Unmanned_system_diameter*0.3);
                    // uav.moveTowardDir(tempDir,moveCtrlMode);
                    // recordFormation1<<"Before cometo formation"<<uav.x_speed<<","<<uav.y_speed<<","<<uav.z_speed<<uav.getPosition().x<<","<<uav.getPosition().y<<","<<uav.getPosition().z<<endl;
                    ROS_INFO("Before cometo formation: %f,%f,%f  %d",uav.getPosition().x,uav.getPosition().y,uav.getPosition().z,moveDir);
                    // cout<<"Before cometo formation"<<uav.x_speed<<","<<uav.y_speed<<","<<uav.getPosition().x<<","<<uav.getPosition().y<<endl;
                    sendformationMsg(uav.getUavID(),*thisflag);//发送编队形成消息///////////////////////
                }
                ros::Duration(0.1).sleep();
            }//已经形成队形
            break;
        }
        default:
        {
            ROS_INFO("Number of uav error");
            break;
        }

    }

    sendformationMsg(uav.getUavID(),*thisflag);
    uav.moveTowardDir(here,moveCtrlMode);//停止
    
    ROS_INFO("Already come into formation !");
    ros::Duration(3).sleep();//等待一段时间
    myBehavior = movetogoal;
    int pathid = 0;

    int formationdir = 2;//用于计算编队行驶过程中队形的朝向
    vector<int> dir_queue(5,0);
    int num_dir = 0;
    int tempformationdir = 0;
    float followerspeedgain = 1.15;
    float formationshensuoyinzi = 2;
    bool changeformationdir = false;

    
    while(!leaderPath.empty() && cmd_status==true)
    {
        if(uav.getUavID() == leaderid)
        {
            swarmCenter = calculateSwarCenter(uav);
            recordSwarmCenter<<"swarmCenter is :"<<swarmCenter.x<<","<<swarmCenter.y<<","<<swarmCenter.z<<" uav1:"<<uav.getPosition().x<<","<<uav.getPosition().y<<","<<uav.getPosition().z<<
            " uav2"<<swarmlocation[2].x<<","<<swarmlocation[2].y<<","<<swarmlocation[2].z<<" uav3"<<swarmlocation[3].x<<","<<swarmlocation[3].y<<","<<swarmlocation[3].z<<endl;
            // ROS_INFO("the first time swarmCenter is %f , %f ",swarmCenter.x,swarmCenter.y);//just for test
            ROS_INFO("leader Position is %f %f , %f ",uav.getPosition().x,uav.getPosition().y,uav.getPosition().z);
            if(isApproimate(uav.getPosition(),swarmCenter,10*Unmanned_system_diameter))//向任务目标点运动行为
            {
                myBehavior = movetogoal;
                if(isApproimate(uav.getPosition(),lastTarget,Unmanned_system_diameter*0.5))
                {
                    targetPoint = leaderPath.top();
                    leaderPath.pop();
                    pathid++;
                }else
                {
                    targetPoint = lastTarget;
                }
                recordLeaderPath<<pathid<<" : "<< targetPoint.x<<","<<targetPoint.y<<","<<targetPoint.z<<endl;
                ROS_INFO("move to target %d and point is %f , %f , %f",pathid, targetPoint.x,targetPoint.y,targetPoint.z);

                if(pathid == 0 )
                {
                    uav.moveTowardDir(here,moveCtrlMode);
                    //ros::Duration(2).sleep();
                    ROS_INFO("star to move !");
                    pathid++;
                }
                
                tempformationdir = calp2pdir(uav.getPosition(),targetPoint);
                if(num_dir < 5)
                {
                    dir_queue[num_dir++] = tempformationdir;
                }
                else//num_dir >=5
                {
                    num_dir = 0;
                }
                if(dir_queue[0] == dir_queue[1] && dir_queue[1] == dir_queue[2] && dir_queue[0] == dir_queue[3] && dir_queue[0] == dir_queue[4])
                {
                    if(formationdir != dir_queue[0])
                    {
                        ROS_INFO("I will change formationdir");
                        formationdir = dir_queue[0];
                        changeformationdir = true;
                    }
                }

                if(pathid == PATHNUMBER-10 )//the first time
                /*执行编队变换
                */
                {
                    ROS_INFO("Change the formation!!!");
                    formationnumber = 1;
                    swarmCenter = calculateSwarCenter(uav);
                    formationPoints = calFormationPoint(carids,uav.getPosition(),formationnumber,formationdir);
                    
                    for(int i = 0;i<other_uav_count;i++)//向其他无人系统发布编队控制点信息
                    {
                        Point3D temppointi = formationPoints.find(other_uav_id[i])->second.point;
                        //ROS_INFO("%d formationPoint is : %f,%f",other_uav_id[i],temppointi.x,temppointi.y);
                        sendformationMsg(other_uav_id[i],temppointi);
                    }

                    uav.moveTowardDir(here,moveCtrlMode);
                    ros::Duration(15).sleep();//静态队形变换中的等待时间
                    pathid++;
                    ROS_INFO("Leader star to move");

                }

                if(pathid == PATHNUMBER-1 )//the second time
                /*执行编队变换
                */
                {
                    ROS_INFO("Change the formation!!!");
                    formationnumber = 0;
                    swarmCenter = calculateSwarCenter(uav);
                    formationPoints = calFormationPoint(carids,uav.getPosition(),formationnumber,formationdir);
                    
                    for(int i = 0;i<other_uav_count;i++)//向其他无人系统发布编队控制点信息
                    {
                        Point3D temppointi = formationPoints.find(other_uav_id[i])->second.point;
                        //ROS_INFO("%d formationPoint is : %f,%f",other_uav_id[i],temppointi.x,temppointi.y);
                        sendformationMsg(other_uav_id[i],temppointi);
                    }

                    uav.moveTowardDir(here,moveCtrlMode);
                    ros::Duration(15).sleep();//静态队形变换中的等待时间
                    pathid++;
                    ROS_INFO("Leader star to move");

                }

                
               // onestep(uav,targetPoint,recordFormation3,recordControlInput);
                swarmCenter = calculateSwarCenter(uav);
                ROS_INFO("beafore :%d , %d",formationnumber,formationdir);
                formationPoints = calFormationPoint(carids,uav.getPosition(),formationnumber,formationdir);


                for(int i = 0;i<other_uav_count;i++)//向其他无人系统发布编队控制点信息
                {
                    Point3D temppointi = formationPoints.find(other_uav_id[i])->second.point;
                    recordFormationPoint<<other_uav_id[i]<<" formationPoint is : "<<temppointi.x <<","<<temppointi.y<<","<<temppointi.z<<endl;
                    ROS_INFO("after!!! %d formationPoint is : %f,%f",other_uav_id[i],temppointi.x,temppointi.y);
                    sendformationMsg(other_uav_id[i],temppointi);
                }

                if(changeformationdir)
                {
                    uav.moveTowardDir(here,moveCtrlMode);
                    ROS_INFO("change formationdir");
                    ros::Duration(10).sleep();//静态队形变换中的等待时间
                    ROS_INFO("last 3 formationdir are: %d %d %d",dir_queue[0],dir_queue[1],dir_queue[2]);
                    changeformationdir = false;
                }
                onestep(uav,targetPoint,recordFormation3,recordControlInput);



                // Point3D tempPoint11,tempPoint12;s
                // tempPoint11 = formationPoints.find(other_uav_id[0])->second.point;
                // tempPoint12 = formationPoints.find(other_uav_id[1])->second.point;
                // ROS_INFO("%d formationPoint is : %f,%f",other_uav_id[0],tempPoint11.x,tempPoint11.y);
                // ROS_INFO("%d formationPoint is : %f,%f",other_uav_id[1],tempPoint12.x,tempPoint12.y);
                // sendformationMsg(other_uav_id[0],tempPoint11);
                // sendformationMsg(other_uav_id[1],tempPoint12);

                lastTarget = targetPoint;
            }
            else//队形保持行为
            {
                myBehavior = keepformation;
                //ROS_INFO("maintain formation %f , %f",targetPoint.x,targetPoint.y);
                swarmCenter = calculateSwarCenter(uav);
                formationPoints = calFormationPoint(carids,uav.getPosition(),formationnumber,formationdir);
                ROS_INFO("I'm keeping the formation");

                onestep(uav,swarmCenter,recordFormation3,recordControlInput);

                for(int i = 0;i<other_uav_count;i++)
                {
                    Point3D temppointi = formationPoints.find(other_uav_id[i])->second.point;
                    //ROS_INFO("%d formationPoint is : %f,%f",other_uav_id[i],temppointi.x,temppointi.y);
                    sendformationMsg(other_uav_id[i],temppointi);
                }
                

                // Point3D tempPoint11,tempPoint12;
                // tempPoint11 = formationPoints.find(other_uav_id[0])->second.point;
                // tempPoint12 = formationPoints.find(other_uav_id[1])->second.point;
                // sendformationMsg(other_uav_id[0],tempPoint11);
                // sendformationMsg(other_uav_id[1],tempPoint12);
                lastTarget = targetPoint;
            }

        }else//从机跟随飞行
        {
            myBehavior = keepformation;
            if(formationover)
                break;
            bool runflag = false;
            while(runflag && cmd_status==true)
            {
                runflag = findFormationPoint(uav,formationPoints);
            }
            Point3D tempformationPoint = formationPoints.find(uav.getUavID())->second.point;
            ROS_INFO("uav%d move from %f , %f to %f ,%f",uav.getUavID(),uav.getPosition().x,uav.getPosition().y,tempformationPoint.x,tempformationPoint.y);
            float follower_dis = tempformationPoint - uav.getPosition();
            if(abs(tempformationPoint -Point3D(0,0,0)) < 1 )
            {
                ROS_INFO("error Formationpoint!!!");
                uav.moveTowardDir(here,moveCtrlMode);
                continue;
            }
            
            onestep(uav,tempformationPoint,recordFormation3,recordControlInput,followerspeedgain);
            
            
        }
        ros::Duration(0.1).sleep();
    }
    uav.moveTowardDir(here,moveCtrlMode);
    sendformationMsg(9999,Point3D(0,0,0));
    ros::Duration(5).sleep();
    thisflag = NULL;

    free(thisflag);

    carids.clear();
    // recordFormation1.close();
    // recordFormation2.close();
    recordFormation3.close();
    recordSwarmCenter.close();
    recordControlInput.close();
    recordFormationPoint.close();
    recordLeaderPath.close();
}

void *experiment(void* args)
{
    while (ros::ok())
    {   
        while (thePathTargets.size() == 0 || uav.getPosition().z <= 0)
        {
            ros::Duration(0.1).sleep();
        }

        vector<Point3D> a;
        while(ros::ok() && cmd_status==true)
        {
            ros::Duration(1).sleep();
            if(myFormationTarget.point-Point3D(0,0,0) > 0.1)
            {
                //ROS_INFO("myFormationTarget is : %f,%f,%f",myFormationTarget.point.x,myFormationTarget.point.y,myFormationTarget.point.z);
                if(!thePathTargets.empty())
                    break;
            }
        }
        
        // /*
        //  * 此处可以修改为输入信息是一系列地面站给出的跟踪目标点，至少为4个
        //  */

        // Point3D  ax1 = { 2*MAPXISHU,2*MAPXISHU,0 }; a.push_back(ax1);
        // Point3D  ax2 = { 3*MAPXISHU,3*MAPXISHU,0 }; a.push_back(ax2);
        // Point3D  ax3 = { 4*MAPXISHU,3*MAPXISHU,0 }; a.push_back(ax3);
        // Point3D  ax4 = { 5*MAPXISHU,3*MAPXISHU,0 }; a.push_back(ax4);
        // Point3D  ax5 = { 6*MAPXISHU,3*MAPXISHU,0 }; a.push_back(ax5);
        // Point3D  ax6 = { 7*MAPXISHU,3.5*MAPXISHU,0 }; a.push_back(ax6);
        // Point3D  ax7 = { 8*MAPXISHU,5*MAPXISHU,0 }; a.push_back(ax7);
        // Point3D	 ax8 = { 7*MAPXISHU,7.4*MAPXISHU,0 }; a.push_back(ax8);
        // Point3D ax9 = { 6*MAPXISHU,8*MAPXISHU,0 }; a.push_back(ax9);
        // Point3D ax10 = { 5*MAPXISHU,8*MAPXISHU,0 }; a.push_back(ax10);
        // Point3D ax11 = { 4*MAPXISHU,8*MAPXISHU,0 }; a.push_back(ax11);
        // Point3D ax12 = { 3*MAPXISHU,8*MAPXISHU,0 }; a.push_back(ax12);
        // Point3D ax13 = { 2*MAPXISHU,8*MAPXISHU,0 }; a.push_back(ax13);

        if(thePathTargets.size() < 4)
        {
            thePathTargets.insert(thePathTargets.begin(),Target(myFormationTarget.point.x,myFormationTarget.point.y,myFormationTarget.point.z,uav.getUavID()));

            Point3D initPoint = myFormationTarget.point;
            Point3D guihuaPoint = thePathTargets[1].point;
            vector<Target>::iterator iter = thePathTargets.begin();
            iter++;
            float dis_x = iter->point.x - myFormationTarget.point.x;
            float dis_y = iter->point.y - myFormationTarget.point.y;
            float dis_z = iter->point.z - myFormationTarget.point.z;
            while(thePathTargets.size() < 4  && cmd_status==true)
            {
                thePathTargets.insert(iter,Target((myFormationTarget.point.x + dis_x/3),(myFormationTarget.point.y + dis_y/3),(myFormationTarget.point.z + dis_z /3),uav.getUavID()));
                thePathTargets.insert(iter,Target((myFormationTarget.point.x + 2*dis_x/3),(myFormationTarget.point.y + 2*dis_y/3),(myFormationTarget.point.z + 2*dis_z /3),uav.getUavID()));
                ros::Duration(0.1).sleep();
            }
        }

        // if(thePathTargets.size() < 4)
        // {
        //     thePathTargets.resize(8);
        // }
        // thePathTargets.insert(thePathTargets.begin(),Target(myFormationTarget.point.x,myFormationTarget.point.y,myFormationTarget.point.z,uav.getUavID()));

        // Point3D initPoint = myFormationTarget.point;
        // Point3D guihuaPoint = thePathTargets[1].point;
        // vector<Target>::iterator iter = thePathTargets.begin();
        // iter++;
        // float dis_x = iter->point.x - myFormationTarget.point.x;
        // float dis_y = iter->point.y - myFormationTarget.point.y;
        // float dis_z = iter->point.z - myFormationTarget.point.z;
        // while(thePathTargets.size() < 4  && cmd_status==true)
        // {
        //     thePathTargets.insert(iter,Target((myFormationTarget.point.x + dis_x/3),(myFormationTarget.point.y + dis_y/3),(myFormationTarget.point.z + dis_z /3),uav.getUavID()));
        //     thePathTargets.insert(iter,Target((myFormationTarget.point.x + 2*dis_x/3),(myFormationTarget.point.y + 2*dis_y/3),(myFormationTarget.point.z + 2*dis_z /3),uav.getUavID()));
        // }

        fstream FormationPath;
        string fm_file7(getDirectory() + "simulation_m100/log/FormationPath" + uavName.c_str() + ".txt");

        FormationPath.open(fm_file7.c_str(),ios::app);

        int num_path = 0;

        for(auto p:thePathTargets)
        {
            a.push_back(p.point);
            
        }
          

        Spline s;
        vector<Point3D>  re;
        s.setVertex(a, PATHNUMBER);
        re = s.spline_create();
        std::reverse(re.begin(), re.end());
        stack<Point3D> paths;

        for (auto p : re)
        {
            FormationPath<<"the num of path"<<num_path++<<":"<<p.x<<","<<p.y<<","<<p.z<<endl;
            paths.push(p);
        }

        
        formation(paths);//编队控制入口


        ROS_INFO("Formation finished!");
        thePathTargets.clear();

        uav.moveTowardDir(here,moveCtrlMode);//停止
        FormationPath.close();
        ros::Duration(0.1).sleep();
    }
}

void subCmdResultCallback(simulation_m100::CmdResult result)
{
	//ROS_INFO("%s,%s",result.component.c_str(),component.c_str());
	//ROS_INFO("%s,%s",result.command.c_str(),move_cmd.c_str());

	if (strcmp(result.component.c_str(), component.c_str()) == 0)
	{
		if (strcmp(result.command.c_str(), move_cmd.c_str()) == 0)
		{
			cmd_status = result.status;

			if (cmd_status == false)
			{
				ROS_INFO("move_refused!");
			}
		}
	}
}

/*
*从定位子系统订阅本机的实时位置信息，并设置到uav中；本机位置的广播由其他子系统完成
*/
void subPosCallback(simulation_m100::Position pos_msg)//本机位置更新
{
	Point3D my_point(pos_msg.x, pos_msg.y, pos_msg.z);

	uav.setPosition(my_point.x,my_point.y,my_point.z);
}

/*
*从其他无人机接收它们的位置信息
*本机维护所有其他无人机的位置信息swarmlocation，每次接收到某个无人机的位置信息时，则覆盖更新之
*/
void subOtherUavPosCallback(simulation_m100::PubSubUavMsg other_uav_pos_msg)//实时更新其他无人机位置
{
	uwblpsMessage::uwblpsMessage other_uav_pos;
	other_uav_pos.ParseFromString(other_uav_pos_msg.playload);

	Point3D point(other_uav_pos.linear_x(), other_uav_pos.linear_y(), other_uav_pos.linear_z());

	int src_uav_id = other_uav_pos_msg.src_uav_id;

    swarmlocation[src_uav_id] = point;

    pthread_mutex_lock(&buf_mutex);

	int i = 0;
	for (i; i<uavIndexBuf.size(); i++)
	{
		if (src_uav_id == uavIndexBuf[i])//缓存中有该无人系统的数据，则更新
		{
			swarmPositionBuf[i] = point;;
			posUpdateTime[i] = ros::Time::now();
			break;
		}
	}

	if (i == uavIndexBuf.size())//缓存中没有该无人系统的数据，则存入
	{
		uavIndexBuf.push_back(src_uav_id);
		swarmPositionBuf.push_back(point);
		posUpdateTime.push_back(ros::Time::now());
	}

	for (int j = 0; j<uavIndexBuf.size(); j++)//如果某个无人系统的位置数据在一段时间内没有被更新过，则将它“踢出”集群
											  //TODO：“踢出”的具体条件需进一步商定（位置没有改变？还是没有再接收到位置数据？）
											  //TODO：“踢出”的优化操作（移除相关数据？还是将数据置为无效？）
	{
		if (uavIndexBuf[j] != -1)
		{
			if (ros::Time::now() - posUpdateTime[j] > ros::Duration(10.0))
			{
				uavIndexBuf[j] = -1;

				Point3D tempPoint(0, 0, 0);
				swarmPositionBuf[j] = tempPoint;
			}
		}
	}

	pthread_mutex_unlock(&buf_mutex);
}

/*
*从地面站接收编队航点，包括编队形成的目标点和编队形成后的运动路径目标点（建议以该消息为编队任务的触发点）
*格式为：(x y z yaw type mark) (x y z yaw type mark)……
*其中，x y z为目标点位置，yaw为到底目标点后的偏航角，type为目标点类型（局部航点、全局航点等），mark为应该到达该点的无人机的ID，如果为0，则为编队后的运动轨迹点（所有无人机均需到达）
*/
void subWaypointFromGcsCallback(const simulation_m100::PubSubUavMsg waypoint_cmd_msg)//订阅并分析地面站发来的航点序列
{
    myTaskMessage::TaskMessage waypoint;//航点序列
    waypoint.Clear();
    waypoint.ParseFromString(waypoint_cmd_msg.playload);

    thePathTargets.clear();

    if (waypoint_cmd_msg.src_uav_id == 0)//初始化目标点（初次从地面接收到目标点）
	{
        ROS_INFO("received waypoints!");
 //       Target myFormationTarget;//编队形成时的目标点（通常只有一个，用于本机参与形成队形），已经定义为全局变量或其他
 //       vector<Target> thePathTargets;//编队形成后的运动路径目标点序列（一个或多个），已经定义为全局变量或其他
        for(int i=0;i<waypoint.point_size();i++)
        {
            ROS_INFO("%f,%f,%f,%d",waypoint.point(i).x(),waypoint.point(i).y(),waypoint.point(i).z(),waypoint.point(i).mark());
            if(waypoint.point(i).mark() == uav.getUavID())
            {
                myFormationTarget.point.x = waypoint.point(i).x();
                myFormationTarget.point.y = waypoint.point(i).y();
                myFormationTarget.point.z = waypoint.point(i).z();
                myFormationTarget.markerID = waypoint.point(i).mark();
            }

            if(waypoint.point(i).mark() == 0)
            {
                Target target(waypoint.point(i).x(),waypoint.point(i).y(),waypoint.point(i).z(),waypoint.point(i).mark());
                thePathTargets.push_back(target);
            }
        }

        cmd_status = true;
        flag1 = false;
        flag2 = false;
        flag3 = false;
        formationover = false;
    }
}

/*
*从其他无人机接收编队航点，只包括编队形成后的运动路径目标点，用于交互航点到达状态信息（在编队中似乎没用到）
*格式为：(x y z yaw type mark) (x y z yaw type mark)……
*其中，x y z为目标点位置，yaw为到底目标点后的偏航角，type为目标点类型（局部航点、全局航点等），mark为已经到达该点的无人机的ID，如果为0，则表示尚未有无人机到底过该点
*/
void subOtherUavFdWPCallback(simulation_m100::PubSubUavMsg other_uav_fd_wp_msg)//实时更新目标航点到达状态（从其他无人机接收到目标点）
{
	myTaskMessage::TaskMessage waypoint;
	waypoint.ParseFromString(other_uav_fd_wp_msg.playload);

	if (other_uav_fd_wp_msg.src_uav_id != 0)
	{
		//ROS_INFO("received a notification!");
        vector<Target> thePathTargets;//编队形成后的运动路径目标点序列（一个或多个），可定义为全局变量或其他，和函数subWaypointFromGcsCallback中的thePathTargets是同一个对象！！！
		for (int i = 0; i<waypoint.point_size(); i++)
		{
			myTaskMessage::MyPoint tempWaypoint;
			tempWaypoint = waypoint.point(i);

			if (tempWaypoint.mark() != 0)//有目标点状态发生改变
			{
				for (int j = 0; j<thePathTargets.size(); j++)
				{
					if (abs(tempWaypoint.x() - thePathTargets[j].point.x) <= 0.01 &&
						abs(tempWaypoint.y() - thePathTargets[j].point.y) <= 0.01 &&
						abs(tempWaypoint.z() - thePathTargets[j].point.z) <= 0.01)
					{
						if (thePathTargets[j].markerID == 0)//该目标点当时状态为0，则接受修改（有其他无人系统先到达了某个目标点）
						{
							thePathTargets[j].markerID = tempWaypoint.mark();

							ROS_INFO("notify:someone(uav%d) reached the target:%d", tempWaypoint.mark(), j);

							break;
						}
					}
				}
			}
		}
	}
}

/*
*从其他无人机接收信息素图（在编队中似乎没用到）
*本机维护所有其他无人机的信息素图，每次接收到某一架无人机的信息素图时，遍历所维护的信息并更新它
*/
void subOtherMBCallback(simulation_m100::PubSubUavMsg other_uav_mb_msg)
{
	mapbuffer::MapBuffer other_mb;
	other_mb.ParseFromString(other_uav_mb_msg.playload);

    vector<mapbuffer::MapBuffer> swarmMapBuf;//本机维护的所有其他无人机的信息素图，应定义成全局变量或其他
	int i=0;
	for(i;i<swarmMapBuf.size();i++)
	{
		if(swarmMapBuf[i].usid() == other_mb.usid())//找到已有信息中的对应图，并更新之
		{
			swarmMapBuf[i] = other_mb;
			break;
		}
	}

	if(i >= swarmMapBuf.size())//所维护的信息中还未有该目标信息，则加入
	{
		swarmMapBuf.push_back(other_mb);
	}
}

/*
*从其他无人机接收编队形成信号或编队控制点信号
*/
void subOtherFmInfoCallback(simulation_m100::PubSubUavMsg other_uav_fm_info)
{
    formationmsg::FormationMsg other_fm_msg;
    other_fm_msg.Clear();
    other_fm_msg.ParseFromString(other_uav_fm_info.playload);

    if(!other_fm_msg.changeformation())//该值为0表示发送的编队形成信号
    {
        //ROS_INFO("I re")
        switch(other_fm_msg.uavid())
        {
            case 1:
            {
                if(other_fm_msg.formationflag() == true)
                    flag1 = true;
                break;
            }
            case 2:
            {
                if(other_fm_msg.formationflag() == true)
                    flag2 = true;
                break;
            }
            case 3:
            {
                if(other_fm_msg.formationflag() == true)
                    flag3 = true;
                break;
            }
            default:
                break;

        }
    }

    if(other_fm_msg.changeformation())//该值为1表示编队控制点信号
    {
         //ROS_INFO("for uav%d formation point : %f,%f,%f ",other_fm_msg.uavid(),other_fm_msg.formationpointx(),other_fm_msg.formationpointy(),other_fm_msg.formationpointz());
        if(other_fm_msg.uavid() == 9999)
        {
            ROS_INFO("get Over Message");
            formationover = true;
        }
        if(formationPoints.find(other_fm_msg.uavid()) == formationPoints.end())//formationPoints中不存在该点，则放入
        {
            Point3D tempPoint(other_fm_msg.formationpointx(),other_fm_msg.formationpointy(),other_fm_msg.formationpointz());
            FormationPoint tempFmPoint(other_fm_msg.uavid(),tempPoint);
            //ROS_INFO("recv formation %d point : %f,%f,%f ",other_fm_msg.uavid(),other_fm_msg.formationpointx(),other_fm_msg.formationpointy(),other_fm_msg.formationpointz());
            formationPoints.insert(make_pair(other_fm_msg.uavid(),tempFmPoint));

        }else//formationPoints中已存在该点，则更新
        {
            auto it = formationPoints.find(other_fm_msg.uavid());
            it->second.point.x = other_fm_msg.formationpointx();
            it->second.point.y = other_fm_msg.formationpointy();
            it->second.point.z = other_fm_msg.formationpointz();
            //ROS_INFO("recv formation %d point : %f,%f,%f ",other_fm_msg.uavid(),other_fm_msg.formationpointx(),other_fm_msg.formationpointy(),other_fm_msg.formationpointz());
            formationPoints[other_fm_msg.uavid()] =FormationPoint(other_fm_msg.uavid(),other_fm_msg.formationpointx(),other_fm_msg.formationpointy(),other_fm_msg.formationpointz());
        }
        
    }
}

string getFileName(const char* address_file_name)
{
    char abs_path[1024];
    int cnt = readlink("/proc/self/exe", abs_path, 1024);//获取可执行程序的绝对路径
    if(cnt < 0|| cnt >= 1024)
    {
        return NULL;
    }

    //最后一个'/' 后面是可执行程序名，去掉devel/lib/m100_v2/exe，只保留前面部分路径
    int count=0;
    for(int i = cnt; i >= 0; --i)
    {
        if(abs_path[i]=='/')
        {
            abs_path[i + 1]='\0';
            count++;

            if(count == 4)
            {
                break;
            }
        }
    }

    string path(abs_path);
    string name(address_file_name);

    name = path + "src/" + name;

    return name;
}

void initOtherUavId()
{
    int error_count = 0;
    while(ros::ok())
    {
        string name = getFileName((other_uav_id_config + uavName + ".txt").c_str());
        ifstream other_uav_id_str;
        other_uav_id_str.open(name.c_str(),ios::in);

        if(other_uav_id_str)
        {
            string str;
            while(getline(other_uav_id_str,str))
            {
                istringstream is(str);
                is>>other_uav_id[other_uav_count];
                ROS_INFO("uav%d ID initialize done.",other_uav_id[other_uav_count]);

                other_uav_count++;
            }

            other_uav_id_str.close();
            break;
        }
        else
        {
            printf("%s ",name.c_str());
            perror("open failed: ");

            if((error_count++) > 500)
            {
                printf("Can not get the uav_id!");
                break;
            }
        }
    }
}

int main(int argc,char * argv[])
{
    ros::init(argc, argv, "m100Formation");
	ROS_INFO("m100Formation_node started!");

	ros::NodeHandle nh;

	ros::param::get("~uav_id", uavID);
	if (uavID == -1)
	{
		return -1;
	}
	uav.setUavID(uavID);

	ros::param::get("~uav_name", uavName);

    ROS_INFO("uavID=%d,uavName=%s",uavID,uavName.c_str());

	ros::param::get("~axis_id", axisID);
    std::string posTopic("/global_position");//不同坐标系的定位数据的订阅话题："/global_position"为自然全局坐标系定位，"/local_position"为实验局部坐标系定位
	if (axisID == 2)
	{
		posTopic = string("/local_position");
		moveCtrlMode = 2;
	}

    ros::Subscriber cmd_result_sub = nh.subscribe(string(uavName + "/command_result").c_str(), 10, subCmdResultCallback);//订阅指令执行结果
    ros::Subscriber pos_sub = nh.subscribe(string(uavName + posTopic).c_str(), 10, subPosCallback);//本机定位
    ros::Subscriber other_uav_pos_sub = nh.subscribe(string(uavName + "/other_uav_pos").c_str(), 10, subOtherUavPosCallback);//其他无人系统定位
    ros::Subscriber waypoint_from_gcs_sub = nh.subscribe(string(uavName + "/formation_cmd").c_str(),10,&subWaypointFromGcsCallback);
    ros::Subscriber other_uav_feedback_waypoint_sub = nh.subscribe(string(uavName + "/other_uav_fd_waypoint").c_str(), 10, subOtherUavFdWPCallback);
    ros::Subscriber other_mb_sub = nh.subscribe(string(uavName + "/other_uav_mapbuffer").c_str(), 10, subOtherMBCallback);
    ros::Subscriber other_uav_fm_info_sub = nh.subscribe(string(uavName + "/other_uav_formation_info").c_str(), 10, subOtherFmInfoCallback);
    
    formation_info_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uavName + "/my_formation_info").c_str(), 10);
    move_cmd_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uavName + "/move_cmd").c_str(), 10);//移动控制指令

    initOtherUavId();

    //experiment();
    pthread_t t1;
    pthread_create(&t1,NULL,experiment,NULL);

    ros::spin();

    ros::shutdown();

    return 0;
}