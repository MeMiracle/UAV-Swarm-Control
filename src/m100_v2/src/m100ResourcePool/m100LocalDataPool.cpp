#include <dji_sdk/dji_sdk.h>
#include <demo_flight_control.h>

#include <std_msgs/String.h>  
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/BatteryState.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <m100_v2/Position.h>
#include <zmq.h>

using namespace std;

const float deg2rad = C_PI/180.0;
const float rad2deg = 180.0/C_PI;

double sin37 = sin(37*deg2rad);//offset angle
double cos37 = cos(37*deg2rad);

int seq_num = 0;
string uav_name("");

//实验场地坐标系与自然全局坐标系的转换关系：
//实验场地坐标系在自然全局坐标系原点以东为正x、以北为正y、北偏东夹角为正（度）
int axisID = 1;
double axisOffsetX = 0;
double axisOffsetY = 0;
double axisOffsetZ = 0;
double axisOffsetAngle = 0;
double sinOffsetAngle = 0;
double cosOffsetAngle = 1;

ros::Publisher fd_data_pub;
ros::Publisher fd_attit_pub;
ros::Publisher fd_gps_pub;
ros::Publisher fd_vel_pub;
ros::Publisher fd_accel_pub;
ros::Publisher fd_status_pub;
ros::Publisher gimbal_angle_pub;


pthread_mutex_t atti_mutex = PTHREAD_MUTEX_INITIALIZER;//互斥锁
pthread_mutex_t gps_mutex = PTHREAD_MUTEX_INITIALIZER;//互斥锁
pthread_mutex_t pos_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t status_mutex = PTHREAD_MUTEX_INITIALIZER;//互斥锁
//pthread_mutex_t mode_mutex = PTHREAD_MUTEX_INITIALIZER;//互斥锁
pthread_mutex_t battery_mutex = PTHREAD_MUTEX_INITIALIZER;//互斥锁
pthread_mutex_t vel_mutex = PTHREAD_MUTEX_INITIALIZER;//互斥锁
pthread_mutex_t localpos_mutex = PTHREAD_MUTEX_INITIALIZER;//互斥锁
pthread_mutex_t imu_mutex = PTHREAD_MUTEX_INITIALIZER;//互斥锁

feedbackData::FeedbackData fd_data;

geometry_msgs::Vector3 glo_attitude;
sensor_msgs::NavSatFix glo_current_gps;
m100_v2::Position glo_current_pos;
std_msgs::UInt8 glo_flight_status;
//std_msgs::UInt8 glo_display_mode;
sensor_msgs::BatteryState glo_battery;
geometry_msgs::Vector3Stamped glo_velocity;
//geometry_msgs::PointStamped glo_local_position;
sensor_msgs::Imu glo_imu;


bool isAttiDone = false;
bool isGpsDone = false;
bool isPosDone = false;
bool isStatusDone = false;
//bool isModeDone = false;
bool isBatteryDone = false;
bool isVelDone = false;
//bool isLocalDone = false;
bool isImuDone = false;


//根据四元组姿态数据，解算出绕三个轴旋转的角度
geometry_msgs::Vector3 toEulerAngle(geometry_msgs::Quaternion quat)
{
  geometry_msgs::Vector3 ans;

  tf::Matrix3x3 R_FLU2ENU(tf::Quaternion(quat.x, quat.y, quat.z, quat.w));
  R_FLU2ENU.getRPY(ans.x, ans.y, ans.z);

  return ans;
}

//姿态数据（四元组）
void attitude_callback(const geometry_msgs::QuaternionStamped::ConstPtr& msg)
{
  //pthread_mutex_lock(&atti_mutex);

  geometry_msgs::QuaternionStamped current_atti;
  current_atti = *msg;
  //ROS_INFO("attitude %f %f %f\n", toEulerAngle(current_atti).x, toEulerAngle(current_atti).y, toEulerAngle(current_atti).z);//弧度
  

//   if(current_atti.orientation.z * current_atti.orientation.w >= 0.0)
//   {
//     fd_data.set_yaw(2 * abs(asin(current_atti.orientation.z)));//偏航角/2的正弦值（-1～+1）换算成弧度
//   }
//   else
//   {
//     fd_data.set_yaw(-2 * abs(asin(current_atti.orientation.z)));//偏航角/2的正弦值（-1～+1）换算成弧度
//   }

  glo_attitude = toEulerAngle(current_atti.quaternion);

    if(axisID == 2)
    {
        double z = glo_attitude.z*rad2deg - axisOffsetAngle;

        //转换为：北偏西+0～+180，北偏东-0～~180
        if(abs(z) > 180)
        {
            int n = abs(z) / 360 + 1;
            n = z>0?-n:n;
            z = z + 360 * n;//旋转角度控制
        }

        glo_attitude.z = z * deg2rad;
    }

  fd_attit_pub.publish(glo_attitude);

  isAttiDone = true;

  //pthread_mutex_unlock(&atti_mutex);
}

