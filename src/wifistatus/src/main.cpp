#include <string>
using std::string;

#include "wifistatus/callback.h"


int main(int argc, char **argv)
{
	ros::init(argc,argv,"wifi");//创建节点

  	ros::NodeHandle ros_nh;//创建节点句柄

	string uav_name("uav1");
	ros::param::get("~uav_name",uav_name);

	int axisID = 1;
	ros::param::get("~axis_id", axisID);
	string posTopic("/global_position");

	if (axisID == 2)
	{
		posTopic = string("/local_position");
	}

	// Subscribe to messages of UAV status
  	ros::Subscriber rossub_attitude = ros_nh.subscribe(string(uav_name + "/attitude").c_str(), 10, &attitude_callback);
  	ros::Subscriber rossub_position = ros_nh.subscribe((uav_name + posTopic).c_str(), 10, &position_callback);
  	ros::Subscriber rossub_velocity = ros_nh.subscribe(string(uav_name + "/velocity").c_str(), 10, &velocity_callback);

	//Subscribe to message of Send/Recv message
	ros::Subscriber rossub_sendmsg = ros_nh.subscribe(string(uav_name + "/comm_send_info").c_str(),10,&sendmsg_callback);
	ros::Subscriber rossub_recvmsg = ros_nh.subscribe(string(uav_name + "/comm_recv_info").c_str(),10,&recvmsg_callback);
	
	ros::spin();

	ros::shutdown();

	return 1;
}