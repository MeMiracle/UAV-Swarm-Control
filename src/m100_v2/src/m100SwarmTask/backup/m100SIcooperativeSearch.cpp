#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ros/ros.h>
#include <sensor_msgs/NavSatFix.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <m100Location/uwblpsmessage.pb.h>
#include <m100Location/uwblpsmessage.pb.cc>
#include <taskmessage.pb.h>
#include <taskmessage.pb.cc>
#include <gcscommand.pb.h>
#include <gcscommand.pb.cc>
#include <mapbuffer.pb.h>
#include <mapbuffer.pb.cc>
#include <m100_v2/PubSubUavMsg.h>
#include <m100_v2/Position.h>
#include <m100_v2/CmdResult.h>

using namespace std;

#define VEHICLE_DIAMETER 1.0//无人系统直径
#define VEHICLE_RADIUS 0.5 //无人系统半径
#define SWARM_RADIUS 3//集群半径
#define ZOOM_FACTOR 0.5 //10*10 0.5 or 5*5 0.25
#define MAXLENGTH 20//场地边长
#define SPEED (float)0.4//速度
#define STEP_TIME 0.4//一次步进时间
#define C_PI (double)3.141592653589793
#define C_EARTH (double)6378137.0
#define MAPLENGTH 20//图长
#define DECAYFACTOR 0.995//衰减因子
#define MAP_FACTOR 1//图长除以边长
#define STEPCOUNT 500//运动总步长
#define MAP_BUFFER 9//交换地图长度

int uavID = -1;
string uavName;

int axisID = 1;
int moveCtrlMode = 1;//速度控制模式：0为机身速度控制，1为自然全局坐标系内速度控制，2为实验局部坐标系内速度控制
std::string posTopic("/global_position");//不同坐标系的定位数据的订阅话题："/global_position"为自然全局坐标系定位，"/local_position"为实验局部坐标系定位

const float deg2rad = C_PI / 180.0;
const float rad2deg = 180.0 / C_PI;

string component = "m100SIcooperativeSearch";
int swarm_intel_priority = 95;

ros::Publisher move_cmd_pub;
ros::Publisher feedback_waypoint_pub;
ros::Publisher mapbuffer_pub;

string takeoff_cmd = "takeoff_cmd";
string land_cmd = "land_cmd";
string gohome_cmd = "gohome_cmd";
string move_cmd = "move_cmd";
string hover_cmd = "hover_cmd";
string waypoint_cmd = "waypoint_cmd";

bool cmd_status = true;//命令执行状态反馈

struct Point3D {
	/*
	* 记录位置点
	*/
	float x;
	float y;
	float z;
	Point3D() {
		x = 0;
		y = 0;
		z = 0;
	}
	Point3D(float tx, float ty) {
		x = tx;
		y = ty;
		z = 0;
	}
	Point3D(float tx, float ty, float tz) {
		x = tx;
		y = ty;
		z = tz;
	}
	bool operator==(const Point3D& p)
	{
		if (this->x == p.x)
			return this->y == p.y;
		else
			return false;
	}

	bool operator<(const Point3D& p)
	{
		if (this->x < p.x)
			return this->y<p.y;
		else return (this->x + this->y) < (p.x + p.y);
	}

	void operator=(const Point3D& p)
	{
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
	}

	double operator-(const Point3D &other) const {
		auto x_ = abs(this->x - other.x);
		auto y_ = abs(this->y - other.y);
		auto z_ = abs(this->z - other.z);
		//return sqrt(pow(x_, 2) + pow(y_, 2) + pow(z_, 2));
		return sqrt(pow(x_, 2) + pow(y_, 2));
	}

};


enum direction {
	left, left_up, up, right_up, right, right_down, down, left_down, here
};

class Target {
public:
	Point3D point;
	int markerID;

	Target(float x, float y, float z, int id) {
		point.x = x;
		point.y = y;
		point.z = z;
		markerID = id;
	}
	Target() {

	}
	~Target() {

	};
};


class vehicle {
private:
	Point3D position;
	int ID;
	float speed;
	float x_speed;
	float y_speed;
	direction olddirection;//前一步运动方向
	

public:
	vehicle(float x, float y, float z, int id) {
		position.x = x;
		position.y = y;
		position.z = z;
		ID = id;

		speed = SPEED;//方向合速度
		x_speed = speed * sin(45 * deg2rad);//x轴分速度
		y_speed = speed * cos(45 * deg2rad);//y轴分速度

		olddirection = here;

	}

