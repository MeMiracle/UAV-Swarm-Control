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

// ofstream gps_str;
// ofstream pos_str;

Point3D uwb_point={0,0,0};
pthread_mutex_t point_mutex = PTHREAD_MUTEX_INITIALIZER;//互斥锁
std::string uwb_serial_port("/dev/uwb_serial_port");
//char uwb_port[] = "/dev/uwb_serial_port";//"uwb_serial_port" defined in /etc/udev/rules.d/serial port.rules

/*
void getUwblps_data()
{
    uwb_zyl uwblps;

    uint8 buffer[256] = {0};
    uwblps.openUwblpsSerial(uwb_serial_port.c_str(),460800,8,NO,1);
    while(ros::ok())
    {
        int rd_num = uwblps.readData(buffer,256);

        if(rd_num)
        {
            for(int i = 0;i<rd_num;i++)
            {
                if(buffer[i] == 0x55 && buffer[i+1] == 0x01)
                {
                    if((rd_num - i) >127)
                        uwblps.uwblps_Data_Convert(buffer,i);

                }
            }
        }


    }
}

void testUwbData()
{
    uwb_zyl uwblps;
    uint8 buffer[256] = {0};
    uwblps.openUwblpsSerial(uwb_serial_port.c_str(),460800,8,NO,1);
    while(ros::ok())
    {
        // int first_num = uwblps.readData(tempbuf,2);
        // if (first_num == 2 && tempbuf[0] == 0x55 && tempbuf[1] == 0x01)
        // {
        //     int res_num = uwblps.readData(resbuf,126);
        //     uwblps.uwblps_Data_Convert(resbuf);

        // }

        int rd_num = uwblps.readData(buffer);
        //cout<<rd_num<<endl;
        if(rd_num)
        {
            // for(int i =0;i<rd_num;i++)
            // {
            //     printf("%.2x",buffer[i]);
            // }
            // cout<<endl;

            for(int i = 0;i<rd_num;i++)
            {
                if(buffer[i] == 0x55 && buffer[i+1] == 0x01)
                {
                    if(rd_num - i >127)
                    {
                        uwblps.uwblps_Data_Convert(buffer,i);
                    }
                }
            }
        }
    }
}


void testUwbbyBuf(Point2D& point)
{
    uwb_zyl uwblps;
    uint8 buffer[256] = {0};
    uwblps.openUwblpsSerial(uwb_serial_port.c_str(),460800,8,NO,1);
    while(ros::ok())
    {
        int rd_num = uwblps.readData(buffer);
        //cout<<rd_num<<endl;
        if(rd_num)
        {
            for(int i = 0;i<rd_num;i++)
            {
                if(buffer[i] == 0x55 && buffer[i+1] == 0x01)
                {
                    if(rd_num - i >127)
                    {
                        point = uwblps.uwblps_Data_Convert(buffer,i);
                    }
                }
            }
        }
    }
}
*/

