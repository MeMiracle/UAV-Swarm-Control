#include <ros/ros.h>
#include <uavmessage.pb.h>
#include <uavmessage.pb.cc>
#include <taskmessage.pb.h>
#include <taskmessage.pb.cc>
#include <feedbackdata.pb.h>
#include <feedbackdata.pb.cc>
#include <uwblpsmessage.pb.h>
#include <uwblpsmessage.pb.cc>
#include <string>
#include <fstream>
#include <pthread.h>
#include <zmq.h>
#include <arpa/inet.h>

using namespace std;

#define pi 3.1415926

void *subscriber;

char uav_address_config[] = "gcs_test/configure/uav_address_config.txt";

char gps_log[] = "gcs_test/log/gps_log.txt";
char dist_log[] = "gcs_test/log/dist_log.txt";
char vel_log[] = "gcs_test/log/vel_log.txt";
char waypoint_log[] = "gcs_test/log/waypoint_log.txt";
char uwb_log[] = "gcs_test/log/uwb.txt";

int tgt_uav_id = 1;//默认接收数据的无人机


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

int connectToDataTran()
{
    ROS_INFO("trying to connect to server");

    int sock_fromserver = -1;

    while(ros::ok())
    {
        sock_fromserver = socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);
        if(-1 == sock_fromserver)
        {
            ROS_INFO("creating socket failed!");
            break;
        }

        struct sockaddr_in server_addr;
        bzero(&server_addr,sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(42766);
        server_addr.sin_addr.s_addr = inet_addr("192.168.4.101");

        ros::Rate rate(1);
        while(ros::ok())
        {
            if(-1 == connect(sock_fromserver,(struct sockaddr*)(&server_addr),sizeof(struct sockaddr)))
            {
                ROS_ERROR("connecting failed:");
                rate.sleep();
                continue;
            }

            else
            {
                ROS_INFO("connected to server!");
                return sock_fromserver;
            }
        }
    }

    return sock_fromserver;
}