//gps数据
void gps_callback(const sensor_msgs::NavSatFix::ConstPtr& msg)
{
  //pthread_mutex_lock(&gps_mutex);
  
  glo_current_gps.longitude = msg->longitude;
  glo_current_gps.latitude = msg->latitude;
  glo_current_gps.altitude = msg->altitude;

  fd_gps_pub.publish(glo_current_gps);
  isGpsDone = true;
  //pthread_mutex_unlock(&gps_mutex);
}

void pos_callback(const m100_v2::Position& msg)//位置更新
{
    //pthread_mutex_lock(&pos_mutex);

    glo_current_pos = msg;
    
    isPosDone = true;
    //pthread_mutex_unlock(&pos_mutex);
}

//状态数据
//  ON_GROUND_STANDBY  = 1,
//  TAKEOFF            = 2,
//  IN_AIR_STANDBY     = 3,
//  LANDING            = 4, 
//  FINISHING_LANDING  = 5
void flight_status_callback(const std_msgs::UInt8::ConstPtr& msg)
{
  //pthread_mutex_lock(&status_mutex);
  glo_flight_status = *msg;
  fd_status_pub.publish(glo_flight_status);

  isStatusDone = true;
  //ROS_INFO("status = %d", glo_flight_status.data);
  //pthread_mutex_unlock(&status_mutex);
}

//飞行模式/控制模式？
// void display_mode_callback(const std_msgs::UInt8::ConstPtr& msg)
// {
//   //pthread_mutex_lock(&mode_mutex);
//   glo_display_mode = *msg;
//   isModeDone = true;
//   //pthread_mutex_unlock(&mode_mutex);
// }

//电池电量，有电压/电容/百分百等
void battery_state_callback(const sensor_msgs::BatteryState::ConstPtr& msg)
{
  //pthread_mutex_lock(&battery_mutex);
  glo_battery = *msg;
  isBatteryDone = true;

  //voltage=0,percentage实际为无符号整型,不确定
  // ROS_INFO("voltage=%lf,current=%lf,charge=%lf,capacity=%lf,design_capacity=%lf,percentage=%lf",
  //   glo_battery.voltage,glo_battery.current,glo_battery.charge,glo_battery.capacity,glo_battery.design_capacity,glo_battery.percentage);
  //pthread_mutex_unlock(&battery_mutex);
}

//速度数据（机身速度还是对地速度？）
void velocity_callback(const geometry_msgs::Vector3Stamped::ConstPtr& msg)
{
  //pthread_mutex_lock(&vel_mutex);
  glo_velocity = *msg;//对地速度，单位m/s

    if(axisID == 2)//全局对地速度转成场地对地速度
    {
        double x = glo_velocity.vector.x * cosOffsetAngle - glo_velocity.vector.y * sinOffsetAngle;
        double y = glo_velocity.vector.x * sinOffsetAngle + glo_velocity.vector.y * cosOffsetAngle;

        glo_velocity.vector.x = x;
        glo_velocity.vector.y = y;
    }

  //ROS_INFO("glo_velocity %f %f %f\n", glo_velocity.vector.x, glo_velocity.vector.y, glo_velocity.vector.z);
  fd_vel_pub.publish(glo_velocity);

  isVelDone = true;
  //pthread_mutex_unlock(&vel_mutex);
}

//no data
// void local_position_callback(const geometry_msgs::PointStamped::ConstPtr& msg)
// {
//   //pthread_mutex_lock(&localpos_mutex);
//   glo_local_position = *msg;
//   isLocalDone = true;
//   //ROS_INFO("glo_local_position: %f %f %f", glo_local_position.point.x, glo_local_position.point.y, glo_local_position.point.z);
//   //pthread_mutex_unlock(&localpos_mutex);
// }

