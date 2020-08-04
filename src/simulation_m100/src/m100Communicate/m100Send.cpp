#include <ros/ros.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <uavmessage.pb.h>
#include <uavmessage.pb.cc>
#include <gcscommand.pb.h>
#include <gcscommand.pb.cc>
#include <feedbackdata.pb.h>
#include <feedbackdata.pb.cc>
#include <simulation_m100/PubSubUavMsg.h>
#include <simulation_m100/Position.h>
#include <m100Location/uwblpsmessage.pb.h>
#include <m100Location/uwblpsmessage.pb.cc>
#include <zmq.h>
#include <string>
#include <simulation_m100/CommInfo.h>
#include <simulation_m100/UavBaseInfo.h>


#define C_PI (double)3.141592653589793
#define C_EARTH (double)6378137.0

const float deg2rad = C_PI/180.0;
const float rad2deg = 180.0/C_PI;

#define pi 3.1415926

using namespace std;

int uav_id = -1;//本机id,默认为-1，再通过初始化工作获取实际id
string uav_name("");

int cluster_id = 0;//集群id，默认为0，由地面站指定

string other_uav_id_config("simulation_m100/configure/other_uav_id_config_");
int other_uav_id[100];
int other_uav_count=0;

ros::Publisher send_info_pub;

string addressToGcs;
string addressToUav;

//u_int seq_num = 0;
u_int fd_data_gcs_seq = 0;//发送到地面站的反馈数据包的序列号
u_int fd_data_uav_seq = 0;//发送到其他无人机的反馈数据包的序列号
u_int fd_wp_uav_seq = 0;//发送到其他无人机的反馈航点包的序列号
u_int pos_uav_seq = 0;//发送到其他无人机的位置数据包的序列号
u_int mpbuff_uav_seq = 0;//发送到其他无人机的信息素包的序列号
u_int formation_uav_seq = 0;//发送到其他无人机的队形信息包的序列号

void *publisherToUav;
void *publisherToGcs;


int sendOutMsg(string msg_str,void *send_publisher,int frame_id,string address)
{
    //string*转成zmq_mst_t*
    zmq_msg_t send_msg;
    zmq_msg_init_size(&send_msg,msg_str.size());
    memcpy(zmq_msg_data(&send_msg),msg_str.c_str(),msg_str.size());

    //发送zmq_msg_t
    int send_byte;
    send_byte = zmq_msg_send(&send_msg,send_publisher,0);
    //ROS_INFO("send %d bytes.",send_byte);


	//ROS_INFO("frame %d,send %d bytes.",frame_id,send_byte);

    simulation_m100::CommInfo send_info;

    //ROS_INFO("frame_id=%d",frame_id);
    send_info.frame_id = frame_id;
    send_info.src_address = address;
    send_info.comm_bytes = send_byte;

    send_info_pub.publish(send_info);

    zmq_msg_close(&send_msg);

    return send_byte;
}