int main(int argc, char *argv[])
{
    /* code for main function */
    ros::init(argc, argv, "gcsRecvDT");
    ros::NodeHandle nh;

    ROS_INFO("gcsRecvDT_node starting.");

    int sock_fromserver = connectToDataTran();

    
    //持续接收数据
    ros::Rate loop(2);
    ros::Rate loop2(50);
    ROS_INFO("receiving msg...");
    //char* buffer = (char*)malloc(1024*(sizeof(char)));
    //memset(buffer,0,1024);
    while(ros::ok())
    {
        string str;
        int recvBytes = 0;

        while(true){
            char buffer[1024]={0};
            int ret = read(sock_fromserver,buffer,1024);//1024
            //int ret = recv(sock_fromserver,buffer,100,0);

            if(ret>0){
                ROS_INFO("recv %d bytes",ret);
                //str.assign(buffer,ret);
                str.append(buffer,ret);
                recvBytes += ret;
            }else{
                break;
            }
            //loop2.sleep();
        }
        

        if(recvBytes < 0)
        {
            perror("recv error:");
            continue;
        }

        else if(recvBytes == 0)
        {
            ROS_INFO("recv no message.");
            loop.sleep();
            continue;
        }

        else
        {
            ROS_INFO("total recv msg %d bytes:%s",recvBytes,str.c_str());
            continue;
        }


        uavMessage::Message msg;

        try
        {
            msg.ParseFromString(str);
        }
        catch (exception e)
        {
            ROS_ERROR("parse error:");
            continue;
        }
        
        ROS_INFO("src_uav_id=%d,msg_id=%d,topic_id=%d",msg.msghead().src_uav_id(),msg.msghead().msg_id(),msg.msghead().topic_id());

        if( msg.msghead().tgt_uav_id()[0] == 0)
        {
            string playload_str;
            playload_str = msg.playload();

            if(msg.msghead().msg_type() == 4)
            {
                //飞行状态反馈数据
                if(msg.mutable_msghead()->msg_id()==3 && msg.mutable_msghead()->topic_id()==5)
                {
                    feedbackData::FeedbackData fd_data;
                    fd_data.Clear();
                    fd_data.ParseFromString(msg.playload());

                    ROS_INFO("batteryPercent=%f,state=%d",fd_data.batterypercent(),fd_data.state());
                    ROS_INFO("dist position:(%lf,%lf,%lf)",fd_data.px(),fd_data.py(),fd_data.pz());
                    ROS_INFO("gps position:(%lf,%lf,%lf)",fd_data.lon(),fd_data.lat(),fd_data.alt());
                    ROS_INFO("roll=%lf,pitch=%lf,yaw=%lf",fd_data.roll(),fd_data.pitch(),fd_data.yaw());
                    ROS_INFO("vx=%lf,vy=%lf,vz=%lf,vw=%lf",fd_data.vx(),fd_data.vy(),fd_data.vz(),fd_data.vw());
                    ROS_INFO("ax=%lf,ay=%lf,az=%lf\n",fd_data.ax(),fd_data.ay(),fd_data.az());

                    // ofstream gps_str;
                    // string gps_file_name(getDirectory() + gps_log);
                    // gps_str.open(gps_file_name.c_str(),ios::out | ios::app);

                    // if(gps_str)
                    // {
                    //     gps_str << fd_data.lon() << " " << fd_data.lat() << " " << fd_data.alt() << endl;

                    //     gps_str.close();
                    // }
                    // else
                    // {
                    //     printf("%s ",gps_file_name.c_str());
                    //     perror("open failed: ");
                    // }

                    // ofstream dist_str;
                    // string dist_file_name(getDirectory() + dist_log);
                    // dist_str.open(dist_file_name.c_str(), ios::out | ios::app);

                    // if(dist_str)
                    // {
                    //     dist_str << fd_data.px() << " " << fd_data.py() << " " << fd_data.pz() << endl;
                        
                    //     dist_str.close();
                    // }
                    // else
                    // {
                    //     printf("%s ",dist_file_name.c_str());
                    //     perror("open failed: ");
                    // }

                    // ofstream vel_str;
                    // string vel_file_name(getDirectory() + vel_log);
                    // vel_str.open(vel_file_name.c_str(), ios::out | ios::app);

                    // if(vel_str)
                    // {
                    //     vel_str << fd_data.vx() << " " << fd_data.vy() << " " << fd_data.vz() << endl;

                    //     vel_str.close();
                    // }
                    // else
                    // {
                    //     printf("%s ",vel_file_name.c_str());
                    //     perror("open failed: ");
                    // }
                }    


                //航迹规划结果
                if(msg.msghead().msg_id()==7 && msg.msghead().topic_id()==9)
                {
                    // myTaskMessage::TaskMessage waypoints;
                    // waypoints.Clear();

                    // waypoints.ParseFromString(msg.playload());

                    // ofstream waypoints_str;
                    // string waypoints_file_name(getDirectory() + waypoint_log);

                    // waypoints_str.open(waypoints_file_name.c_str(),ios::out|ios::app);

                    // if(waypoints_str)
                    // {
                    //     for(int i=0;i<waypoints.point_size();i++)
                    //     {
                    //         waypoints_str << waypoints.point(i).x() << " " << waypoints.point(i).y() << " " << waypoints.point(i).z() << endl;
                    //     }

                    //     waypoints_str.close();
                    // }
                    // else
                    // {
                    //     printf("%s ",waypoints_file_name.c_str());
                    //     perror("open failed: ");
                    // }   
                }

                if(msg.msghead().msg_id()==10 && msg.msghead().topic_id()==12)
                {
                    // uwblpsMessage::uwblpsMessage uwb_pos;
                    // uwb_pos.Clear();

                    // uwb_pos.ParseFromString(msg.playload());

                    
                    // ofstream uwb_str;
                    // string uwb_file_name(getDirectory() + uwb_log);

                    // uwb_str.open(uwb_file_name.c_str(), ios::out|ios::app);

                    // if(uwb_str && msg.msghead().src_uav_id()==1)
                    // {
                    //     uwb_str << uwb_pos.linear_x() << " " << uwb_pos.linear_y() << " " << uwb_pos.linear_z() << endl;
                    //     uwb_str.close();
                    // }
                    // else
                    // {
                    //     printf("%s ",uwb_file_name.c_str());
                    //     perror("open failed: ");
                    // }   
                }
            }
        }
    }

    //free(buffer);

    ros::shutdown();
    return 0;
}