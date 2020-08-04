#include <ros/ros.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <zmq.h>
#include "../include/delay/delay.pb.h"
#include "../include/delay/delay.pb.cc"
#include <geometry_msgs/Vector3Stamped.h>
#include <delay/Position.h>

using namespace std;
char subaddr[30] = "tcp://125.216.243.77:7050";  //ip of gcs
char uavpubaddr[30] = "tcp://*:7051";

int uav_id = 1;

double lat;//纬度,或者y轴距离
double lon;//经度，或者x轴距离
double alt;//高度，米
double vx;//x轴线速度，米/秒
double vy;//y轴线速度，米/秒
double vz;//z轴线速度，米/秒


void velocity_callback(const geometry_msgs::Vector3Stamped& vel)//全局对地速度
{
    vy = vel.vector.y;
    vx = vel.vector.x;
    vz = vel.vector.z;
    //ROS_INFO("vz = %lf",vz);
}

void pos_callback(const delay::Position& msg)//位置更新
{
    lat = msg.y;
    lon = msg.x;
    alt = msg.z;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "delayuav");
  ros::NodeHandle nh;

  string uavName("m100");
  ros::param::get("~uav_name",uavName);

  int axisID = 1;
  ros::param::get("~axis_id",axisID);

  ROS_INFO("uavName=%s,asix_id=%d",uavName.c_str(),axisID);
  
  string posTopic("/global_position");
  if(axisID == 2)
  {
      posTopic = string("/local_position");
  }

  ros::Subscriber vel_sub = nh.subscribe(string(uavName + "/velocity"),10,&velocity_callback);
  ros::Subscriber posSub = nh.subscribe((uavName + posTopic).c_str(), 10, &pos_callback);

  void* ctx = zmq_ctx_new();

  void* subscriber = zmq_socket(ctx, ZMQ_SUB);
  if(0 == zmq_connect(subscriber, subaddr)){
    ROS_INFO("subscriber connect success");//return 0 if success
  }else{
    perror("subscriber connect failed:");
    return -1;
  }
  zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);

  void* publisher = zmq_socket(ctx, ZMQ_PUB);
  if(0 == zmq_bind(publisher,uavpubaddr)){
    ROS_INFO("publisher bind success");//return 0 if success
  }else{
    perror("publisher bind failed:");
    return -1;
  }

  delayMessage::DelayMsg delaymsg;

  ros::Rate loop_rate(100);
  while (ros::ok()) {
    delaymsg.Clear();
    zmq_msg_t recv_msg;
    zmq_msg_init(&recv_msg);
    int recv_byte = zmq_msg_recv(&recv_msg,subscriber,0);//ZMQ_DONTWAIT

    if(recv_byte > 0)
    {
      ROS_INFO("receive message (%d bytes) success.",recv_byte);

      string str;
      str.assign((char*)zmq_msg_data(&recv_msg),recv_byte);
      zmq_msg_close(&recv_msg);
    //    int index = str.find_first_of('\0');
    //    ROS_INFO("%d",index);
    //    delaymsg.ParseFromString(str.substr(0,index));
      delaymsg.ParseFromString(str);
      ROS_INFO("msg_id:%d send time:%.9lf",delaymsg.msg_id(),delaymsg.send_time());

      delaymsg.set_uav_id(uav_id);
      delaymsg.set_lat(lat);
      delaymsg.set_lon(lon);
      delaymsg.set_alt(alt);
      delaymsg.set_vx(vx);
      delaymsg.set_vy(vy);
      delaymsg.set_vz(vz);
      string send_str = delaymsg.SerializeAsString();
      ROS_INFO("%d",send_str.size());

      //string转成zmq_msg_t
      zmq_msg_t send_msg;
      zmq_msg_init_size(&send_msg,256);
      memcpy(zmq_msg_data(&send_msg),send_str.c_str(),send_str.size());

      //发送zmq_msg_t
      int send_byte = zmq_msg_send(&send_msg,publisher,0);
      ROS_INFO("send message (%d bytes) success.",send_byte);
      zmq_msg_close(&send_msg);

      //delaymsg.ParseFromString(send_str);
      //ROS_INFO("vz = %lf",delaymsg.vz());
    }

    ros::spinOnce();
    loop_rate.sleep();
  }


  zmq_close(subscriber);
  zmq_ctx_destroy(ctx);

ros::shutdown();
return 0;
}
