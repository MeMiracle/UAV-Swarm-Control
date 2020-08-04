#include <ros/ros.h>
#include <stdlib.h>
#include <vector>
#include <m100_v2/PubSubUavMsg.h>
#include <m100_v2/CmdResult.h>
#include <feedbackdata.pb.h>
#include <feedbackdata.pb.cc>
#include <gcscommand.pb.h>
#include <gcscommand.pb.cc>
#include <taskmessage.pb.h>
#include <taskmessage.pb.cc>
#include <fstream>

using namespace std;

int uav_id = -1;//本机id,默认为-1，再通过初始化工作获取实际id
string uav_name("");

string component = "m100SyncFly";
string move_cmd = "move_cmd";
int sync_fly_priority = 90;

void subOtherUavFdDataCallback(m100_v2::PubSubUavMsg other_uav_fd_data_msg);//实时更新其他无人系统状态数据
void subMyUavFdDataCallback(m100_v2::PubSubUavMsg my_uav_fd_data_msg);//实时更新本机状态数据
void syncfly_task_callback(const m100_v2::PubSubUavMsg move_cmd_msg);//订阅并分析控制指令
void subCmdResultCallback(m100_v2::CmdResult result);

pthread_t syncflyThread;
void* syncflyOtherUav();

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

//ros::Publisher waypoint_cmd_pub;
ros::Publisher move_cmd_pub;

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

	int count1=0;
	while(ros::ok())
	{
    	swarm_pos_str.open(pos_file_name.c_str(), ios::out | ios::app);
		
		if(swarm_pos_str.is_open())
	    {
			ROS_INFO("file ok:%s",pos_file_name.c_str());
        	//swarm_pos_str << ros::Time::now() << endl;
			break;
    	}
		else
		{
			ROS_INFO("file failed:%s",pos_file_name.c_str());
			count1++;

			if(count1 > 10)
			{
				break;
			}
			else
			{
				ros::Duration(0.5).sleep();
			}
		}
	}

	char vel_log[] = "m100_v2/log/swarm_vel_log_";
    string vel_file_name(getDirectory() + vel_log + uav_name.c_str() + ".txt");
    
	int count2=0;
	while(ros::ok())
	{
    	swarm_vel_str.open(vel_file_name.c_str(), ios::out | ios::app);
		
		if(swarm_vel_str.is_open())
	    {
			ROS_INFO("file ok:%s",vel_file_name.c_str());
        	//swarm_pos_str << ros::Time::now() << endl;
			break;
    	}
		else
		{
			ROS_INFO("file failed:%s",vel_file_name.c_str());
			count2++;

			if(count2 > 10)
			{
				break;
			}
			else
			{
				ros::Duration(0.5).sleep();
			}
		}
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
    ros::init(argc, argv, "m100SyncFly");
    ros::NodeHandle nh;

	ROS_INFO("m100SyncFly_node started.");

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

    ros::Subscriber other_uav_feedback_data_sub = nh.subscribe(string(uav_name + "/other_uav_feedback_data").c_str(),10,subOtherUavFdDataCallback);//其他无人系统反馈数据
	ros::Subscriber my_uav_feedback_data_sub = nh.subscribe(string(uav_name + "/feedback_data").c_str(), 10, subMyUavFdDataCallback);
	ros::Subscriber cmd_result_sub = nh.subscribe(string(uav_name + "/command_result").c_str(), 10, subCmdResultCallback);//订阅指令执行结果
    ros::Subscriber syncfly_task_sub = nh.subscribe(string(uav_name + "/syncfly_task").c_str(),10,&syncfly_task_callback);

	//waypoint_cmd_pub = nh.advertise<m100_v2::PubSubUavMsg>(string(uav_name + "/waypoint_cmd").c_str(),10);
	move_cmd_pub = nh.advertise<m100_v2::PubSubUavMsg>(string(uav_name + "/move_cmd").c_str(),10);

	//pthread_create(&syncflyThread,NULL,syncflyOtherUav,NULL);
    //ros::Duration(0.5).sleep();//等待线程启动
    //ROS_INFO("syncflyThread is started.");

	openLogFiles(uav_name);

	ros::Rate loop_rate(100);
    while(ros::ok())
	{
        ros::spinOnce();

		syncflyOtherUav();

        loop_rate.sleep();
    }

	
	closeLogFiles();

	ros::shutdown();

    return 0;
}

