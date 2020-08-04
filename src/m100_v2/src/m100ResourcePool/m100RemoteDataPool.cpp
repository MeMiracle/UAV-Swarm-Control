#include <ros/ros.h>
#include <stdlib.h>
#include <vector>
#include <m100_v2/PubSubUavMsg.h>
#include <m100Location/uwblpsmessage.pb.h>
#include <m100Location/uwblpsmessage.pb.cc>
#include <feedbackdata.pb.h>
#include <feedbackdata.pb.cc>
#include <m100_v2/Position.h>
#include <fstream>

using namespace std;

int uav_id = -1;//本机id,默认为-1，再通过初始化工作获取实际id
string uav_name("");

void subOtherUavPosCallback(m100_v2::PubSubUavMsg other_uav_pos_msg);//实时更新其他无人系统位置
void subOtherUavFdDataCallback(m100_v2::PubSubUavMsg other_uav_fd_data_msg);//实时更新其他无人系统状态数据
void subOtherUavFdWPCallback(m100_v2::PubSubUavMsg other_uav_fd_wp_msg);
void subOtherUavMBCallback(m100_v2::PubSubUavMsg other_uav_mb_msg);
void subPosCallback(m100_v2::Position pos_msg);//位置更新
void subMyUavFdDataCallback(m100_v2::PubSubUavMsg my_uav_fd_data_msg);//实时更新本机状态数据

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
		return sqrt(pow(x_, 2) + pow(y_, 2) + pow(z_, 2));
	}

};

string getDirectory()
{
    char abs_path[1024];
    int cnt = readlink("/proc/self/exe", abs_path, 1024);//获取可执行程序的绝对路径
    if(cnt < 0|| cnt >= 1024)
    {
        return NULL;
    }

    //最后一个'/' 后面是可执行程序名，去掉devel/lib/m100/exe，只保留前面部分路径
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

    path = path + "src/";

    return path;
}

fstream swarm_pos_str;
fstream swarm_vel_str;

void openLogFiles(string uav_name)
{
    char pos_log[] = "m100_v2/log/swarm_pos_log_";
    string pos_file_name(getDirectory() + pos_log + uav_name.c_str() + ".txt");
    swarm_pos_str.open(pos_file_name.c_str(), ios::out | ios::app);
    if(swarm_pos_str)
    {
        swarm_pos_str << ros::Time::now() << endl;
    }

	char vel_log[] = "m100_v2/log/swarm_vel_log_";
    string vel_file_name(getDirectory() + vel_log + uav_name.c_str() + ".txt");
    swarm_vel_str.open(vel_file_name.c_str(), ios::out | ios::app);
    if(swarm_vel_str)
    {
        swarm_vel_str << ros::Time::now() << endl;
    }
}

void closeLogFiles()
{
    swarm_pos_str.close();
	swarm_vel_str.close();
}

int main(int argc, char *argv[])
{
    /* code for main function */
    ros::init(argc, argv, "m100RemoteDataPool");
    ros::NodeHandle nh;

	ROS_INFO("m100RemoteDataPool_node started.");

	ros::param::get("~uav_id",uav_id);
    if(uav_id == -1)
    {
        return -1;
    }

    ros::param::get("~uav_name",uav_name);

	int axisID = 1;
    string posTopic("/global_position");
    ros::param::get("~axis_id", axisID);
	if (axisID == 2)
	{
		posTopic = string("/local_position");
	}

    ros::Subscriber other_uav_pos_sub = nh.subscribe(string(uav_name + "/other_uav_pos").c_str(), 10, subOtherUavPosCallback);//其他无人系统uwb定位
    ros::Subscriber other_uav_feedback_data_sub = nh.subscribe(string(uav_name + "/other_uav_feedback_data").c_str(),10,subOtherUavFdDataCallback);//其他无人系统反馈数据
    ros::Subscriber other_uav_feedback_waypoint_sub = nh.subscribe(string(uav_name + "/other_uav_fd_waypoint").c_str(), 10, subOtherUavFdWPCallback);
	ros::Subscriber other_uav_mapbuffer_sub = nh.subscribe(string(uav_name + "/other_uav_mapbuffer"), 10, subOtherUavMBCallback);
	ros::Subscriber global_pos_sub = nh.subscribe(string(uav_name + posTopic).c_str(), 10, subPosCallback);
	ros::Subscriber my_uav_feedback_data_sub = nh.subscribe(string(uav_name + "/feedback_data").c_str(), 10, subMyUavFdDataCallback);
	

	//openLogFiles(uav_name);

	ros::Rate loop_rate(100);
    while(ros::ok())
	{
        ros::spinOnce();

        loop_rate.sleep();
    }
	ros::shutdown();
	
	//closeLogFiles();

    return 0;
}

// vector<Point3D> swarmDataBuf;
// vector<int> uavIndexBuf;
std::map<int, Point3D> swarmPosition;
std::map<int, Point3D> swarmVelocity;