	void moveTowardDir(direction adjDir, int moveCtrlMode)
	{
		ROS_INFO("uav%d move to %d", ID, adjDir);

		gcsCommand::Twist twist;
		twist.Clear();

		switch (adjDir)
		{
		case (direction)0://left
			twist.set_linear_x(-speed);
			break;
		case (direction)1://left_up
			twist.set_linear_x(-x_speed);
			twist.set_linear_y(y_speed);
			break;
		case (direction)2://up
			twist.set_linear_y(speed);
			break;
		case (direction)3://right_up
			twist.set_linear_x(x_speed);
			twist.set_linear_y(y_speed);
			break;
		case (direction)4://right
			twist.set_linear_x(speed);
			break;
		case (direction)5://right_down
			twist.set_linear_x(x_speed);
			twist.set_linear_y(-y_speed);
			break;
		case (direction)6://down
			twist.set_linear_y(-speed);
			break;
		case (direction)7://left_down
			twist.set_linear_x(-x_speed);
			twist.set_linear_y(-y_speed);
			break;
		default:
			break;
		}

		twist.set_angular_x(moveCtrlMode);

		m100_v2::PubSubUavMsg move_cmd_msg;
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
		}
	}

	void stopUav() {
		gcsCommand::Twist twist;
		twist.Clear();

		twist.set_angular_x(1);//对地速度标志位

		m100_v2::PubSubUavMsg move_cmd_msg;
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
		}
	}

	Point3D getPosition() {
		return position;
	}

	void setPosition(Point3D point)
	{
		position = point;
	}

	int getID() {
		return ID;
	}

	void setID(int id) {
		ID = id;
	}
	
	direction getoldD() {
		return olddirection;
	}
	void setoldD(direction d) {
		olddirection = d;
	}

};

// class TranslateMapBuffer {
// public:
// 	vector<float> mapbuffer;//用以存储传输的信息素图
// 	int mapbufferlength;//传输的信息素图长度，根据需要值可能为9，13，25，41等等，由宏定义指定
// 	int seedi;//发送信息的无人系统所在网格横坐标
// 	int seedj;//发送信息的无人系统所在网格纵坐标
// 	int usid;//发送消息的无人系统id
// };

vehicle uav(0, 0, 0, uavID);

float _map[MAPLENGTH][MAPLENGTH];//信息素图

int validitycheck(int i) {
	if (i > MAPLENGTH - 1)
		i = int(MAPLENGTH - 1);
	else if (i < 0)
		i = 0;
	return i;
}

void updatemap() {//无人系统移动后更新信息素图
	for (int i = 0; i < MAPLENGTH; i++)
		for (int j = 0; j < MAPLENGTH; j++)
		{
			if (_map[i][j] > 0.1) {
				_map[i][j] *= (float)DECAYFACTOR;
				if (_map[i][j] < 0.1)
					_map[i][j] = 0.1;//衰减最小为0.1
			}
		}
	int mapi = validitycheck(int(uav.getPosition().x * MAP_FACTOR));
	int mapj = validitycheck(int(uav.getPosition().y * MAP_FACTOR));
	_map[mapi][mapj] = 1;
}

void updatemap2(vector<float> mapbuffer, int i, int j) {
	if (MAP_BUFFER == 9)
	{
		if (mapbuffer[0] > _map[i - 1][j + 1])
			_map[i - 1][j + 1] = mapbuffer[0];
		if (mapbuffer[1] > _map[i][j + 1])
			_map[i][j + 1] = mapbuffer[1];
		if (mapbuffer[2] > _map[i + 1][j + 1])
			_map[i + 1][j + 1] = mapbuffer[2];
		if (mapbuffer[3] > _map[i - 1][j])
			_map[i - 1][j] = mapbuffer[3];
		if (mapbuffer[4] > _map[i][j])
				_map[i][j] = mapbuffer[4];
		if (mapbuffer[5] > _map[i + 1][j])
			_map[i + 1][j] = mapbuffer[5];
		if (mapbuffer[6] > _map[i - 1][j - 1])
			_map[i - 1][j - 1] = mapbuffer[6];
		if (mapbuffer[7] > _map[i][j - 1])
			_map[i][j - 1] = mapbuffer[7];
		if (mapbuffer[8] > _map[i + 1][j - 1])
			_map[i + 1][j - 1] = mapbuffer[8];
	}
}

float getmapV(int i, int j) {
	return _map[i][j];
}

vector<mapbuffer::MapBuffer> swarmMapBuf;
vector<Point3D> swarmPositionBuf;
vector<int> uavIndexBuf;
vector<ros::Time> posUpdateTime;

pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER;

vector<Target> targetBuf;
pthread_mutex_t target_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mb_mutex = PTHREAD_MUTEX_INITIALIZER;

void subCmdResultCallback(m100_v2::CmdResult result)
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

void subPosCallback(m100_v2::Position pos_msg)//本机位置更新
{
	Point3D my_point(pos_msg.x, pos_msg.y, pos_msg.z);

	uav.setPosition(my_point);
}