std::map<int, Point3D> swarmPosition;
std::map<int, Point3D> swarmVelocity;
bool cmd_status = false;//命令执行状态反馈
bool new_task = false;
double delay = 0.1;
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
		if(cmd_status)
		{
			std::map<int,Point3D>::iterator iter;
			iter = swarmData.begin();

			if(new_task == true)
			{
				swarm_str << delay << endl;
			}
			
			swarm_str << ros::Time::now() << " ";
			while(iter != swarmData.end())
			{
				//ROS_INFO("uav%d:%lf,%lf,%lf",iter->first,iter->second.x,iter->second.y,iter->second.z);

				if(swarm_str)
				{
					swarm_str << iter->second.x << " " << iter->second.y << " " << iter->second.z << " ";                  
				}
				
				iter ++;
			} 
			//printf("\n"); 
			swarm_str << endl;
		}
		
	// 	timeToShow = ros::Time::now();
    // }  
}

///通过统一反馈数据获取其他无人机信息
void subOtherUavFdDataCallback(m100_v2::PubSubUavMsg other_uav_fd_data_msg)//实时更新其他无人系统状态数据
{
	static double timeToUpdate = ros::Time::now().toSec();
	if(ros::Time::now().toSec()-timeToUpdate >= delay)
	{
		//ROS_INFO("got other uav fd_data.");
		feedbackData::FeedbackData other_uav_fd_data;
		other_uav_fd_data.ParseFromString(other_uav_fd_data_msg.playload);

		Point3D other_uav_pos(other_uav_fd_data.px(),other_uav_fd_data.py(),other_uav_fd_data.pz());
		Point3D other_uav_vel(other_uav_fd_data.vx(),other_uav_fd_data.vy(),other_uav_fd_data.vz());

		int other_uav_id = other_uav_fd_data_msg.src_uav_id;

		updateSwarmData(swarmPosition,other_uav_id,other_uav_pos,swarm_pos_str);
		updateSwarmData(swarmVelocity,other_uav_id,other_uav_vel,swarm_vel_str);

		timeToUpdate = ros::Time::now().toSec();
	}
}