void gimbalAngleCallback(const geometry_msgs::Vector3Stamped::ConstPtr& msg)
{
    geometry_msgs::Vector3Stamped gimbal_angle_msg;
    gimbal_angle_msg = *msg;
    cout << "Gimbal angle is ;" << gimbal_angle_msg.vector.x << "; " << gimbal_angle_msg.vector.y << "; " << gimbal_angle_msg.vector.z << endl;

    m100_v2::PubSubUavMsg gimbal_angle;

    myTaskMessage::TaskMessage gimbal_angle_task;

    myTaskMessage::MyPoint* mp0;
    mp0 = gimbal_angle_task.add_point();
    mp0->set_x(gimbal_angle_msg.vector.x);
    mp0->set_y(gimbal_angle_msg.vector.y);
    mp0->set_z(gimbal_angle_msg.vector.z);
    mp0->set_yaw(0);
    mp0->set_type(2);

    gimbal_angle_task.SerializeToString(&gimbal_angle.playload);
    gimbal_angle_pub.publish(gimbal_angle);

}


void gimbal_target_callback(m100_v2::PubSubUavMsg gimbal_target)
{
    myTaskMessage::TaskMessage gimbal_target_task;
	gimbal_target_task.ParseFromString(gimbal_target.playload);

	//ROS_INFO("received waypoints!");
	if (gimbal_target.src_uav_id == 0)//初始化目标点（初次从地面接收到目标点）
	{
		for (int i = 0; i<gimbal_target_task.point_size(); i++)
		{
			myTaskMessage::MyPoint waypoint;
			waypoint = gimbal_target_task.point(i);

			ROS_INFO("target:%f,%f,%f,%d", waypoint.x(), waypoint.y(), waypoint.z(), waypoint.mark());
		}
	}
    
}