void updateSwarmData(std::map<int, Point3D>& swarmData,int src_uav_id,Point3D point,fstream& swarm_str){
/*
	// int i=0;
	// for(i;i<uavIndexBuf.size();i++)
	// {
	// 	if(src_uav_id == uavIndexBuf[i])//缓存中有该无人系统的数据，则更新
	// 	{
	// 		swarmDataBuf[i] = point;

	// 		//swarmDataBuf.erase(i);
	// 		//swarmDataBuf.push_back(point);

	// 		break;
	// 	}
	// }

	// if(i == uavIndexBuf.size())//缓存中没有该无人系统的数据，则存入
	// {
	// 	uavIndexBuf.push_back(src_uav_id);
	// 	swarmDataBuf.push_back(point);
	// }
*/

    if(swarmData.find(src_uav_id) != swarmData.end())//缓存中有该无人系统的数据，则更新
    {
        swarmData[src_uav_id] = point;
    }
    else
    {
        swarmData.insert(pair<int,Point3D>(src_uav_id,point));
    }  

	// static ros::Time timeToShow = ros::Time::now();
    // if(ros::Time::now() - timeToShow >= ros::Duration(1))
    // {
		//ROS_INFO("swarmData size = %d",swarmData.size());
		// if(swarmData.size() >= 3)
		// {
		// 	std::map<int,Point3D>::iterator iter;
		// 	iter = swarmData.begin();

		// 	swarm_str << ros::Time::now() << " ";
		// 	while(iter != swarmData.end())
		// 	{
		// 		//ROS_INFO("uav%d:%lf,%lf,%lf",iter->first,iter->second.x,iter->second.y,iter->second.z);

		// 		if(swarm_str)
		// 		{
		// 			swarm_str << iter->second.x << " " << iter->second.y << " " << iter->second.z << " ";                  
		// 		}
				
		// 		iter ++;
		// 	} 
		// 	//printf("\n"); 
		// 	swarm_str << endl;
		// }
		
	// 	timeToShow = ros::Time::now();
    // }  
}


//通过专门数据通道获取其他无人机位置信息
void subOtherUavPosCallback(m100_v2::PubSubUavMsg other_uav_pos_msg)//实时更新其他无人系统位置
{
	// uwblpsMessage::uwblpsMessage other_uav_pos;
	// other_uav_pos.ParseFromString(other_uav_pos_msg.playload);

	// Point3D point(other_uav_pos.linear_x(),other_uav_pos.linear_y(),other_uav_pos.linear_z());

	// int src_uav_id = other_uav_pos_msg.src_uav_id;

	// updateSwarmData(swarmPosition,src_uav_id,point,swarm_pos_str);
}

void subPosCallback(m100_v2::Position pos_msg)//位置更新
{
    // Point3D point(pos_msg.x,pos_msg.y,pos_msg.z);

	// int src_uav_id = uav_id;

	// updateSwarmData(swarmPosition,src_uav_id,point,swarm_pos_str);
}


//通过统一反馈数据获取其他无人机信息
void subOtherUavFdDataCallback(m100_v2::PubSubUavMsg other_uav_fd_data_msg)//实时更新其他无人系统状态数据
{
	//ROS_INFO("got other uav fd_data.");
	feedbackData::FeedbackData other_uav_fd_data;
	other_uav_fd_data.ParseFromString(other_uav_fd_data_msg.playload);

	Point3D other_uav_pos(other_uav_fd_data.px(),other_uav_fd_data.py(),other_uav_fd_data.pz());
	Point3D other_uav_vel(other_uav_fd_data.vx(),other_uav_fd_data.vy(),other_uav_fd_data.vz());

	int other_uav_id = other_uav_fd_data_msg.src_uav_id;

	updateSwarmData(swarmPosition,other_uav_id,other_uav_pos,swarm_pos_str);
	updateSwarmData(swarmVelocity,other_uav_id,other_uav_vel,swarm_vel_str);
}

void subMyUavFdDataCallback(m100_v2::PubSubUavMsg my_uav_fd_data_msg)//实时更新本机状态数据
{
	static double timeToDeal = ros::Time::now().toSec();
	//if((ros::Time::now().toSec()-timeToDeal)*1000 >= 100){
		//ROS_INFO("got my fd_data.");
		feedbackData::FeedbackData my_uav_fd_data;
		my_uav_fd_data.ParseFromString(my_uav_fd_data_msg.playload);

		Point3D my_uav_pos(my_uav_fd_data.px(),my_uav_fd_data.py(),my_uav_fd_data.pz());
		Point3D my_uav_vel(my_uav_fd_data.vx(),my_uav_fd_data.vy(),my_uav_fd_data.vz());

		int my_uav_id = uav_id;

		updateSwarmData(swarmPosition,my_uav_id,my_uav_pos,swarm_pos_str);
		updateSwarmData(swarmVelocity,my_uav_id,my_uav_vel,swarm_vel_str);

	//	timeToDeal = ros::Time::now().toSec();
	//}
}

void subOtherUavFdWPCallback(m100_v2::PubSubUavMsg other_uav_fd_wp_msg)
{

}

void subOtherUavMBCallback(m100_v2::PubSubUavMsg other_uav_mb_msg)
{
	
}