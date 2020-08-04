#include <ros/ros.h>
#include <zhelpers.h>
#include <uavmessage.pb.h>
#include <uavmessage.pb.cc>
#include <gcscommand.pb.h>
#include <gcscommand.pb.cc>
#include <taskmessage.pb.h>
#include <taskmessage.pb.cc>
#include <simulation_m100/PubSubUavMsg.h>
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
#include <simulation_m100/CommInfo.h>
#include <simulation_m100/UavBaseInfo.h>

using namespace std;


int uav_id = -1;//本机id,默认为-1，再通过初始化工作获取实际id
string uav_name("");

int cluster_id = 0;//集群id，默认为0，通过地面站指定

void *context;
string other_uav_address_config("simulation_m100/configure/other_uav_address_config_");


bool thread_created_done = false;
pthread_mutex_t msg_mutex = PTHREAD_MUTEX_INITIALIZER;//线程锁
pthread_mutex_t info_mutex = PTHREAD_MUTEX_INITIALIZER;//线程锁

ros::Publisher takeoff_cmd_pub;
ros::Publisher land_cmd_pub;
ros::Publisher gohome_cmd_pub;
ros::Publisher move_cmd_pub;
ros::Publisher waypoint_cmd_pub;
ros::Publisher pathplan_cmd_pub;
ros::Publisher swarm_waypoint_pub;
ros::Publisher formation_cmd_pub;
ros::Publisher syncfly_task_pub;

ros::Publisher base_info_pub;
ros::Publisher recv_info_pub;

ros::Publisher other_uav_pos_pub;
ros::Publisher other_uav_fd_data_pub;
ros::Publisher other_uav_fd_wp_pub;
ros::Publisher other_uav_mb_pub;
ros::Publisher other_uav_fm_pub;

void analyzeMsg(string uavmsg,string address,int recv_bytes);//分析消息

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
            //ROS_INFO("recv %d bytes from %s",ret,(char*)address.c_str());
        }


/*      //zmq_msg_t消息转成char  
        int size = zmq_msg_size(&recv_msg);
        char *ch = (char*)malloc(ret);

        memcpy(ch,zmq_msg_data(&recv_msg),ret);
        //str[size] = 0;

        for(int i=0;i<ret;i++)
        {
            ROS_INFO("%d ",ch[i]);
        }
        ROS_INFO("\n");
*/        
        //string str;
        //str.assign((char*)zmq_msg_data(&recv_msg),ret);
        
        //string str = static_cast<string>(ch);
        //string str.to_string(ch);// = (string)malloc(ret);
        //memcpy(&str,zmq_msg_data(&recv_msg),ret);
        //free(ch);

/*      //打印string
        ROS_INFO("msg_subscriber stirng %d bytes:",(int)str.size());
        for(int i=0;i<ret;i++)
        {
            ROS_INFO("%d ",str[i]);
        }
        ROS_INFO("\n");
*/
        //simulation_m100::PubSubUavMsg uavmsg;
        //uavmsg.playload.assign((char*)zmq_msg_data(&recv_msg),ret);

        string uavmsg;
        uavmsg.assign((char*)zmq_msg_data(&recv_msg),ret);

        analyzeMsg(uavmsg,address,ret);

        zmq_msg_close(&recv_msg); 
    }
    zmq_close(msg_subscriber);
}