void feedback_data_callback(simulation_m100::PubSubUavMsg fd_data_msg)//状态数据反馈
{              
    static ros::Time timeToGcs = ros::Time::now();
	static ros::Time timeToShow = ros::Time::now();
    if(ros::Time::now() - timeToGcs >= ros::Duration(1))//持续发送的数据需要指定发送频率
    {
        uavMessage::Message message; 

        message.mutable_msghead()->set_stx(254);
        message.mutable_msghead()->set_msg_type(4);
        message.mutable_msghead()->set_cluster_id(cluster_id);
        message.mutable_msghead()->set_src_uav_id(uav_id);
        message.mutable_msghead()->set_seq_num(fd_data_gcs_seq++);
        message.mutable_msghead()->set_reserved(0);
        message.mutable_msghead()->set_topic_id(5);
        message.mutable_msghead()->set_msg_id(3);
        message.mutable_msghead()->set_msg_length(fd_data_msg.playload.size());
        message.mutable_msghead()->set_time_stamp(ros::Time::now().toSec());

        message.set_playload(fd_data_msg.playload);

        message.mutable_msghead()->set_tgt_uav_count(1);
        message.mutable_msghead()->add_tgt_uav_id(0);

        string msg_str;
        if(!message.SerializeToString(&msg_str))
        {
            perror("serialize error:");
            return;
        }

        //ROS_INFO("frame_id=%d",message.msghead().seq_num());
        int send_byte = sendOutMsg(msg_str,publisherToGcs,message.msghead().seq_num(),addressToGcs);//将整合的反馈数据发送到地面站
        
		if(ros::Time::now() - timeToShow >= ros::Duration(3)){
			ROS_INFO("publish to send fd_data %d bytes.",send_byte);
			timeToShow = ros::Time::now();
		}

        timeToGcs = ros::Time::now();
    }


    static ros::Time timeToUav = ros::Time::now();
    if(ros::Time::now() - timeToUav >= ros::Duration(0.1))//持续发送的数据需要指定发送频率
    {
        uavMessage::Message message; 

        message.mutable_msghead()->set_stx(254);
        message.mutable_msghead()->set_msg_type(4);
        message.mutable_msghead()->set_cluster_id(cluster_id);
        message.mutable_msghead()->set_src_uav_id(uav_id);
        message.mutable_msghead()->set_seq_num(fd_data_uav_seq++);
        message.mutable_msghead()->set_reserved(0);
        message.mutable_msghead()->set_topic_id(5);
        message.mutable_msghead()->set_msg_id(3);
        message.mutable_msghead()->set_msg_length(fd_data_msg.playload.size());
        message.mutable_msghead()->set_time_stamp(ros::Time::now().toSec());

        message.set_playload(fd_data_msg.playload);

        message.mutable_msghead()->set_tgt_uav_count(other_uav_count);
        for(int i=0;i<other_uav_count;i++)
        {
            message.mutable_msghead()->add_tgt_uav_id(other_uav_id[i]);
        }

        string msg_str;
        if(!message.SerializeToString(&msg_str))
        {
            perror("serialize error:");
            return;
        }

        int send_byte = sendOutMsg(msg_str,publisherToUav,message.msghead().seq_num(),addressToUav);//将反馈数据发送到其他无人机

        timeToUav = ros::Time::now();
    }
}

void feedback_waypoint_callback(const simulation_m100::PubSubUavMsg cmd_msg)//航点反馈：规划结果航点、群体飞行到达航点
{
    uavMessage::Message message; 

    message.mutable_msghead()->set_stx(254);
    message.mutable_msghead()->set_msg_type(4);
    message.mutable_msghead()->set_cluster_id(cluster_id);
    message.mutable_msghead()->set_src_uav_id(uav_id);
    message.mutable_msghead()->set_seq_num(fd_wp_uav_seq++);
    message.mutable_msghead()->set_reserved(0);
    message.mutable_msghead()->set_topic_id(9);
    message.mutable_msghead()->set_msg_id(7);
    message.mutable_msghead()->set_msg_length(cmd_msg.playload.size());
    message.mutable_msghead()->set_time_stamp(ros::Time::now().toSec());
    message.set_playload(cmd_msg.playload);

    message.mutable_msghead()->set_tgt_uav_count(other_uav_count);
    for(int i=0;i<other_uav_count;i++)
    {
        message.mutable_msghead()->add_tgt_uav_id(other_uav_id[i]);
    }
    

    string msg_str;
    if(!message.SerializeToString(&msg_str))
    {
        perror("serialize error:");
        return;
    }

    //ROS_INFO("frame_id=%d",message.msghead().seq_num());
    int send_byte = sendOutMsg(msg_str,publisherToUav,message.msghead().seq_num(),addressToUav);//将航点序列发送到其他无人机
    ROS_INFO("waypoint send %d bytes.",send_byte);

}