void subOtherUavPosCallback(m100_v2::PubSubUavMsg other_uav_pos_msg)//实时更新其他无人系统位置
{
	uwblpsMessage::uwblpsMessage other_uav_pos;
	other_uav_pos.ParseFromString(other_uav_pos_msg.playload);

	Point3D point(other_uav_pos.linear_x(), other_uav_pos.linear_y(), other_uav_pos.linear_z());

	int src_uav_id = other_uav_pos_msg.src_uav_id;


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

//获取待发送到其他无人系统的信息图
vector<float> getMapBuffer(vehicle us)
{
	vector<float> mapbuffer(MAP_BUFFER, 0);
	if (MAP_BUFFER == 9)
	{
		int seedi = int(us.getPosition().x*MAP_FACTOR);
		int seedj = int(us.getPosition().y*MAP_FACTOR);
		int i = seedi - 1;
		int j = seedj + 1;
		if (i<0 || j>MAPLENGTH - 1)
			mapbuffer[0] = 0;
		else
			mapbuffer[0] = getmapV(i, j);

		if (j > MAPLENGTH - 1)
		{
			mapbuffer[1] = 0;
		}
		else
			mapbuffer[1] = getmapV(i + 1, j);
		if (i < 0)
			mapbuffer[3] = 0;
		else
			mapbuffer[3] = getmapV(i, j - 1);
		i = seedi + 1;
		if (i > MAPLENGTH - 1 || j > MAPLENGTH - 1)
			mapbuffer[2] = 0;
		else
			mapbuffer[2] = getmapV(i, j);
		if (i > MAPLENGTH - 1)
			mapbuffer[5] = 0;
		else
			mapbuffer[5] = getmapV(i, j - 1);
		j = seedj - 1;
		if (i > MAPLENGTH - 1 || j < 0)
			mapbuffer[8] = 0;
		else
			mapbuffer[8] = getmapV(i, j);
		if (j < 0)
			mapbuffer[7] = 0;
		else
			mapbuffer[7] = getmapV(i - 1, j);
		i = seedi - 1;
		if (i <0 || j < 0)
			mapbuffer[6] = 0;
		else
			mapbuffer[6] = getmapV(i, j);
		mapbuffer[4] = getmapV(seedi, seedj);
	}
	return mapbuffer;
}

void subOtherMBCallback(m100_v2::PubSubUavMsg other_uav_mb_msg)
{
	mapbuffer::MapBuffer other_mb;
	other_mb.ParseFromString(other_uav_mb_msg.playload);

	// for(int i=0;i<other_mb.mb_item_size();i++)
	// {
	// 	ROS_INFO("recv:%d__%f",i,other_mb.mb_item(i));
	// }

	pthread_mutex_lock(&mb_mutex);

	int i=0;
	for(i;i<swarmMapBuf.size();i++)
	{
		if(swarmMapBuf[i].usid() == other_mb.usid())
		{
			swarmMapBuf[i] = other_mb;
			break;
		}
	}

	if(i >= swarmMapBuf.size())
	{
		swarmMapBuf.push_back(other_mb);
	}

	pthread_mutex_unlock(&mb_mutex);
}

//根据获取集群其他无人系统信息图更新自身的信息素图
void updateMapBuffer(std::vector<mapbuffer::MapBuffer> &swarmMap)
{
	pthread_mutex_lock(&mb_mutex);
	swarmMap = swarmMapBuf;
	pthread_mutex_unlock(&mb_mutex);

	for (int i = 0; i < swarmMap.size(); i++)//swarmMap
	{
		// if (swarmMap[i].usid != 0)//usid不为0视为有更新////
		// {
		// 	updatemap2(swarmMap[i].mapbuffer, swarmMap[i].seedi, swarmMap[i].seedj);//根据某个无人系统发送的信息素图更新自身的信息素图
		// 	swarmMap[i].usid = 0;//更新后将usid置0////不能修改usid，因为维护更新swarmMapBuf时要用到usid
		// }

		if (swarmMap[i].length() != 0)//length不为0视为有更新
		{
			vector<float> mb;
			for(int j=0;j<swarmMap[i].mb_item_size();j++)
			{
				mb.push_back(swarmMap[i].mb_item(j));
			}

			updatemap2(mb, swarmMap[i].seedi(), swarmMap[i].seedj());//根据某个无人系统发送的信息素图更新自身的信息素图
			swarmMap[i].set_length(0);//更新后将length置0
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

int lastTargetIndex = -1;
void subSwarmWaypointCallback(m100_v2::PubSubUavMsg target_msg)
{
	myTaskMessage::TaskMessage target_waypoints;
	target_waypoints.ParseFromString(target_msg.playload);

	//ROS_INFO("received waypoints!");
	if (target_msg.src_uav_id == 0)//初始化目标点（初次从地面接收到目标点）
	{
		pthread_mutex_lock(&target_mutex);

		targetBuf.clear();
		lastTargetIndex = -1;

		for (int i = 0; i<target_waypoints.point_size(); i++)
		{
			myTaskMessage::MyPoint waypoint;
			waypoint = target_waypoints.point(i);

			Target target(waypoint.x(), waypoint.y(), waypoint.z(), waypoint.mark());

			targetBuf.push_back(target);

			//ROS_INFO("target:%f,%f,%f,%d", waypoint.x(), waypoint.y(), waypoint.z(), waypoint.mark());
		}

		cmd_status = true;

		pthread_mutex_unlock(&target_mutex);
	}
}

void subOtherUavFdWPCallback(m100_v2::PubSubUavMsg other_uav_fd_wp_msg)//实时更新目标航点到达状态
{
	myTaskMessage::TaskMessage target_waypoints;
	target_waypoints.ParseFromString(other_uav_fd_wp_msg.playload);

	//ROS_INFO("received waypoints!");
	if (other_uav_fd_wp_msg.src_uav_id != 0)//更新目标点（从其他无人机接收到目标点）
	{
		//ROS_INFO("received a notification!");
		for (int i = 0; i<target_waypoints.point_size(); i++)
		{
			myTaskMessage::MyPoint waypoint;
			waypoint = target_waypoints.point(i);

			if (waypoint.mark() != 0)//有目标点状态发生改变
			{
				pthread_mutex_lock(&target_mutex);
				for (int j = 0; j<targetBuf.size(); j++)
				{
					if (abs(waypoint.x() - targetBuf[j].point.x) <= 0.01 &&
						abs(waypoint.y() - targetBuf[j].point.y) <= 0.01 &&
						abs(waypoint.z() - targetBuf[j].point.z) <= 0.01)
					{
						if (targetBuf[j].markerID == uav.getID())//该点是自己的目标点，则接受修改（有其他无人系统先到达了某个目标点）
						{
							targetBuf[j].markerID = waypoint.mark();

							ROS_INFO("notify:someone(uav%d) reached the target:%d", -waypoint.mark(), j+1);//该点被到达后，mark为到达该点的无人机的id负值

							break;
						}
					}
				}
				pthread_mutex_unlock(&target_mutex);
			}
		}
	}
}

//更新各目标点状态
void updateTarget(vector<Target> &target)
{
	pthread_mutex_lock(&target_mutex);
	target = targetBuf;
	pthread_mutex_unlock(&target_mutex);
}

//本无人系统先到达某个目标点，则发出通告
void tryToDeclareTarget(vector<Target> target, int targetIndex)
{
	bool reachFirst = false;

	pthread_mutex_lock(&target_mutex);
	if (targetBuf[targetIndex].markerID == uav.getID());
	{
		targetBuf[targetIndex].markerID = -uav.getID();//到达该点后，将该点的markID设为无人机id的负值，表示“已到达”
		target[targetIndex].markerID = -uav.getID();

		reachFirst = true;
	}
	pthread_mutex_unlock(&target_mutex);

	if (reachFirst)//本机先到达目标点，则广播一次该位置已经到达
	{
		ROS_INFO("I(uav%d) reached target:%d\n", uav.getID(), targetIndex+1);

		myTaskMessage::TaskMessage task_waypoint;
		myTaskMessage::MyPoint* mp;

		mp = task_waypoint.add_point();
		mp->set_x(target[targetIndex].point.x);
		mp->set_y(target[targetIndex].point.y);
		mp->set_z(target[targetIndex].point.z);
		//mp->set_yaw(yawdist);
		mp->set_type(2);
		mp->set_mark(target[targetIndex].markerID);

		//task_waypoint.set_clusterid();
		task_waypoint.set_kind(2);

		m100_v2::PubSubUavMsg task_waypoint_str;

		if (!task_waypoint.SerializeToString(&task_waypoint_str.playload))
		{
			perror("serialize error:");
		}
		else
		{
			feedback_waypoint_pub.publish(task_waypoint_str);
			//ROS_INFO("broadcast target %d",targetIndex+1);
		}
	}
	else//其他无人机先到达了目标点
	{
		ROS_INFO("others(uav%d) a little earlier reached target:%d\n", -target[targetIndex].markerID, targetIndex+1);
	}
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

int JudgeEdge(float rx1, float rx2, float ry1, float ry2, Point3D point, int clock)//clock==1顺时针，clock==2逆时针
{
	return 0;
	Point3D Bedge;
	Bedge.z = point.z;
	//左边
	if ((point.y <= ry2*MAXLENGTH) && (point.y >= ry1*MAXLENGTH))
	{

		Bedge.x = rx1*MAXLENGTH;
		Bedge.y = point.y;
		if (Bedge - point <= VEHICLE_RADIUS)
			return clock;
		//右边
		Bedge.x = rx2*MAXLENGTH;
		if (point - Bedge <= VEHICLE_RADIUS)
			return clock;

	}
	//下边
	if ((point.x <= rx2*MAXLENGTH) && (point.x >= rx1*MAXLENGTH))
	{
		Bedge.y = ry1*MAXLENGTH;
		Bedge.x = point.x;
		if (Bedge - point <= VEHICLE_RADIUS)
			return clock;
		//上边
		Bedge.y = ry2*MAXLENGTH;
		if (point - Bedge <= VEHICLE_RADIUS)
			return clock;
	}

	//四角
	Bedge.x = rx1*MAXLENGTH;
	Bedge.y = ry1*MAXLENGTH;
	if (Bedge - point <= VEHICLE_RADIUS) {
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

int JudgeBarrier(Point3D point)
{

	//第一个障碍物
	if (JudgeEdge(0.8, 0.9, 0.2, 0.3, point, 1))
		return JudgeEdge(0.8, 0.9, 0.2, 0.3, point, 1);

	//第二个障碍物
	if (JudgeEdge(0.6, 0.7, 0.4, 0.5, point, 1))
		return JudgeEdge(0.6, 0.7, 0.4, 0.5, point, 1);

	//第三个障碍物
	if (JudgeEdge(0.2, 0.6, 0.65, 0.75, point, 1))
		return JudgeEdge(0.2, 0.6, 0.65, 0.75, point, 1);

	//第四个障碍物
	if (JudgeEdge(0.2, 0.6, 0.9, 1, point, 2))
		return JudgeEdge(0.2, 0.6, 0.9, 1, point, 2);

	return false;
}


direction GetDirection(float speed, Point3D nowPosition, direction& moveDirection, const vector<Point3D>& swarmPosition)
/*
*获得无障碍物运动方向
*/
{
	int barrierflg = 0;
	int swarmflg = 0;
	int count = 0;
	Point3D attemptPosition;
	int flg = 1;
	while (flg && true == cmd_status)
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
		barrierflg = JudgeBarrier(attemptPosition);
		if (!barrierflg)
		{
			swarmflg=0;
			for (auto it = swarmPosition.begin(); it != swarmPosition.end(); it++)
			{
				if (abs(attemptPosition - (*it)) < 1.2*VEHICLE_DIAMETER)
				//if (abs(nowPosition - (*it)) < 1.3*VEHICLE_DIAMETER)
					swarmflg = 1;
			}
			if (!swarmflg)
				return moveDirection;//无障碍物
		}
		if (swarmflg == 1)
		{
			ROS_INFO("send:%d",moveDirection);
			moveDirection = (direction)((moveDirection + 1) % 8);
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
	}
	moveDirection = here;
	return moveDirection;//有障碍物
}

bool isApproimate(Point3D p1, Point3D p2)
{
	if ((p1.x - p2.x) <= 0.7*VEHICLE_DIAMETER && (p1.y - p2.y) <= 0.7*VEHICLE_DIAMETER)
	{
		return ((p1.x - p2.x) >= -0.7*VEHICLE_DIAMETER) && ((p1.y - p2.y) >= -0.7*VEHICLE_DIAMETER);
	}
	else
		return false;
}

direction calculateDirection(Point3D targetPosition, Point3D nowPosition)
/*
* 计算运动方向
*/
{
	if (targetPosition.x - nowPosition.x >= 0.5*VEHICLE_DIAMETER)
	{
		if (targetPosition.y - nowPosition.y >= 0.5*VEHICLE_DIAMETER)
			return right_up;
		else if (targetPosition.y - nowPosition.y <= -0.5*VEHICLE_DIAMETER)
			return right_down;
		else
			return direction::right;
	}
	else if (targetPosition.x - nowPosition.x <= -0.5*VEHICLE_DIAMETER)
	{
		if (targetPosition.y - nowPosition.y >= 0.5*VEHICLE_DIAMETER)
			return left_up;
		else if (targetPosition.y - nowPosition.y <= -0.5*VEHICLE_DIAMETER)
			return left_down;
		else return direction::left;
	}
	else if (targetPosition.y - nowPosition.y >= 0.5*VEHICLE_DIAMETER)
		return up;
	else if (targetPosition.y - nowPosition.y <= -0.5*VEHICLE_DIAMETER)
		return down;
	else return direction(int(7 * (rand() / (RAND_MAX + 1.0))));
}


direction GetDirection(vehicle &us, Point3D nowPosition, const vector<Point3D>& swarmPosition)
/*
*获得协同搜索运动方向
*/
{
	int mapi = int(nowPosition.x * MAP_FACTOR);
	int mapj = int(nowPosition.y * MAP_FACTOR);
	mapi = validitycheck(mapi);
	mapj = validitycheck(mapj);
	int i = mapi, j = mapj;//一定要整型

	direction moveDirection;
	if (us.getoldD() == here) {
		int d = 7 * (rand() / (RAND_MAX + 1.0));//上一个时刻无运动方向，随机选一个
		moveDirection = (direction)d;
		bool bf1 = true;
		while (bf1 && true == cmd_status) {
			//碰撞检测
			moveDirection = GetDirection(SPEED, us.getPosition(), moveDirection, swarmPosition);
			//边界处理
			i = mapi, j = mapj;
			switch (moveDirection)
			{
			case (direction)0://left
				i = mapi - 1;
				break;
			case (direction)1://left_up
				i = mapi - 1;
				j = mapj + 1;
				break;
			case (direction)2://up
				j = mapj + 1;
				break;
			case (direction)3://right_up
				i = mapi + 1;
				j = mapj + 1;
				break;
			case (direction)4://right
				i = mapi + 1;
				break;
			case (direction)5://right_down
				i = mapi + 1;
				j = mapj - 1;
				break;
			case (direction)6://down
				j = mapj - 1;
				break;
			case (direction)7://left_down
				i = mapi - 1;
				j = mapj - 1;
				break;
			default:
				break;
			}
			if (i > (MAPLENGTH - 1) && j > (MAPLENGTH - 1))
			{
				moveDirection = direction(7);
				bf1 = true;
			}
			else if (i > (MAPLENGTH - 1) && j < 0)
			{
				moveDirection = direction(1);
				bf1 = true;
			}
			else if (i < 0 && j >(MAPLENGTH - 1))
			{
				moveDirection = direction(5);
				bf1 = true;
			}
			else if (i >(MAPLENGTH - 1))
			{
				if (moveDirection == direction(5))
					moveDirection = direction(6);
				else if (moveDirection == direction(3))
					moveDirection = direction(2);
				else
					moveDirection = direction(0);
				bf1 = true;
			}
			else if (j > (MAPLENGTH - 1))
			{
				if (moveDirection == direction(1))
					moveDirection = direction(0);
				else if (moveDirection == direction(3))
					moveDirection = direction(4);
				else
					moveDirection = direction(6);
				bf1 = true;
			}
			else if (i < 0 && j < 0)
			{
				moveDirection = direction(3);
				bf1 = true;
			}
			else if (i < 0)
			{
				if (moveDirection == direction(7))
					moveDirection = direction(6);
				else if (moveDirection == direction(1))
					moveDirection = direction(2);
				else
					moveDirection = direction(4);
				bf1 = true;
			}
			else if (j < 0)
			{
				if (moveDirection == direction(7))
					moveDirection = direction(0);
				else if (moveDirection == direction(5))
					moveDirection = direction(4);
				else
					moveDirection = direction(2);
				bf1 = true;
			}
			else
				bf1 = false;
		}
		us.setoldD((direction)d);
		return moveDirection;
	}
	else {
		float r = (rand() / (RAND_MAX + 1.0));
		if (r > 0.9)
			moveDirection = direction((us.getoldD() + 1) % 8);//有一定概率右转
		else if (r < 0.1)
			moveDirection = direction((us.getoldD() + 7) % 8);//有一定概率左转
		else
			moveDirection = us.getoldD();//直走
	}

	int count = 0;
	int flg = 1;


	while (flg && true == cmd_status)//候选运动方向前进的网格是否未搜索
	{
		switch (moveDirection)
		{
		case (direction)0://left
			i = mapi - 1;
			break;
		case (direction)1://left_up
			i = mapi - 1;
			j = mapj - 1;
			break;
		case (direction)2://up
			j = mapj + 1;
			break;
		case (direction)3://right_up
			i = mapi + 1;
			j = mapj + 1;
			break;
		case (direction)4://right
			i = mapi + 1;
			break;
		case (direction)5://right_down
			i = mapi + 1;
			j = mapj - 1;
			break;
		case (direction)6://down
			j = mapj - 1;
			break;
		case (direction)7://left_down
			i = mapi - 1;
			j = mapj - 1;
			break;
		default:
			break;
		}

		if (i >= 0 && i<MAPLENGTH && j >= 0 && j<MAPLENGTH)
		{
			if ((flg == 1) && (getmapV((int)i, (int)j) >= 0.1)) {
				moveDirection = direction((moveDirection + 1) % 8);
				count++;
				if (count == 9) {
					flg = 0;
				}
			}
			else
				flg = 0;
		}
		else
			flg = 0;
	}
	bool bf1 = true;
	while (bf1 && true == cmd_status) {
		//判断障碍物
		moveDirection = GetDirection(SPEED, uav.getPosition(), moveDirection, swarmPosition);
		//边界处理
		i = mapi, j = mapj;
		switch (moveDirection)
		{
		case (direction)0://left
			i = mapi - 1;
			break;
		case (direction)1://left_up
			i = mapi - 1;
			j = mapj + 1;
			break;
		case (direction)2://up
			j = mapj + 1;
			break;
		case (direction)3://right_up
			i = mapi + 1;
			j = mapj + 1;
			break;
		case (direction)4://right
			i = mapi + 1;
			break;
		case (direction)5://right_down
			i = mapi + 1;
			j = mapj - 1;
			break;
		case (direction)6://down
			j = mapj - 1;
			break;
		case (direction)7://left_down
			i = mapi - 1;
			j = mapj - 1;
			break;
		default:
			break;
		}
		if (i > (MAPLENGTH - 1) && j > (MAPLENGTH - 1))
		{
			moveDirection = direction(7);
			bf1 = true;
		}
		else if (i > (MAPLENGTH - 1) && j < 0)
		{
			moveDirection = direction(1);
			bf1 = true;
		}
		else if (i < 0 && j >(MAPLENGTH - 1))
		{
			moveDirection = direction(5);
			bf1 = true;
		}
		else if (i >(MAPLENGTH - 1))
		{
			if (moveDirection == direction(5))
				moveDirection = direction(6);
			else if (moveDirection == direction(3))
				moveDirection = direction(2);
			else
				moveDirection = direction(0);
			bf1 = true;
		}
		else if (j > (MAPLENGTH - 1)) {
			if (moveDirection == direction(1))
				moveDirection = direction(0);
			else if (moveDirection == direction(3))
				moveDirection = direction(4);
			else
				moveDirection = direction(6);
			bf1 = true;
		}
		else if (i < 0 && j < 0)
		{
			moveDirection = direction(3);
			bf1 = true;
		}
		else if (i < 0)
		{
			if (moveDirection == direction(7))
				moveDirection = direction(6);
			else if (moveDirection == direction(1))
			{
				moveDirection = direction(2);
			}
			else
				moveDirection = direction(4);
			bf1 = true;
		}
		else if (j < 0)
		{
			if (moveDirection == direction(7))
			{
				moveDirection = direction(0);
			}
			else if (moveDirection == direction(5))
			{
				moveDirection = direction(4);
			}
			else
				moveDirection = direction(2);
			bf1 = true;
		}
		else {
			bf1 = false;
		}

	}
	us.setoldD(moveDirection);
	return moveDirection;
}

void sendMapBuffer(vector<float> &MB,int seedi,int seedj,int mapbufferlength,int usid)
{
	mapbuffer::MapBuffer MB_msg;
	vector<float>::iterator it;

	for(it=MB.begin();it!=MB.end();it++)
	{
		MB_msg.add_mb_item(*it);
	}

	// for(int i=0;i<MB_msg.mb_item_size();i++)
	// {
	// 	ROS_INFO("send:%d__%f",i,MB_msg.mb_item(i));
	// }

	MB_msg.set_seedi(seedi);
	MB_msg.set_seedj(seedj);
	MB_msg.set_length(mapbufferlength);
	MB_msg.set_usid(usid);

	m100_v2::PubSubUavMsg ps_msg;
	MB_msg.SerializeToString(&ps_msg.playload);

	mapbuffer_pub.publish(ps_msg);
}

void* Cooperative_search(void* args)
{
	while (ros::ok())
	{
		ROS_INFO("targetBuf.size=%d uav.height=%f", targetBuf.size(), uav.getPosition().z);
		while (targetBuf.size() == 0 || uav.getPosition().z <= 0)
		{
			//sleep(0.1);
			ros::Duration(0.5).sleep();
		}

		ROS_INFO("cooperative search starting!");

		ofstream recordPosition;
		ofstream recordDirection;
		ofstream recordMap;

		char posFileName[] = "m100_v2/log/recordPosition";
		string pos_file(getDirectory() + posFileName + uavName.c_str() + ".txt");

		char dirFileName[] = "m100_v2/log/recordDirection";
		string dir_file(getDirectory() + dirFileName + uavName.c_str() + ".txt");

		char mapFileName[] = "m100_v2/log/recordMap";
		string map_file(getDirectory() + mapFileName + uavName.c_str() + ".txt");

		recordPosition.open(pos_file.c_str(), ios::app);
		recordDirection.open(dir_file.c_str(), ios::app);
		recordMap.open(map_file.c_str(), ios::app);

		recordPosition << ros::Time::now() << endl;
		recordDirection << ros::Time::now() << endl;
		recordMap << ros::Time::now() << endl;

		vector<Target> target;

		vector<Point3D> swarmPosition;
		vector<mapbuffer::MapBuffer> swarmMap;

		Point3D myPosition = { 0,0,0 };
		direction moveDirection;
		Point3D targetPoint;
		int targetIndex = -1;
		float speed = float(SPEED); //无人系统速度，单位米每步
		float swarm_radius = SWARM_RADIUS;
		bool ifFinishAll = false;
		int count = 1;
		bool new_target = true;
		bool targetflg = false;//是否到达目标点

		//抵达相应的目标位置
		while (false == ifFinishAll && true == cmd_status)//监听条件：所有目标点是否完成&任务是否被中断
		{
			updateTarget(target);

			for(int i=0;i<target.size();i++)
			{
				if(target[i].markerID == uav.getID())
				{
					targetPoint = target[i].point;
					targetIndex = i;

					if (lastTargetIndex != targetIndex)//开始一个新的目标点
					{
						lastTargetIndex = targetIndex;
						new_target = true;
					}

					break;
				}
			}

			if(targetIndex < 0)
			{
				ROS_INFO("NO target for me!");
				break;
			}

			if (count == STEPCOUNT)//已到达指定运动步长，则停止 
			{
				direction stopHere = direction::here;
				uav.moveTowardDir(stopHere, moveCtrlMode);

				ROS_INFO("all steps finished!\n");

				ifFinishAll = true;
				break;
			}

			myPosition = uav.getPosition();
			//读取其他无人系统位置
			getSwarmPosition(swarmPosition);


			if (true == new_target)
			{
				ROS_INFO("start a new target: %d", targetIndex+1);
				ROS_INFO("myPosition:%f,%f,%f", myPosition.x, myPosition.y, myPosition.z);
				ROS_INFO("targetPoint:%f,%f,%f", targetPoint.x, targetPoint.y, targetPoint.z);
				ROS_INFO("moving to the target %d...", targetIndex+1);

				new_target = false;
			}
			getSwarmPosition(swarmPosition);

			if (!targetflg) {
				if (!isApproimate(targetPoint, myPosition)) {
					moveDirection = calculateDirection(targetPoint, myPosition);
					moveDirection = GetDirection(SPEED, myPosition, moveDirection, swarmPosition);
				}
				else {
					//更新目标位置序列
					updateTarget(target);

					if (target[targetIndex].markerID == uav.getID())//本机到达目标点，则广播一次该位置已经到达
					{
						//标记并广播目标位置序列
						tryToDeclareTarget(target, targetIndex);
					}

					count = 0;
					targetflg = true;
				}

			}
			if (targetflg) {//不需要抵达目标地，执行搜索
				moveDirection = GetDirection(uav, myPosition, swarmPosition);
				//ROS_INFO("myPosition:%f,%f,%f", myPosition.x, myPosition.y, myPosition.z);

				if (target[targetIndex].markerID == uav.getID())//有新的目标点到来///////////////////////
				{
					targetflg = false;
				}
			}

			updatemap();

			vector<float> MB = getMapBuffer(uav);
			int seedi = int(uav.getPosition().x*MAP_FACTOR);
			int seedj = int(uav.getPosition().y*MAP_FACTOR);
			int mapbufferlength = MAP_BUFFER;
			int usid = uav.getID();

			sendMapBuffer(MB,seedi,seedj,mapbufferlength,usid);//发送自己的mapbuffer到其他无人系统，函数未定义，参数列表可修改，内容为上面5变量
			updateMapBuffer(swarmMap);//根据其他无人系统位置更新信息素图，待测试

			uav.moveTowardDir(moveDirection, moveCtrlMode);

			ros::Time start_time = ros::Time::now();
			while (ros::ok() && true == cmd_status)
			{
				ros::Duration(STEP_TIME).sleep();
				if (ros::Time::now() - start_time > ros::Duration(STEP_TIME))
				{
					break;
				}
			}

			recordDirection << moveDirection << endl;
			recordPosition << myPosition.x << " " << myPosition.y << endl;
			count++;
		}

		for (int i = 0; i < MAPLENGTH; i++)
		{
			for (int j = 0; j < MAPLENGTH; j++)
			{
				recordMap << getmapV(i, j) << " ";
			}
			recordMap << endl;
		}
		
		uavIndexBuf.clear();
		swarmPositionBuf.clear();
		targetBuf.clear();
		swarmMapBuf.clear();

		recordDirection.close();
		recordPosition.close();
		recordMap.close();
	}
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "m100SIcooperativeSearch");
	ROS_INFO("m100SIcooperativeSearch_node started!");

	ros::NodeHandle nh;

	ros::param::get("~uav_id", uavID);
	if (uavID == -1)
	{
		return -1;
	}
	uav.setID(uavID);

	ros::param::get("~uav_name", uavName);

	ros::param::get("~axis_id", axisID);
	if (axisID == 2)
	{
		posTopic = string("/local_position");
		moveCtrlMode = 2;
	}


	ros::Subscriber swarm_waypoint_sub = nh.subscribe(string(uavName + "/swarm_waypoint").c_str(), 10, subSwarmWaypointCallback);//群体智能目标航点
	ros::Subscriber pos_sub = nh.subscribe(string(uavName + posTopic).c_str(), 10, subPosCallback);//本机定位
	ros::Subscriber other_uav_pos_sub = nh.subscribe(string(uavName + "/other_uav_pos").c_str(), 10, subOtherUavPosCallback);//其他无人系统定位
	ros::Subscriber other_uav_feedback_waypoint_sub = nh.subscribe(string(uavName + "/other_uav_fd_waypoint").c_str(), 10, subOtherUavFdWPCallback);
	ros::Subscriber cmd_result_sub = nh.subscribe(string(uavName + "/command_result").c_str(), 10, subCmdResultCallback);//订阅指令执行结果
	ros::Subscriber other_mb_sub = nh.subscribe(string(uavName + "/other_uav_mapbuffer").c_str(), 10, subOtherMBCallback);
	

	move_cmd_pub = nh.advertise<m100_v2::PubSubUavMsg>(string(uavName + "/move_cmd").c_str(), 10);//移动控制指令
	feedback_waypoint_pub = nh.advertise<m100_v2::PubSubUavMsg>(string(uavName + "/feedback_waypoint").c_str(), 10);//目标航点状态通告
	mapbuffer_pub = nh.advertise<m100_v2::PubSubUavMsg>(string(uavName + "/my_mapbuffer").c_str(),10);

	for (int i = 0; i<MAPLENGTH; i++)//初始化信息素图
	for (int j = 0; j < MAPLENGTH; j++)
	{
		_map[i][j] = 0.05;
	}

	pthread_t Cooperative_search_thread;
	pthread_create(&Cooperative_search_thread, NULL, Cooperative_search, NULL);

	ros::spin();

	pthread_mutex_destroy(&buf_mutex);
	pthread_mutex_destroy(&target_mutex);
	return 0;
}