void analyzeMsg(string uavmsg,string address,int recv_bytes)//分析消息
{ 
    uavMessage::Message message;
    message.ParseFromString(uavmsg);

    simulation_m100::CommInfo recv_info;

    recv_info.frame_id = message.msghead().seq_num();
    recv_info.src_address = address;
    recv_info.comm_bytes = recv_bytes;

    pthread_mutex_lock(&info_mutex);

    recv_info_pub.publish(recv_info);

    pthread_mutex_unlock(&info_mutex);

    //ROS_INFO("stx=%d,count=%d,type=%d,src_id=%d,tgt_id=%d",message.msghead().stx(),message.msghead().tgt_uav_count(),message.msghead().msg_type(),message.msghead().src_uav_id(),message.msghead().tgt_uav_id()[0]);
    if(message.msghead().stx() == 254)
    {
        int ifToMe = 0;

        if(message.msghead().tgt_uav_count() == 0)
        {
            ifToMe = 1;
        }

        if(message.msghead().tgt_uav_count() > 0)
        {   
            for(int i=0;i<message.msghead().tgt_uav_count();i++)
            {
                if(uav_id == message.msghead().tgt_uav_id()[i])
                {
                    ifToMe = 1;
                    break;
                }
            }
        }

        if(ifToMe == 1)
        {
            simulation_m100::PubSubUavMsg playload_msg;
            playload_msg.playload = message.playload();

            //分类处理消息
            if(message.msghead().msg_type() == 1)//指令消息
            {
                if(message.msghead().msg_id() == 1)//动作控制指令：起飞、降落、返航
                {
                    if(message.msghead().topic_id() == 1)//起飞指令  
                    {
                        ROS_INFO("takeoff_cmd from gcs.");

                        pthread_mutex_lock(&msg_mutex);
                        if(message.msghead().src_uav_id() == 0)
                        {
                            playload_msg.cmd_priority = 100;//地面指令优先级100
                        }
                        playload_msg.start_time = ros::Time::now().toSec();
                        takeoff_cmd_pub.publish(playload_msg);                  
                        pthread_mutex_unlock(&msg_mutex);
                    }

                    if(message.msghead().topic_id() == 2)//降落指令 
                    {
                        ROS_INFO("land_cmd from gcs.");

                        pthread_mutex_lock(&msg_mutex);
                        if(message.msghead().src_uav_id() == 0)
                        {
                            playload_msg.cmd_priority = 100;//地面指令优先级100
                        }
                        playload_msg.start_time = ros::Time::now().toSec();
                        land_cmd_pub.publish(playload_msg);                   
                        pthread_mutex_unlock(&msg_mutex);
                    }

                    if(message.msghead().topic_id() == 3)//返航指令  
                    {
                        ROS_INFO("gohome_cmd from gcs.");

                        pthread_mutex_lock(&msg_mutex);
                        if(message.msghead().src_uav_id() == 0)
                        {
                            playload_msg.cmd_priority = 100;//地面指令优先级100
                        }
                        playload_msg.start_time = ros::Time::now().toSec();
                        gohome_cmd_pub.publish(playload_msg);                  
                        pthread_mutex_unlock(&msg_mutex);
                    }
                }

                if(message.msghead().msg_id() == 2)//速度控制指令
                {
                    if(message.msghead().topic_id() == 4)//移动指令  
                    {
                        ROS_INFO("move_cmd from gcs.");

                        pthread_mutex_lock(&msg_mutex);
                        if(message.msghead().src_uav_id() == 0)
                        {
                            playload_msg.cmd_priority = 100;//地面指令优先级100
                        }
                        playload_msg.component = "Recv";
                        playload_msg.start_time = ros::Time::now().toSec();
                        move_cmd_pub.publish(playload_msg);                  
                        pthread_mutex_unlock(&msg_mutex);
                    }
                }

                if(message.msghead().msg_id() == 7)//航点控制指令
                {
                    if(message.msghead().topic_id() == 9)//航点任务
                    {
                        myTaskMessage::TaskMessage waypoint;
                        waypoint.ParseFromString(playload_msg.playload);

						ROS_INFO("clusterid=%d,kind=%d",waypoint.clusterid(),waypoint.kind());
                        
                        cluster_id = waypoint.clusterid();
                        simulation_m100::UavBaseInfo uav_base_info;
                        uav_base_info.cluster_id = cluster_id;

                        base_info_pub.publish(uav_base_info);

                        if(waypoint.kind() == 0)//单机航点
                        {
                            ROS_INFO("single waypoint_cmd from gcs.");

                            pthread_mutex_lock(&msg_mutex);
                            if(message.msghead().src_uav_id() == 0)
                            {
                                playload_msg.cmd_priority = 100;//地面指令优先级100
                            }
                            playload_msg.start_time = ros::Time::now().toSec();
                            waypoint_cmd_pub.publish(playload_msg);                  
                            pthread_mutex_unlock(&msg_mutex);
                        }
                        
                        if(waypoint.kind() == 1)//航迹规划的控制航点   
                        {
                            ROS_INFO("pathplan_cmd from gcs.");

                            pthread_mutex_lock(&msg_mutex);
                            playload_msg.start_time = ros::Time::now().toSec();
                            pathplan_cmd_pub.publish(playload_msg);               
                            pthread_mutex_unlock(&msg_mutex);
                        }

                        if(waypoint.kind() == 2)//群体航点
                        {
                            ROS_INFO("swarm waypoint from gcs.");

                            pthread_mutex_lock(&msg_mutex);
							playload_msg.src_uav_id = message.msghead().src_uav_id();
                            playload_msg.start_time = ros::Time::now().toSec();
                            swarm_waypoint_pub.publish(playload_msg);                  
                            pthread_mutex_unlock(&msg_mutex);
                        }

                        if(waypoint.kind() == 3)//编队形成的目标航点
                        {
                            ROS_INFO("formation waypoint from gcs.");

                            pthread_mutex_lock(&msg_mutex);
                            playload_msg.start_time = ros::Time::now().toSec();
                            formation_cmd_pub.publish(playload_msg);                  
                            pthread_mutex_unlock(&msg_mutex);
                        }

                        if(waypoint.kind() == 4)//避让追及实验起始点（实验用）
                        {
                            ROS_INFO("syncfly waypoint_cmd from gcs.");

                            pthread_mutex_lock(&msg_mutex);
                            if(message.msghead().src_uav_id() == 0)
                            {
                                playload_msg.cmd_priority = 100;//地面指令优先级100
                            }
                            playload_msg.start_time = ros::Time::now().toSec();
                            syncfly_task_pub.publish(playload_msg);                  
                            pthread_mutex_unlock(&msg_mutex);
                        }
                    }
                }
            }

            if(message.msghead().msg_type() == 4)//数据消息
            {
                if(message.msghead().msg_id() == 3)//其他无人系统的整合的状态反馈数据
                {
                    if(message.msghead().topic_id() == 5)
                    {
                        //ROS_INFO("feedback data from other uav.");

                        pthread_mutex_lock(&msg_mutex);

                        playload_msg.src_uav_id = message.msghead().src_uav_id();
                        playload_msg.seq_num = message.msghead().seq_num();
                        playload_msg.time_stamp = message.msghead().time_stamp();
                        other_uav_fd_data_pub.publish(playload_msg);
                                
                        pthread_mutex_unlock(&msg_mutex);
                    }
                }

                if(message.msghead().msg_id() == 7)//航点反馈数据：群体飞行时的航点到达状态
                {
                    if(message.msghead().topic_id() == 9)
                    {
                        ROS_INFO("swarm waypoint update from other uav.");

                        pthread_mutex_lock(&msg_mutex);

                        playload_msg.src_uav_id = message.msghead().src_uav_id();
                        //swarm_waypoint_pub.publish(playload_msg); 
                        other_uav_fd_wp_pub.publish(playload_msg);

                        pthread_mutex_unlock(&msg_mutex);
                    }
                }

                if(message.msghead().msg_id() == 10)//定位数据
                {
                    if(message.msghead().topic_id() == 12)//定位数据
                    {
                        //ROS_INFO("pos_data from other uav.");

                        pthread_mutex_lock(&msg_mutex);

                        playload_msg.src_uav_id = message.msghead().src_uav_id();
                        other_uav_pos_pub.publish(playload_msg);
                                
                        pthread_mutex_unlock(&msg_mutex);
                    }
                }

                if(message.msghead().msg_id() == 11)
                {
                    if(message.msghead().topic_id() == 13)//信息图
                    {
                        pthread_mutex_lock(&msg_mutex);

                        playload_msg.src_uav_id = message.msghead().src_uav_id();
                        other_uav_mb_pub.publish(playload_msg);

                        pthread_mutex_unlock(&msg_mutex);
                    }
                }

                if(message.msghead().msg_id() == 12)
                {
                    if(message.msghead().topic_id() == 14)
                    {
                        pthread_mutex_lock(&msg_mutex);

                        playload_msg.src_uav_id = message.msghead().src_uav_id();
                        other_uav_fm_pub.publish(playload_msg);
                        
                        pthread_mutex_unlock(&msg_mutex);
                    }
                }
            }
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
    string name(address_file_name);

    name = path + "src/" + name;

    return name;
}

int main(int argc, char *argv[])
{
    /* code for main function */
    ros::init(argc, argv, "m100Recv");
    ros::NodeHandle nh;

    ROS_INFO("m100Recv_node started.");

    ros::param::get("~uav_id",uav_id);
    if(uav_id == -1)
    {
        return -1;
    }

    ros::param::get("~uav_name",uav_name);

    takeoff_cmd_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/takeoff_cmd").c_str(),10);
    land_cmd_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/land_cmd").c_str(),10);
    gohome_cmd_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/gohome_cmd").c_str(),10);
    move_cmd_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/move_cmd").c_str(),10);
    waypoint_cmd_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/waypoint_cmd").c_str(),10);
    pathplan_cmd_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/pathplan_cmd").c_str(),10);
    swarm_waypoint_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/swarm_waypoint").c_str(),10);
    formation_cmd_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/formation_cmd").c_str(),10);
    syncfly_task_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/syncfly_task").c_str(),10);

    base_info_pub = nh.advertise<simulation_m100::UavBaseInfo>(string(uav_name + "/uav_base_info").c_str(),10);
    recv_info_pub = nh.advertise<simulation_m100::CommInfo>(string(uav_name + "/comm_recv_info").c_str(),10);

    other_uav_pos_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/other_uav_pos").c_str(),10);
    other_uav_fd_data_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/other_uav_feedback_data").c_str(),10);
    other_uav_fd_wp_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/other_uav_fd_waypoint").c_str(),10);
    other_uav_mb_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/other_uav_mapbuffer").c_str(),10);
    other_uav_fm_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/other_uav_formation_info").c_str(),10);

    context = zmq_init(2);
    if(context == NULL)
    {
        perror("zmq_init error:");
        return -1;
    }


    //接收地面站1消息，地址配置用参数服务器管理（由launch文件指定）
    string gcs_address1;
    ros::param::get("~gcs_address1",gcs_address1);
    pthread_t thread_recvFromGcs1;

    pthread_create(&thread_recvFromGcs1,NULL,recvMsg,(void*)(gcs_address1.c_str()));//创建线程，接收地面站消息

    while(thread_created_done == false)//阻塞至线程创建完成，否则运行太快会导致读取的gcs_address1被覆盖
    {
        ros::Duration(0.5).sleep();
    }

        
    //接收地面站2消息，地址配置用参数服务器管理（由launch文件指定）
    string gcs_address2;
    ros::param::get("~gcs_address2",gcs_address2);
    pthread_t thread_recvFromGcs2;

    thread_created_done = false;
    pthread_create(&thread_recvFromGcs2,NULL,recvMsg,(void*)(gcs_address2.c_str()));//创建线程，接收地面站消息

    while(thread_created_done == false)//阻塞至线程创建完成，否则运行太快会导致读取的gcs_address2被覆盖
    {
        ros::Duration(0.1).sleep();
    }


    //接收其他无人机的消息，由于无人机数量不确定，因此地址配置用文件管理
    //string uav_file_name = getFileName(other_uav_address_config);//获取配置文件的绝对路径
    string uav_file_name = getFileName((other_uav_address_config + uav_name + ".txt").c_str());

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

    pthread_mutex_destroy(&msg_mutex);
    pthread_mutex_destroy(&info_mutex);

    zmq_term(context);

    ros::shutdown();
    return 0;
}