void subMyUavFdDataCallback(m100_v2::PubSubUavMsg my_uav_fd_data_msg)//实时更新本机状态数据
{
	//static double timeToDeal = ros::Time::now().toSec();
	//if(ros::Time::now().toSec()-timeToDeal >= delay){
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

//myTaskMessage::TaskMessage my_target_point;
int XorY = 0;
double velocity = 1;
void syncfly_task_callback(const m100_v2::PubSubUavMsg syncfly_task_msg)//
{
	myTaskMessage::TaskMessage task_waypoints;
    task_waypoints.ParseFromString(syncfly_task_msg.playload);

	delay = task_waypoints.point(0).yaw();//由地面站指定数据更新频率，默认0.1s
	ROS_INFO("New task:other uav data delay %lfs.",delay);

	if(abs(task_waypoints.point(0).x()) > abs(task_waypoints.point(0).y()))//通过比较X和Y的值，判断朝哪个方向前进，默认朝X轴
	{
		XorY = 0;//朝X轴飞行
	}
	else
	{
		XorY = 1;//朝Y轴飞行
	}

	velocity = task_waypoints.point(0).z();//由地面站指定飞行速度，默认1m/s

	new_task = true;
    cmd_status = true;
}

void* syncflyOtherUav()//持续发布指令
{
	//while(ros::ok())
	//{
        if(cmd_status == false)
        {
            //ros::Duration(0.1).sleep();
        }
		else
		{
			//////////////////////求平均位置//////////////////////
            double sumD = 0.0;
			std::map<int,Point3D>::iterator iter;
			iter = swarmPosition.begin();
			while(iter != swarmPosition.end())
			{
				if(XorY == 0)
				{
					sumD += iter->second.x;
				}
				else if(XorY == 1)
				{
					sumD += iter->second.y;
				}

				iter ++;
			}

            double averageD = 0;
            if(swarmPosition.size() != 0)
            {
                averageD = sumD/swarmPosition.size();
            }
			else
			{
				if(XorY == 0)
				{
					averageD = swarmPosition[uav_id].x;
				}
				else if(XorY == 1)
				{
					averageD = swarmPosition[uav_id].y;
				}
			}
            //////////////////////////////////////////////////////////////////

			//////////////求总的机间距离///////////////////////
            double sumDeltaD = 0.0;
			std::map<int,Point3D>::iterator iter1;
			iter1 = swarmPosition.begin();
			while(iter1 != swarmPosition.end())
			{
				if(XorY == 0)
				{
					sumDeltaD += abs(averageD - iter1->second.x);
				}
				else if(XorY == 1)
				{
					sumDeltaD += abs(averageD - iter1->second.y);
				}
				
				iter1 ++;
			}
            ROS_INFO("sumDeltaD=%lf",sumDeltaD);

			/////////////////求自身的速度调整幅度/////////////////////////
			double myD = averageD;
			if(XorY == 0)
			{
				myD = swarmPosition[uav_id].x;
			}
			else if(XorY == 1)
			{
				myD = swarmPosition[uav_id].y;
			}

            double deltaV = 0.0;
            if(sumDeltaD != 0)
            {
                deltaV = (averageD-myD)/sumDeltaD;
            }

            if(abs(averageD-myD) <= 2)
            {
                deltaV *= 0.8;
            }
            if(abs(averageD-myD) <= 1)
            {
                deltaV *= 0.6;
            }
            if(abs(averageD-myD) <= 0.5)
            {
                deltaV *= 0.5;
            }
            else if(abs(averageD-myD) <= 0.2)
            {
                deltaV *= 0.2;
            }
            else if(abs(averageD-myD) <= 0.1)
            {
                deltaV *= 0.1;
            }
            else if(abs(averageD-myD) <= 0.05)
            {
                deltaV *= 0.05;
            }
            else if(abs(averageD-myD) <= 0.02)
            {
                deltaV *= 0.02;
            }
            else if(abs(averageD-myD) <= 0.01)
            {
                deltaV *= 0.01;
            }
				
            double ctl_vel = velocity;
            ctl_vel += deltaV;

            //ROS_INFO("ctl_vel=%lf",ctl_vel);

            gcsCommand::Twist new_twist;

			if(XorY == 0)
			{
				new_twist.set_linear_x(ctl_vel);
            	new_twist.set_linear_y(0);
			}
			else if(XorY == 1)
			{
				new_twist.set_linear_x(0);
            	new_twist.set_linear_y(ctl_vel);
			}
            
            new_twist.set_linear_z(0);
            new_twist.set_angular_x(2);//场地坐标系速度控制
            new_twist.set_angular_y(0);
            new_twist.set_angular_z(0);

            string new_twist_str;
            if(!new_twist.SerializeToString(&new_twist_str))
            {
                perror("serialize error:");
            }

            m100_v2::PubSubUavMsg playload_msg;
            playload_msg.playload = new_twist_str;
            playload_msg.component = component;
            playload_msg.cmd_priority = sync_fly_priority;
            playload_msg.start_time = ros::Time::now().toSec();
            
            move_cmd_pub.publish(playload_msg);

			new_task = false;

            //ros::Duration(0.05).sleep();//
		}
//	}
}
