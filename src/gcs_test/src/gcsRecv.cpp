#include <ros/ros.h>
#include <zhelpers.h>
#include <uavmessage.pb.h>
#include <uavmessage.pb.cc>
#include <feedbackdata.pb.h>
#include <feedbackdata.pb.cc>
#include <gcscommand.pb.h>
#include <gcscommand.pb.cc>
#include <taskmessage.pb.h>
#include <taskmessage.pb.cc>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Empty.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <pthread.h>
#include <gcs_test/PubSubUavMsg.h>

using namespace std;


void *context;
string uav_address_config("gcs_test/configure/uav_address_config.txt");


bool thread_created_done = false;


void *recvMsg(void *args)//共用线程函数，接收消息
{
    void *msg_subscriber;
 
    msg_subscriber = zmq_socket(context,ZMQ_SUB);//context是线程安全的，无需加锁

    string address((char*)args);
    ROS_INFO("trying to connect to %s",(char*)address.c_str());
    thread_created_done = true;
    
    int try_count=0;
    while(ros::ok())
    {
        int ret = zmq_connect(msg_subscriber,(char*)address.c_str());

        if(ret != 0)
        {
            perror("connecting error:");

            try_count++;
            if(try_count >= 3)
            {
                ROS_INFO("Can not connect to the address %s",(char*)address.c_str());
                return NULL;
            }

            ros::Duration(1).sleep();
        }

        else
        {
            ROS_INFO("connected to the server.");
            break;
        }
    }

    int flag = zmq_setsockopt(msg_subscriber, ZMQ_SUBSCRIBE, "", 0);

    if(flag != 0)
    {
        perror("set socket error:");
        return NULL;
    }

    ROS_INFO("receiving message...");
    while(ros::ok())
    {
        zmq_msg_t recv_msg;

        zmq_msg_init(&recv_msg);
        int ret = zmq_msg_recv(&recv_msg,msg_subscriber,0);

        if(ret < 0)
        {
            perror("msg_subscriber error:");
            break;
        }

        else if(ret == 0)
        {
            ROS_INFO("msg_subscriber no message.");
        }

        else
        {
             ROS_INFO("recv %d bytes from %s",ret,(char*)address.c_str());
        }

        string uavmsg;
        uavmsg.assign((char*)zmq_msg_data(&recv_msg),ret);

        uavMessage::Message msg;
        msg.ParseFromString(uavmsg);
        
        // ROS_INFO("tgt_id=%d",msg.msghead().tgt_uav_id()[0]);
        if( msg.msghead().tgt_uav_id()[0] == 0)
        {
            // string playload_str;
            // playload_str = msg.playload();

            //ROS_INFO("msg_type=%d,msg_id=%d,topic_id=%d",msg.msghead().msg_type(),msg.mutable_msghead()->msg_id(),msg.mutable_msghead()->topic_id());
            if(msg.msghead().msg_type() == 4)
            {
                //飞行状态反馈数据
                if(msg.mutable_msghead()->msg_id()==3 && msg.mutable_msghead()->topic_id()==5)
                {
                    feedbackData::FeedbackData fd_data;
                    fd_data.Clear();
                    fd_data.ParseFromString(msg.playload());

                    //ROS_INFO("uavID=%d,clusterID=%d",msg.msghead().src_uav_id(),msg.msghead().cluster_id());
                    //ROS_INFO("batteryPercent=%f,state=%d",fd_data.batterypercent(),fd_data.state());
                    //ROS_INFO("dist position:(%lf,%lf,%lf)",fd_data.px(),fd_data.py(),fd_data.pz());
                    // ROS_INFO("gps position:(%lf,%lf,%lf)",fd_data.lon(),fd_data.lat(),fd_data.alt());
                    // ROS_INFO("roll=%lf,pitch=%lf,yaw=%lf",fd_data.roll(),fd_data.pitch(),fd_data.yaw());
                    // ROS_INFO("vx=%lf,vy=%lf,vz=%lf,vw=%lf",fd_data.vx(),fd_data.vy(),fd_data.vz(),fd_data.vw());
                    // ROS_INFO("ax=%lf,ay=%lf,az=%lf\n",fd_data.ax(),fd_data.ay(),fd_data.az());
                    ROS_INFO(" ");
                }    

                if(msg.mutable_msghead()->msg_id()==7 && msg.mutable_msghead()->topic_id()==14)
                {
                    gcs_test::PubSubUavMsg playload_msg;
                    playload_msg.playload = msg.playload();
                    playload_msg.src_uav_id = msg.msghead().src_uav_id();

                    myTaskMessage::TaskMessage gimbal_angle_task;
                    gimbal_angle_task.ParseFromString(playload_msg.playload);
                    

                    //ROS_INFO("received waypoints!");
                   
                    for (int i = 0; i<gimbal_angle_task.point_size(); i++)
                    {
                        myTaskMessage::MyPoint waypoint;
                        waypoint = gimbal_angle_task.point(i);

                        ROS_INFO("target:%f,%f,%f,%d", waypoint.x(), waypoint.y(), waypoint.z(), waypoint.mark());
                    }
                    

                    //ROS_INFO("uavID=%d,clusterID=%d",msg.msghead().src_uav_id(),msg.msghead().cluster_id());

                    ROS_INFO("Receive gimbal data");
                    
                    ROS_INFO(" ");
                }   

            }
        }

        zmq_msg_close(&recv_msg); 
    }
    zmq_close(msg_subscriber);
}


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

int main(int argc, char *argv[])
{
    /* code for main function */
    ros::init(argc, argv, "gcsRecv");
    ros::NodeHandle nh;

    ROS_INFO("gcsRecv_node started.");

    context = zmq_init(2);
    if(context == NULL)
    {
        perror("zmq_init error:");
        return -1;
    }

    //接收无人机的消息，由于无人机数量不确定，因此地址配置用文件管理
    //string uav_file_name = getFileName(other_uav_address_config);//获取配置文件的绝对路径
    string uav_file_name(getDirectory() + uav_address_config);

    ifstream uav_ifs;
    uav_ifs.open(uav_file_name.c_str(), ios::in|ios::out);
    if(!uav_ifs)
    {
        printf("%s ",uav_file_name.c_str());
        perror("open error:");

        return -1;
    }

    string uav_address;
    pthread_t thread_recvFromUav;
    while(getline(uav_ifs,uav_address))//为每个地址开启一个独立的接收线程
    {
        //ROS_INFO("address: %s",uav_address.c_str());

        thread_created_done = false;
        pthread_create(&thread_recvFromUav,NULL,recvMsg,(void*)(uav_address.c_str()));//创建线程，接收地面站消息

        while(thread_created_done == false)
        {
            ros::Duration(0.1).sleep();
        }
    }
    
    uav_ifs.close();

    ros::spin();
    
    zmq_term(context);

    ros::shutdown();
    return 0;
}