void sendPosition(simulation_m100::Position pos_msg)
{
    static ros::Time timeToUav = ros::Time::now();
    if(ros::Time::now() - timeToUav >= ros::Duration(0.1))//持续发送的数据需要指定发送频率
    {
        uwblpsMessage::uwblpsMessage uwb_pos;

        uwb_pos.set_linear_x(pos_msg.x);
        uwb_pos.set_linear_y(pos_msg.y);
        uwb_pos.set_linear_z(pos_msg.z);

        string uwb_pos_str;
        uwb_pos.SerializeToString(&uwb_pos_str);


        uavMessage::Message message; 

        message.mutable_msghead()->set_stx(254);
        message.mutable_msghead()->set_msg_type(4);
        message.mutable_msghead()->set_cluster_id(cluster_id);
        message.mutable_msghead()->set_src_uav_id(uav_id);
        message.mutable_msghead()->set_seq_num(pos_uav_seq++);
        message.mutable_msghead()->set_reserved(0);
        message.mutable_msghead()->set_topic_id(12);
        message.mutable_msghead()->set_msg_id(10);
        message.mutable_msghead()->set_msg_length(uwb_pos_str.size());
        message.mutable_msghead()->set_time_stamp(ros::Time::now().toSec());

        message.set_playload(uwb_pos_str);


        message.mutable_msghead()->set_tgt_uav_count(other_uav_count);
        for(int i=0;i<other_uav_count;i++)
        {
            message.mutable_msghead()->add_tgt_uav_id(other_uav_id[i]);
        }

        string msg_str;
        if(!message.SerializeToString(&msg_str))
        {
            perror("serialize error:");
            return;
        }

        //ROS_INFO("frame_id=%d",message.msghead().seq_num());
        int send_byte = sendOutMsg(msg_str,publisherToUav,message.msghead().seq_num(),addressToUav);//将uwb定位数据发送到其他无人机
        //ROS_INFO("send feedback data %d bytes.",send_byte);

        //uavMessage::Message msg_test;
        //msg_test.ParseFromString(msg_str);

        //uwblpsMessage::uwblpsMessage uwb;
        //uwb.ParseFromString(msg_test.playload());
        //ROS_INFO("(%f,%f,%f)",uwb.linear_x(),uwb.linear_y(),uwb.linear_z());

        timeToUav = ros::Time::now();
    }
}

void subPosCallback(simulation_m100::Position pos_msg)//位置更新
{
    sendPosition(pos_msg);
}

void mapbuffer_callback(simulation_m100::PubSubUavMsg MB_msg)
{
    uavMessage::Message message; 

    message.mutable_msghead()->set_stx(254);
    message.mutable_msghead()->set_msg_type(4);
    message.mutable_msghead()->set_cluster_id(cluster_id);
    message.mutable_msghead()->set_src_uav_id(uav_id);
    message.mutable_msghead()->set_seq_num(mpbuff_uav_seq++);
    message.mutable_msghead()->set_reserved(0);
    message.mutable_msghead()->set_topic_id(13);
    message.mutable_msghead()->set_msg_id(11);
    message.mutable_msghead()->set_msg_length(MB_msg.playload.size());
    message.mutable_msghead()->set_time_stamp(ros::Time::now().toSec());
    
    message.set_playload(MB_msg.playload);
            
    message.mutable_msghead()->set_tgt_uav_count(other_uav_count);
    for(int i=0;i<other_uav_count;i++)
    {
        message.mutable_msghead()->add_tgt_uav_id(other_uav_id[i]);
    }

    string msg_str;
    if(!message.SerializeToString(&msg_str))
    {
        perror("serialize error:");
        return;
    }

    //ROS_INFO("frame_id=%d",message.msghead().seq_num());
    int send_byte = sendOutMsg(msg_str,publisherToUav,message.msghead().seq_num(),addressToUav);//将数据发送到其他无人机
    //ROS_INFO("send data %d bytes.",send_byte);
}