string getDirectory()
{
    char abs_path[1024];
    int cnt = readlink("/proc/self/exe", abs_path, 1024);//获取可执行程序的绝对路径
    if(cnt < 0|| cnt >= 1024)
    {
        return NULL;
    }

    //最后一个'/' 后面是可执行程序名，去掉devel/lib/simulation_m100/exe，只保留前面部分路径
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

bool uwb_pubing = true;//优先使用uwb定位，uwb无数据时使用gps
void* getUwbPos3D(void* args)
{
	while(ros::ok())
	{
		uwb_zyl uwblps;
		uint8 buffer[256] = {0};
		bool ifprint = true;
		bool ifsuccess = false;
		while(ros::ok())//open uwb serial port
		{
			ifsuccess = uwblps.openUwblpsSerial(uwb_serial_port.c_str(),460800,8,NO,1);

			if(ifsuccess == false)
			{
				if(ifprint)
				{
					ROS_INFO("Error Opening %s Port",uwb_serial_port.c_str());
					ROS_INFO("change to gps location...");
					ifprint = false;
					uwb_pubing = false;
				}

				ros::Duration(1).sleep();
			}
			else
			{
				ROS_INFO("Success Opening %s Port",uwb_serial_port.c_str());
                ROS_INFO("using uwb location...");
				break;
			}
		}


		char raw_uwb_fileName[] = "simulation_m100/log/raw_uwb_log.txt";
		string raw_uwb_workspace_path = getDirectory();
		string raw_uwb_file(raw_uwb_workspace_path + raw_uwb_fileName);

		ofstream raw_uwb_str;
		raw_uwb_str.open(raw_uwb_file.c_str(), ios::out|ios::app);//open log file

		if(raw_uwb_str)
		{
		    raw_uwb_str << ros::Time::now() <<endl;
		}


        char valid_uwb_fileName[] = "simulation_m100/log/valid_uwb_log.txt";
		string valid_uwb_workspace_path = getDirectory();
		string valid_uwb_file(valid_uwb_workspace_path + valid_uwb_fileName);

		ofstream valid_uwb_str;
		valid_uwb_str.open(valid_uwb_file.c_str(), ios::out|ios::app);//open log file

		if(valid_uwb_str)
		{
		    valid_uwb_str << ros::Time::now() <<endl;
		}


		ros::Time start_time = ros::Time::now();
        ros::Time count_time = ros::Time::now();
		simulation_m100::Position pos;
		bool print_flag1 = true;
		ros::Rate loop_rate(200);
        int lost_count = 0;
		while(ros::ok())
		{
			uwb_point.x = 0;
			uwb_point.y = 0;
			uwb_point.z = 0;

		    int rd_num = uwblps.readData(buffer);
		    //cout<<rd_num<<endl;

		    if(rd_num)//读取到数据，但是可能不是有效数据
		    {
                lost_count = 0;

		        for(int i = 0;i<rd_num;i++)
		        {
		            if(buffer[i] == 0x55 && buffer[i+1] == 0x01)//帧头
		            {
		                if(rd_num - i >127)//是否包含一个完整的帧
		                {
                            //存储一个帧
                            raw_uwb_str << ros::Time::now() << " ";
                            for (size_t j = 0; j < 128; j++)
                            {
                                raw_uwb_str << (int)buffer[j] << " ";
                                // ROS_INFO("%d ",buffer[j]);
                            }
                            raw_uwb_str << endl;
                            

		                    uwb_point = uwblps.uwblps_Data_Convert(buffer,i);

							if((abs(uwb_point.x-0.761)<0.01 && abs(uwb_point.y-0.761)<0.01 && abs(uwb_point.z-0)<0.01) ||//no valid data
                                (abs(uwb_point.x-0)<0.01 && abs(uwb_point.y-0)<0.01 && abs(uwb_point.z-0)<0.01))
                            {
								if(ros::Time::now() - start_time > ros::Duration(1) && print_flag1)
								{
									ROS_INFO("data from uwb location is invalid!");
									ROS_INFO("uwb location is pausing and waiting!");
								
									print_flag1 = false;
									uwb_pubing = false;

									start_time = ros::Time::now();

									break;
								}
							}
							else//valid data
							{
								pos.kind = 1;

								pos.x = uwb_point.x;
								pos.y = uwb_point.y;
								pos.z = uwb_point.z;

								local_pos_pub.publish(pos);

								//ROS_INFO("uwb location:%f,%f,%f",pos.x,pos.y,pos.z);

                                //局部坐标转成全局坐标
                                simulation_m100::Position global_pos;
                                global_pos.kind = 0;
                                global_pos.x = pos.x * cosOffsetAngle + pos.y * sinOffsetAngle + axisOffsetX;
                                global_pos.y =-pos.x * sinOffsetAngle + pos.y * cosOffsetAngle + axisOffsetY;
                                global_pos.z = pos.z + axisOffsetZ;

                                global_pos_pub.publish(global_pos);

								print_flag1 = true;
								uwb_pubing = true;


                                // if(ros::Time::now() - count_time >= ros::Duration(0.02))
                                // {
                                    if(valid_uwb_str)//record valid data
                                    {
                                        valid_uwb_str << ros::Time::now() << " " << uwb_point.x << " " << uwb_point.y << " " << uwb_point.z << endl;
                                    }

                                //     count_time = ros::Time::now();
                                // }

								start_time = ros::Time::now();
							}
		                }
		            }
		        }
		    }
			else//读取不到数据，可能是因为uwb标签已拔出，或者是超出定位范围
			{
                lost_count++;

                if(lost_count > 200)//连续丢包超过200个，则认为是标签已拔出，，需要重新监听标签是否再次插入
                {
                    ROS_INFO("can not read data from uwb location!");
                
                    uwb_pubing = false;

                    break;
                }
			}

			loop_rate.sleep();
		}

        uwblps.closeUwblpsSerial();
		raw_uwb_str.close();
        valid_uwb_str.close();
	}
}

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

bool isHomeInited = false;
void gps_callback(const sensor_msgs::NavSatFix::ConstPtr& msg)
{
    current_gps = *msg;//按回调频率更新数据

    if(isHomeInited == false)
    {
        if(home_gps.longitude < 0 && home_gps.latitude < 0 && home_gps.altitude < 0)
        {   
            //如果没有手动设置home点，则在开机初始化时获取当前位置作为家的位置
            home_gps = current_gps;
        }
        else
        {
            //如果手动设置了home点，则只需要修正高度值即可
            home_gps.altitude = current_gps.altitude;
        }

        isHomeInited = true;
    }

    
    //gps经纬度转换为全局距离坐标
    geometry_msgs::Vector3 gps_point;
    localOffsetFromGpsOffset(gps_point, current_gps, home_gps);

    simulation_m100::Position pos;
    pos.kind = 0;

    pos.x = gps_point.x;
    pos.y = gps_point.y;
    pos.z = gps_point.z;

    //ROS_INFO("gps location:%f,%f,%f",pos.x,pos.y,pos.z);

    //gps全局距离坐标转换为局部坐标（实验坐标系内的坐标）
    simulation_m100::Position local_pos;
    local_pos.kind = 1;
    local_pos.x = pos.x * cosOffsetAngle - pos.y * sinOffsetAngle - axisOffsetX;
    local_pos.y = pos.x * sinOffsetAngle + pos.y * cosOffsetAngle - axisOffsetY;
    local_pos.z = pos.z - axisOffsetZ;

    static ros::Time count_time = ros::Time::now();
    // if(ros::Time::now() - count_time >= ros::Duration(0.02))
    // {
        // if(gps_str)
        // {
        //     gps_str << ros::Time::now() << " " << current_gps.longitude << " " << current_gps.latitude << " " << current_gps.altitude << endl;      
        // }


        // if(pos_str)
        // {
        //     pos_str << ros::Time::now() << " " << local_pos.x << " " << local_pos.y << " " << local_pos.z << endl;                  
        // }

    //     count_time = ros::Time::now();
    // }

    static bool flag = true;
    if(uwb_pubing == false)
    {
        global_pos_pub.publish(pos);
        local_pos_pub.publish(local_pos);

        if(flag)//初次（或重新）获得数据时打印
        {
            ROS_INFO("using gps location...");
            flag = false;
        }
    }
    else
    {
        flag = true;
    }
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "m100Location_node");
    ros::NodeHandle nh;

    ROS_INFO("m100Location_node started!");

    ros::param::get("~uwb_serial_port",uwb_serial_port);

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

    local_pos_pub = nh.advertise<simulation_m100::Position>(string(uav_name + "/local_position").c_str(),10);
    global_pos_pub = nh.advertise<simulation_m100::Position>(string(uav_name + "/global_position").c_str(),10);


    // char gps_log[] = "simulation_m100/log/gps_log_";
    // string gps_file_name(getDirectory() + gps_log + uav_name.c_str() + ".txt");
    // gps_str.open(gps_file_name.c_str(),ios::out | ios::app);
    // if(gps_str)
    // {
    //     gps_str << ros::Time::now() << endl;
    // }

    // char pos_log[] = "simulation_m100/log/pos_log_";
    // string pos_file_name(getDirectory() + pos_log + uav_name.c_str() + ".txt");
    // pos_str.open(pos_file_name.c_str(), ios::out | ios::app);
    // if(pos_str)
    // {
    //     pos_str << ros::Time::now() << endl;
    // }

    // zmq::context_t context(1);
    // zmq::socket_t publisher(context, ZMQ_PUB);
    // publisher.bind("tcp://*:5563");

    // Point2D the_point={0,0};
    // thread t1(testUwbbyBuf,ref(the_point));

    pthread_t uwb_thread;
	pthread_create(&uwb_thread,NULL,getUwbPos3D,NULL);
	ros::Duration(0.5).sleep();

    //thread t1(getUwbPos3D,ref(uwb_point));

    ros::spin();

    //if(t1.joinable())
    //    t1.join();

    // gps_str.close();
    // pos_str.close();

    ros::shutdown();
    return 0;
}
