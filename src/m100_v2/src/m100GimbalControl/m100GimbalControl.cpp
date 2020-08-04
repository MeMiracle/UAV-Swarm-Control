#include <ros/ros.h>
#include <dji_sdk/dji_sdk.h>
#include <demo_camera_gimbal.h>

int main(int argc, char **argv)
{
    //gimbal_angle_cmd_subscriber = nh.subscribe<dji_sdk::Gimbal>("dji_sdk/gimbal_angle_cmd", 10, &DJISDKNode::gimbalAngleCtrlCallback, this);
    //gimbal_speed_cmd_subscriber = nh.subscribe<geometry_msgs::Vector3Stamped>("dji_sdk/gimbal_speed_cmd", 10, &DJISDKNode::gimbalSpeedCtrlCallback, this);

    ros::init(argc,argv,"m100GimbalControl");
    ros::NodeHandle nh;

    ros::Publisher gimbal_angle_cmd = nh.advertise<dji_sdk::Gimbal>("dji_sdk/gimbal_angle_cmd", 10);
    ros::Publisher gimbal_speed_cmd = nh.advertise<geometry_msgs::Vector3Stamped>("dji_sdk/gimbal_speed_cmd", 10);

    ROS_INFO("Gimbal Speed Description: \n\n"
            "Roll  - unit rad/s input rate [-pi, pi]\n"
            "Pitch - unit rad/s input rate [-pi, pi]\n"
            "Yaw   - unit rad/s input rate [-pi, pi]\n\n");

    ROS_INFO("Setting Roll rate to 10 deg/s, Pitch rate to 5 deg/s, Yaw Rate to -20 deg/s.");
    geometry_msgs::Vector3Stamped gimbalSpeed;
    gimbalSpeed.vector.y = DEG2RAD(10);
    gimbalSpeed.vector.x = DEG2RAD(5);
    gimbalSpeed.vector.z = DEG2RAD(-20);
 
    // gimbal_speed_cmd.publish(gimbalSpeed);

    return 0;
}