void subFormationInfoCallback(simulation_m100::PubSubUavMsg fm_msg)
{
    uavMessage::Message message; 

    message.mutable_msghead()->set_stx(254);
    message.mutable_msghead()->set_msg_type(4);
    message.mutable_msghead()->set_cluster_id(cluster_id);
    message.mutable_msghead()->set_src_uav_id(uav_id);
    message.mutable_msghead()->set_seq_num(formation_uav_seq++);
    message.mutable_msghead()->set_reserved(0);
    message.mutable_msghead()->set_topic_id(14);
    message.mutable_msghead()->set_msg_id(12);
    message.mutable_msghead()->set_msg_length(fm_msg.playload.size());
    message.mutable_msghead()->set_time_stamp(ros::Time::now().toSec());
    
    message.set_playload(fm_msg.playload);

    message.mutable_msghead()->set_tgt_uav_count(other_uav_count); 
    for(int i=0;i<other_uav_count;i++)
    {
        message.mutable_msghead()->add_tgt_uav_id(other_uav_id[i]);
    }

    string msg_str;
    if(!message.SerializeToString(&msg_str))
    {
        perror("serialize error:");
        return;
    }

    //ROS_INFO("frame_id=%d",message.msghead().seq_num());
    int send_byte = sendOutMsg(msg_str,publisherToUav,message.msghead().seq_num(),addressToUav);//将数据发送到其他无人机
    //ROS_INFO("send data %d bytes.",send_byte);
}

void subUavBaseInfoCallback(simulation_m100::UavBaseInfo uav_base_info){
    cluster_id = uav_base_info.cluster_id;
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

void initOtherUavId()
{
    int error_count = 0;
    while(ros::ok())
    {
        string name = getFileName((other_uav_id_config + uav_name + ".txt").c_str());
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

        ros::Duration(0.01).sleep();
    }
}

int main(int argc,char** argv)
{
    ros::init(argc,argv,"m100Send");
    ros::NodeHandle nh;

    ROS_INFO("m100Send_node started.");

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

    ros::Subscriber feedback_waypoint_sub = nh.subscribe(string(uav_name + "/feedback_waypoint").c_str(),10,&feedback_waypoint_callback);
    ros::Subscriber feedback_data_sub = nh.subscribe(string(uav_name + "/feedback_data").c_str(),10,feedback_data_callback);
    ros::Subscriber global_pos_sub = nh.subscribe(string(uav_name + posTopic).c_str(), 10, subPosCallback);
    ros::Subscriber mapbuffer_sub = nh.subscribe(string(uav_name + "/my_mapbuffer").c_str(),10,&mapbuffer_callback);
    ros::Subscriber formation_sub = nh.subscribe(string(uav_name + "/my_formation_info").c_str(),10,&subFormationInfoCallback);
    ros::Subscriber base_info_sub = nh.subscribe(string(uav_name + "/uav_base_info").c_str(), 10, subUavBaseInfoCallback);
    
    send_info_pub = nh.advertise<simulation_m100::CommInfo>(string(uav_name + "/comm_send_info").c_str(),10);

    initOtherUavId();

    //prepare the publisherToUav and publisherToGcs
    void *context = zmq_init(1);
    publisherToUav = zmq_socket(context,ZMQ_PUB);
    publisherToGcs = zmq_socket(context,ZMQ_PUB);

    //绑定对地面站的发送端口地址
    ros::param::get("~address_to_gcs",addressToGcs);
    ROS_INFO("binding addressToGcs %s",addressToGcs.c_str());
    int error_count = 0;
    int ret;
    while(ros::ok())//bind the address
    {
        ret = zmq_bind(publisherToGcs,addressToGcs.c_str());//return 0 if success

        if(ret != 0)
        {
            perror("bind address error:");

            error_count++;
            if(error_count>3)
            {
                ROS_INFO("Can not bind to %s !",addressToGcs.c_str());
                return -1;
            }

            ros::Duration(1).sleep();
        }

        else
        {
            ROS_INFO("bind success.");
            break;
        }  
    }
    

    //绑定对其他无人机的发送端口地址
    ros::param::get("~address_to_uav",addressToUav);
    ROS_INFO("binding addressToUav %s",addressToUav.c_str());
    error_count = 0;
    while(ros::ok())//bind the address
    {
        ret = zmq_bind(publisherToUav,addressToUav.c_str());//return 0 if success

        if(ret != 0)
        {
            perror("bind address error:");

            error_count++;
            if(error_count>3)
            {
                ROS_INFO("Can not bind to %s !",addressToUav.c_str());
                return -1;
            }

            ros::Duration(1).sleep();
        }

        else
        {
            ROS_INFO("bind success.");
            break;
        }  
    }


	ros::spin();

    zmq_close(publisherToGcs);
    zmq_close(publisherToUav);

    ros::shutdown();
    return 0;
}