//imu数据
void imu_callback(const sensor_msgs::Imu::ConstPtr& msg)
{
  //pthread_mutex_lock(&imu_mutex);
  glo_imu = *msg;

  if(axisID == 2)//机身加速度转成场地加速度
    {
        double x = glo_imu.linear_acceleration.x * sinOffsetAngle + glo_imu.linear_acceleration.y * cosOffsetAngle;
        double y = glo_imu.linear_acceleration.x * cosOffsetAngle - glo_imu.linear_acceleration.y * sinOffsetAngle;
    
        glo_imu.linear_acceleration.x = x;
        glo_imu.linear_acceleration.y = y;
    }
  
  //ROS_INFO("angular_velocity %f %f %f\n", glo_imu.angular_velocity.x, glo_imu.angular_velocity.y, glo_imu.angular_velocity.z);//角速度，单位 弧度/s
  //ROS_INFO("linear_acceleration %f %f %f\n", glo_imu.linear_acceleration.x, glo_imu.linear_acceleration.y, glo_imu.linear_acceleration.z);//单位g
  
  fd_accel_pub.publish(glo_imu.linear_acceleration);
  
  isImuDone = true;
  //pthread_mutex_unlock(&imu_mutex);
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

ofstream gps_log_str;
ofstream pos_log_str;
ofstream vel_log_str;
ofstream atti_delay_str;
ofstream gps_delay_str;
ofstream pos_delay_str;
ofstream vel_delay_str;
ofstream accel_delay_str;
ofstream PJPositionData_str;

void openLogFiles()
{
    char gps_log[] = "m100_v2/log/gps_log_";
    string gps_log_file_name(getDirectory() + gps_log + uav_name.c_str() + ".txt");
    
    gps_log_str.open(gps_log_file_name.c_str(), ios::out | ios::app);
 
    if(gps_log_str)
    {
        gps_log_str << ros::Time::now() << endl;
    }
 
    char pos_log[] = "m100_v2/log/pos_log_";
    string pos_log_file_name(getDirectory() + pos_log + uav_name.c_str() + ".txt");
    
    pos_log_str.open(pos_log_file_name.c_str(), ios::out | ios::app);
 
    if(pos_log_str)
    {
        pos_log_str << ros::Time::now() << endl;
    }
 
    char vel_log[] = "m100_v2/log/vel_log_";
    string vel_log_file_name(getDirectory() + vel_log + uav_name.c_str() + ".txt");
    
    vel_log_str.open(vel_log_file_name.c_str(), ios::out | ios::app);
 
    if(vel_log_str)
    {
        vel_log_str << ros::Time::now() << endl;
    }
 
 
    char atti_delay[] = "m100_v2/log/atti_delay_";
    string atti_delay_file_name(getDirectory() + atti_delay + uav_name.c_str() + ".txt");
    
    atti_delay_str.open(atti_delay_file_name.c_str(), ios::out | ios::app);
 
    
    if(atti_delay_str)
    {
        atti_delay_str << ros::Time::now() << endl;
    }
 
 
    char gps_delay[] = "m100_v2/log/gps_delay_";
    string gps_delay_file_name(getDirectory() + gps_delay + uav_name.c_str() + ".txt");
    
    gps_delay_str.open(gps_delay_file_name.c_str(), ios::out | ios::app);
 
    
    if(gps_delay_str)
    {
        gps_delay_str << ros::Time::now() << endl;
    }
 
 
    char pos_delay[] = "m100_v2/log/pos_delay_";
    string pos_delay_file_name(getDirectory() + pos_delay + uav_name.c_str() + ".txt");
    
    pos_delay_str.open(pos_delay_file_name.c_str(), ios::out | ios::app);
 
    
    if(pos_delay_str)
    {
        pos_delay_str << ros::Time::now() << endl;
    }
 
 
    char vel_delay[] = "m100_v2/log/vel_delay_";
    string vel_delay_file_name(getDirectory() + vel_delay + uav_name.c_str() + ".txt");
    
    vel_delay_str.open(vel_delay_file_name.c_str(), ios::out | ios::app);
 
    
    if(vel_delay_str)
    {
        vel_delay_str << ros::Time::now() << endl;
    }
 
 
    char accel_delay[] = "m100_v2/log/accel_delay_";
    string accel_delay_file_name(getDirectory() + accel_delay + uav_name.c_str() + ".txt");
    
    accel_delay_str.open(accel_delay_file_name.c_str(), ios::out | ios::app);
 
    
    if(accel_delay_str)
    {
        accel_delay_str << ros::Time::now() << endl;
    }
 
 
    char PJPositionData[] = "m100_v2/log/PositionData.txt";
    string PJPositionData_file_name(getDirectory() + PJPositionData);
    
    PJPositionData_str.open(PJPositionData_file_name.c_str(), ios::out | ios::app);
 
    
    if(PJPositionData_str)
    {
        PJPositionData_str << ros::Time::now() << endl;
    }
}
 
void closeLogFiles()
{
   gps_log_str.close();
   gps_log_str.close();
    vel_log_str.close();
    atti_delay_str.close();
    gps_delay_str.close();
    pos_delay_str.close();
    vel_delay_str.close();
    accel_delay_str.close();
   PJPositionData_str.close();
}
 
void m100HandleFeedbackData()
{
    static bool isNeedToSend = false;
 
    if(isAttiDone)
    {
        //pthread_mutex_lock(&atti_mutex);
 
        fd_data.set_roll(glo_attitude.x*rad2deg);//弧度转成角度，偏北为正，偏南为负
        fd_data.set_pitch(glo_attitude.y*rad2deg);
        fd_data.set_yaw(glo_attitude.z*rad2deg);
 
        isAttiDone = false;
        isNeedToSend = true;
 
        //pthread_mutex_unlock(&atti_mutex);
 
        
        if(atti_delay_str)
        {
         static double atti_time = ros::Time::now().toSec();
            atti_delay_str << (ros::Time::now().toSec()-atti_time)*1000 << endl;
            atti_time = ros::Time::now().toSec();
        }
    }
 
    if(isGpsDone)
    {
        //pthread_mutex_lock(&gps_mutex);
        //ROS_INFO("handlefeedback got the lock: VelDone");
 
        fd_data.set_lon(glo_current_gps.longitude);
        fd_data.set_lat(glo_current_gps.latitude);
        fd_data.set_alt(glo_current_gps.altitude);//高度，米
 
        isGpsDone = false;
        isNeedToSend = true;
 
        //pthread_mutex_unlock(&gps_mutex);
 
      if(gps_log_str)
      {
         gps_log_str << fd_data.lon() << " " << fd_data.lat() << " " << fd_data.alt() << endl;
      }
        
        if(gps_delay_str)
        {
         static double gps_time = ros::Time::now().toSec();
            gps_delay_str << (ros::Time::now().toSec()-gps_time)*1000 << endl;
            gps_time = ros::Time::now().toSec();
        }
    }
 
    if(isPosDone)
    {
        //pthread_mutex_lock(&pos_mutex);
 
        fd_data.set_px(glo_current_pos.x);
        fd_data.set_py(glo_current_pos.y);
        fd_data.set_pz(glo_current_pos.z);
 
        isPosDone = false;
        isNeedToSend = true;
 
        //pthread_mutex_unlock(&pos_mutex);
 
      if(pos_log_str)
      {
         pos_log_str << fd_data.px() << " " << fd_data.py() << " " << fd_data.pz() << endl;
      }
 
        
        if(pos_delay_str)
        {
         static double pos_time = ros::Time::now().toSec();
            pos_delay_str << (ros::Time::now().toSec()-pos_time)*1000 << endl;
            pos_time = ros::Time::now().toSec();
        }
    }
 
    if(isStatusDone)
    {
        //pthread_mutex_lock(&status_mutex);
        
        //状态数据
        //  ON_GROUND_STANDBY  = 1,
        //  TAKEOFF            = 2,
        //  IN_AIR_STANDBY     = 3,
        //  LANDING            = 4, 
        //  FINISHING_LANDING  = 5
        fd_data.set_state(glo_flight_status.data);
 
        isStatusDone = false;
        isNeedToSend = true;
 
        //pthread_mutex_unlock(&status_mutex);
    }
 
    // if(isModeDone)
    // {
 
    // }
 
    if(isBatteryDone)
    {
        //pthread_mutex_lock(&battery_mutex);
 
        fd_data.set_batterypercent(glo_battery.percentage);//？？
 
        isBatteryDone = false;
        isNeedToSend = true;
 
        //pthread_mutex_unlock(&battery_mutex);
    }
 
    if(isVelDone)
    {
        //pthread_mutex_lock(&vel_mutex);
 
        fd_data.set_vx(glo_velocity.vector.x);//对地速度，m/s
        fd_data.set_vy(glo_velocity.vector.y);
        fd_data.set_vz(glo_velocity.vector.z);
 
        isVelDone = false;
        isNeedToSend = true;
 
        //pthread_mutex_unlock(&vel_mutex);
 
        if(vel_log_str)
        {
            vel_log_str << fd_data.vx() << " " << fd_data.vy() << " " << fd_data.vz() << endl;      
        }
 
        
        if(vel_delay_str)
        {
         static double vel_time = ros::Time::now().toSec();
            vel_delay_str << (ros::Time::now().toSec()-vel_time)*1000 << endl;
            vel_time = ros::Time::now().toSec();
        }
    }
 
    // if(isLocalDone)
    // {
 
    // }
 
    if(isImuDone)
    {      
        //pthread_mutex_lock(&imu_mutex);
 
        fd_data.set_vw(glo_imu.angular_velocity.z*rad2deg);//角速度 弧度/秒 -> 角度/秒
 
        fd_data.set_ax(glo_imu.linear_acceleration.x/9.801);//单位：g 转成 米平方/秒
        fd_data.set_ay(glo_imu.linear_acceleration.y/9.801);//有噪声
        fd_data.set_az(-glo_imu.linear_acceleration.z/9.801);//有噪声
 
        isImuDone = false;
        isNeedToSend = true;
 
        //pthread_mutex_unlock(&imu_mutex);
 
        
        if(accel_delay_str)
        {
         static double accel_time = ros::Time::now().toSec();
            accel_delay_str << (ros::Time::now().toSec()-accel_time)*1000 << endl;
            accel_time = ros::Time::now().toSec();
        }
    }
 
 
    if(isNeedToSend)//
    {
        m100_v2::PubSubUavMsg fd_data_str;
        
        if(!fd_data.SerializeToString(&fd_data_str.playload))
        {
            perror("feedbackdata serializing error:");
        }
        else
        {
            //ROS_INFO("batteryPercent=%f,state=%d",fd_data.batterypercent(),fd_data.state());
            //ROS_INFO("gps position:(%lf,%lf,%lf)",fd_data.lon(),fd_data.lat(),fd_data.alt());
            //ROS_INFO("dist position:(%lf,%lf,%lf)",fd_data.px(),fd_data.py(),fd_data.pz());
            //ROS_INFO("roll=%lf,pitch=%lf,yaw=%lf",fd_data.roll(),fd_data.pitch(),fd_data.yaw());
            //ROS_INFO("vx=%lf,vy=%lf,vz=%lf,vw=%lf",fd_data.vx(),fd_data.vy(),fd_data.vz(),fd_data.vw());
            //ROS_INFO("ax=%lf,ay=%lf,az=%lf\n",fd_data.ax(),fd_data.ay(),fd_data.az());
         if(PJPositionData_str)
         {
            PJPositionData_str<<fd_data.lon()<<" "<<fd_data.lat()<<" "<<fd_data.alt()<<" "<<fd_data.vx() << " " << fd_data.vy() << " " << fd_data.vz()<<" ";
            PJPositionData_str<<fd_data.px()<<" "<<fd_data.py()<<" "<<fd_data.pz()<<" "<<fd_data.ax() << " " << fd_data.ay() << " " << fd_data.az()<<" ";
            PJPositionData_str<<fd_data.roll()<<" "<<fd_data.pitch()<<" "<<fd_data.yaw()<<endl;
         }
 
            fd_data_pub.publish(fd_data_str);
 
            isNeedToSend = false;
        }
    }
}

int main(int argc,char** argv)
{
    ros::init(argc,argv,"m100LocalDataPool");
    ros::NodeHandle nh;

    ROS_INFO("m100LocalDataPool_node started.");


    ros::param::get("~uav_name",uav_name);


    ros::param::get("~axis_id",axisID);
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

    string posTopic("/global_position");
    if(axisID == 2)
    {
        posTopic = string("/local_position");
    }

    //M100 flight data
    ros::Subscriber attitudeSub = nh.subscribe("dji_sdk/attitude", 10, &attitude_callback);
    ros::Subscriber gpsSub      = nh.subscribe("dji_sdk/gps_position", 10, &gps_callback);
    ros::Subscriber flightStatusSub = nh.subscribe("dji_sdk/flight_status", 10, &flight_status_callback);
    //ros::Subscriber displayModeSub = nh.subscribe("dji_sdk/display_mode", 10, &display_mode_callback);
    ros::Subscriber batteryStateSub = nh.subscribe("/dji_sdk/battery_state", 10, &battery_state_callback);
    ros::Subscriber velocitySub = nh.subscribe("dji_sdk/velocity", 10, &velocity_callback);
    ros::Subscriber gimbalAngleSub = nh.subscribe("dji_sdk/gimbal_angle", 10, &gimbalAngleCallback);
    //ros::Subscriber localPositionSub = nh.subscribe("dji_sdk/local_position", 10, &local_position_callback);
    ros::Subscriber imuleSub = nh.subscribe("dji_sdk/imu", 10, &imu_callback);

    ros::Subscriber gimbalTargetSub = nh.subscribe(string(uav_name + "/gimbal_target_info").c_str(), 10, &gimbal_target_callback);
     
    ros::Subscriber posSub = nh.subscribe((uav_name + posTopic).c_str(), 10, &pos_callback);

    fd_attit_pub = nh.advertise<geometry_msgs::Vector3>(string(uav_name + "/attitude").c_str(),10);
    fd_gps_pub = nh.advertise<sensor_msgs::NavSatFix>(string(uav_name + "/gps").c_str(),10);
    fd_vel_pub = nh.advertise<geometry_msgs::Vector3Stamped>(string(uav_name + "/velocity").c_str(),10);
    fd_accel_pub = nh.advertise<geometry_msgs::Vector3>(string(uav_name + "/acceleration").c_str(),10);
    fd_status_pub = nh.advertise<std_msgs::UInt8>(string(uav_name + "/flight_status").c_str(),10);
    gimbal_angle_pub = nh.advertise<m100_v2::PubSubUavMsg>(string(uav_name + "/gimbal_angle_info").c_str(),10);

    fd_data_pub = nh.advertise<m100_v2::PubSubUavMsg>(string(uav_name + "/feedback_data").c_str(),10);
    
    //pthread_t handleFeedbackData_thread;
    //pthread_create(&handleFeedbackData_thread,NULL,m100HandleFeedbackData,NULL);
    
    //ros::spin();

    openLogFiles();

    ros::Rate loop_rate(100);
    while(ros::ok())
    {
        ros::spinOnce();

        m100HandleFeedbackData();

        loop_rate.sleep();
    }

    closeLogFiles();

    pthread_mutex_destroy(&atti_mutex);
    pthread_mutex_destroy(&gps_mutex);
    pthread_mutex_destroy(&pos_mutex);
    pthread_mutex_destroy(&status_mutex);
    //pthread_mutex_destroy(&mode_mutex);
    pthread_mutex_destroy(&battery_mutex);
    pthread_mutex_destroy(&vel_mutex);
    pthread_mutex_destroy(&localpos_mutex);
    pthread_mutex_destroy(&imu_mutex);

    ros::shutdown();
    return 0;
}
