#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PointStamped.h>
#include <std_msgs/Empty.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/Range.h>
#include <feedbackdata.pb.h>
#include <feedbackdata.pb.cc>
#include <uavmessage.pb.h>
#include <uavmessage.pb.cc>
#include <simulation_m100/PubSubUavMsg.h>
#include <simulation_m100/Position.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <pthread.h>
#include <string>
#include <tf/tf.h>

using namespace std;

#define C_EARTH (double)6378137.0
#define C_PI (double)3.141592653589793
const float deg2rad = C_PI/180.0;
const float rad2deg = 180.0/C_PI;

string uav_name("");

//实验场地坐标系与自然全局坐标系的转换关系：
//实验场地坐标系在自然全局坐标系原点以东为正x、以北为正y、北偏东夹角为负（度）
int axisID = 1;
double axisOffsetX = 0;
double axisOffsetY = 0;
double axisOffsetZ = 0;
double axisOffsetAngle = 0;
double sinOffsetAngle = 0;
double cosOffsetAngle = 1;

ros::Publisher uav_attit_pub;
ros::Publisher uav_gps_pub;
ros::Publisher uav_vel_pub;
ros::Publisher uav_accel_pub;

ros::Publisher uav_data_pub;

// pthread_mutex_t nsf_mutex = PTHREAD_MUTEX_INITIALIZER;//线程锁
// pthread_mutex_t vel_mutex = PTHREAD_MUTEX_INITIALIZER;//线程锁
// pthread_mutex_t imu_mutex = PTHREAD_MUTEX_INITIALIZER;//线程锁
// pthread_mutex_t pos_mutex = PTHREAD_MUTEX_INITIALIZER;//线程锁

feedbackData::FeedbackData fd_data;
bool isNsfDone = false;
bool isVelDone = false;
bool isImuDone = false;
bool isPosDone = false;

void getOdometry(const nav_msgs::Odometry& odo)//里程计数据
{
    //ROS_INFO("p_x=%lf,p_y=%lf,p_z=%lf\n",odo.pose.pose.position.x,odo.pose.pose.position.y,odo.pose.pose.position.z);
    //订阅不到数据
}

sensor_msgs::NavSatFix glo_nsf;
void getFix(const sensor_msgs::NavSatFix& nsf)//GPS位置数据
{
    //ROS_INFO("lon=%lf,lat=%lf,al=%lf\n",nsf.longitude,nsf.latitude,nsf.altitude);
    //室内场景也可用
    //pthread_mutex_lock(&nsf_mutex);//线程锁，防止争用
    //ROS_INFO("getFix got the lock");

    glo_nsf.longitude = nsf.latitude;//gazebo中将经度和纬度进行了转置
    glo_nsf.latitude = nsf.longitude;
    glo_nsf.altitude = nsf.altitude;

    uav_gps_pub.publish(glo_nsf);

    isNsfDone = true;
    //pthread_mutex_unlock(&nsf_mutex);
}

simulation_m100::Position glo_pos;
void getUavPos(const simulation_m100::Position& pos_msg)//全局坐标系的位置更新
{
    //pthread_mutex_lock(&pos_mutex);

    glo_pos = pos_msg;
    isPosDone = true;

    //pthread_mutex_unlock(&pos_mutex);
}

geometry_msgs::Vector3Stamped glo_vel;
void getVelocity(const geometry_msgs::Vector3Stamped& vel)//速度
{
    //ROS_INFO("v_x=%lf,v_y=%lf,v_z=%lf\n",vel.vector.x,vel.vector.y,vel.vector.z);
    //pthread_mutex_lock(&vel_mutex);
    //ROS_INFO("getVelocity got the lock");
    glo_vel = vel;

    if(axisID == 2)
    {
        double x = glo_vel.vector.x * cosOffsetAngle - glo_vel.vector.y * sinOffsetAngle;
        double y = glo_vel.vector.x * sinOffsetAngle + glo_vel.vector.y * cosOffsetAngle;
    
        glo_vel.vector.x = x;
        glo_vel.vector.y = y;
    }

    uav_vel_pub.publish(glo_vel);

    isVelDone = true;
    //pthread_mutex_unlock(&vel_mutex);
}

sensor_msgs::Imu glo_imu;
geometry_msgs::Vector3 glo_attitude;
void getImu(const sensor_msgs::Imu& imu)
{
    //ROS_INFO("ori_x=%lf,ori_y=%lf,ori_z=%lf,ori_w=%lf",imu.orientation.x,imu.orientation.y,imu.orientation.z,imu.orientation.w);
    //ROS_INFO("ang_vel_x=%lf,ang_vel_y=%lf,ang_vel_z=%lf",imu.angular_velocity.x,imu.angular_velocity.y,imu.angular_velocity.z);
    //ROS_INFO("lin_acc_x=%lf,lin_acc_y=%lf,lin_acc_z=%lf\n",imu.linear_acceleration.x,imu.linear_acceleration.y,imu.linear_acceleration.z);
    //pthread_mutex_lock(&imu_mutex);
    //ROS_INFO("getImu got the lock");
    glo_imu = imu;

    // if(glo_imu.orientation.z * glo_imu.orientation.w >= 0.0)
    // {
    //     fd_data.set_yaw(2 * abs(asin(glo_imu.orientation.z)));//偏航角/2的正弦值（-1～+1）换算成弧度
    // }
    // else
    // {
    //     fd_data.set_yaw(-2 * abs(asin(glo_imu.orientation.z)));//偏航角/2的正弦值（-1～+1）换算成弧度
    // }

    tf::Matrix3x3 R_FLU2ENU(tf::Quaternion(glo_imu.orientation.x, glo_imu.orientation.y, glo_imu.orientation.z, glo_imu.orientation.w));
    R_FLU2ENU.getRPY(glo_attitude.y, glo_attitude.x, glo_attitude.z);//gazebo修正
    glo_attitude.y = -glo_attitude.y;

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
    
    uav_attit_pub.publish(glo_attitude);


    // if(axisID == 2)
    // {
        //double x = glo_imu.linear_acceleration.x * cosOffsetAngle - glo_imu.linear_acceleration.y * sinOffsetAngle;
        //double y = glo_imu.linear_acceleration.x * sinOffsetAngle + glo_imu.linear_acceleration.y * cosOffsetAngle;
        
        double x = glo_imu.linear_acceleration.x * cos(-glo_attitude.z) + glo_imu.linear_acceleration.y * sin(-glo_attitude.z);
        double y =-glo_imu.linear_acceleration.x * sin(-glo_attitude.z) + glo_imu.linear_acceleration.y * cos(-glo_attitude.z);

        glo_imu.linear_acceleration.x = -x;
        glo_imu.linear_acceleration.y = -y;
    // }

    uav_accel_pub.publish(glo_imu.linear_acceleration);

    isImuDone = true;
    //pthread_mutex_unlock(&imu_mutex);
}

void getBiasImu(const sensor_msgs::Imu& bias_imu)
{
    //ROS_INFO("bias:ori_x=%lf,ori_y=%lf,ori_z=%lf,ori_w=%lf",bias_imu.orientation.x,bias_imu.orientation.y,bias_imu.orientation.z,bias_imu.orientation.w);
    //ROS_INFO("bias:ang_vel_x=%lf,ang_vel_y=%lf,ang_vel_z=%lf",bias_imu.angular_velocity.x,bias_imu.angular_velocity.y,bias_imu.angular_velocity.z);
    //ROS_INFO("bias:lin_acc_x=%lf,lin_acc_y=%lf,lin_acc_z=%lf\n",bias_imu.linear_acceleration.x,bias_imu.linear_acceleration.y,bias_imu.linear_acceleration.z);
    //无相应变化
}

void getMag(const geometry_msgs::Vector3Stamped& mag)
{
    //ROS_INFO("mag_x=%lf,mag_y=%lf,mag_z=%lf\n",mag.vector.x,mag.vector.y,mag.vector.z);
    //与x轴和y轴旋转角度有关
}

void getSonar(const sensor_msgs::Range& sonar)
{
    //ROS_INFO("sonar_field=%f,sonar_min=%f,sonar_max=%f,sonar_range=%f\n",sonar.field_of_view,sonar.min_range,sonar.max_range,sonar.range);
    //无数据
}

void getHeight(const geometry_msgs::PointStampedConstPtr& height)
{
    //无数据
}

void getEuler(const geometry_msgs::Vector3Stamped& euler)
{
    //ROS_INFO("euler_x=%lf,euler_y=%lf,euler_z=%lf\n",euler.vector.x,euler.vector.y,euler.vector.z);
    //与x轴和y轴加速度有关
}

void getTra(const nav_msgs::Path& path)
{
    //for()
    //{
    //    ROS_INFO("p_x=%f,p_y=%f,p_z=%f",path.poses.pose.position.x,path.poses.pose.position.y,path.poses.pose.position.z);
    //    ROS_INFO("o_x=%f,o_y=%f,o_z=%f,o_w=%f\n",path.poses.pose.orientation.x,path.poses.pose.orientation.y,path.poses.pose.orientation.z,path.poses.pose.orientation.w);
    //}
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

void openLogFiles()
{
    char gps_log[] = "simulation_m100/log/gps_log_";
    string gps_file_name(getDirectory() + gps_log + uav_name.c_str() + ".txt");
    gps_log_str.open(gps_file_name.c_str(),ios::out | ios::app);
    if(gps_log_str)
    {
        gps_log_str << ros::Time::now() << endl;
    }

    char pos_log[] = "simulation_m100/log/pos_log_";
    string pos_file_name(getDirectory() + pos_log + uav_name.c_str() + ".txt");
    pos_log_str.open(pos_file_name.c_str(), ios::out | ios::app);
    if(pos_log_str)
    {
        pos_log_str << ros::Time::now() << endl;
    }

    char vel_log[] = "simulation_m100/log/vel_log_";
    string vel_log_file_name(getDirectory() + vel_log + uav_name.c_str() + ".txt");
    vel_log_str.open(vel_log_file_name.c_str(), ios::out | ios::app);
    if(vel_log_str)
    {
        vel_log_str << ros::Time::now() << endl;
    }


    char atti_delay[] = "simulation_m100/log/atti_delay_";
    string atti_delay_file_name(getDirectory() + atti_delay + uav_name.c_str() + ".txt");   
    atti_delay_str.open(atti_delay_file_name.c_str(), ios::out | ios::app);  
    if(atti_delay_str)
    {
        atti_delay_str << ros::Time::now() << endl;
    }


    char gps_delay[] = "simulation_m100/log/gps_delay_";
    string gps_delay_file_name(getDirectory() + gps_delay + uav_name.c_str() + ".txt");
    gps_delay_str.open(gps_delay_file_name.c_str(), ios::out | ios::app);
    if(gps_delay_str)
    {
        gps_delay_str << ros::Time::now() << endl;
    }


    char pos_delay[] = "simulation_m100/log/pos_delay_";
    string pos_delay_file_name(getDirectory() + pos_delay + uav_name.c_str() + ".txt"); 
    pos_delay_str.open(pos_delay_file_name.c_str(), ios::out | ios::app);
    if(pos_delay_str)
    {
        pos_delay_str << ros::Time::now() << endl;
    }


    char vel_delay[] = "simulation_m100/log/vel_delay_";
    string vel_delay_file_name(getDirectory() + vel_delay + uav_name.c_str() + ".txt");  
    vel_delay_str.open(vel_delay_file_name.c_str(), ios::out | ios::app);
    if(vel_delay_str)
    {
        vel_delay_str << ros::Time::now() << endl;
    }


    char accel_delay[] = "simulation_m100/log/accel_delay_";
    string accel_delay_file_name(getDirectory() + accel_delay + uav_name.c_str() + ".txt");
    accel_delay_str.open(accel_delay_file_name.c_str(), ios::out | ios::app);
    if(accel_delay_str)
    {
        accel_delay_str << ros::Time::now() << endl;
    }
}

void closeLogFiles()
{
    gps_log_str.close();
    pos_log_str.close();
    vel_log_str.close();
    atti_delay_str.close();
    gps_delay_str.close();
    pos_delay_str.close();
    vel_delay_str.close();
    accel_delay_str.close();
}

void handleFeedbackData()
{
    bool isNeedToSend = false;
    // while(ros::ok())
    // {
        if(isNsfDone)
        {
            //pthread_mutex_lock(&nsf_mutex);
            //ROS_INFO("handlefeedback got the lock: NsfDone");

            fd_data.set_lon(glo_nsf.longitude);
            fd_data.set_lat(glo_nsf.latitude);
            fd_data.set_alt(glo_nsf.altitude);

            isNsfDone = false;
            isNeedToSend = true;

            //pthread_mutex_unlock(&nsf_mutex);

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

            fd_data.set_px(glo_pos.x);
            fd_data.set_py(glo_pos.y);
            fd_data.set_pz(glo_pos.z);

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

        if(isVelDone)
        {
            //pthread_mutex_lock(&vel_mutex);
            //ROS_INFO("handlefeedback got the lock: VelDone");

            fd_data.set_vx(glo_vel.vector.x);//相对世界坐标系的速度
            fd_data.set_vy(glo_vel.vector.y);
            fd_data.set_vz(glo_vel.vector.z);

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

        if(isImuDone)
        {      
            //pthread_mutex_lock(&imu_mutex);
            //ROS_INFO("handlefeedback got the lock: ImuDone");

            fd_data.set_vw(glo_imu.angular_velocity.z);//有噪声
    
            fd_data.set_roll(glo_attitude.x*rad2deg);
            fd_data.set_pitch(glo_attitude.y*rad2deg);
            fd_data.set_yaw(glo_attitude.z*rad2deg);

            fd_data.set_ax(glo_imu.linear_acceleration.x);//有噪声
            fd_data.set_ay(glo_imu.linear_acceleration.y);//有噪声
            fd_data.set_az(glo_imu.linear_acceleration.z);//有噪声

            isImuDone = false;
            isNeedToSend = true;

            //pthread_mutex_unlock(&imu_mutex);

            
            if(atti_delay_str)
            {
                static double atti_time = ros::Time::now().toSec();
                atti_delay_str << (ros::Time::now().toSec()-atti_time)*1000 << endl;
                atti_time = ros::Time::now().toSec();
            }

            
            if(accel_delay_str)
            {
                static double accel_time = ros::Time::now().toSec();
                accel_delay_str << (ros::Time::now().toSec()-accel_time)*1000 << endl;
                accel_time = ros::Time::now().toSec();
            }
        }

        if(isNeedToSend)//
        {
            simulation_m100::PubSubUavMsg uavmsg;
            if(!fd_data.SerializeToString(&uavmsg.playload))
            {
                perror("feedbackdata serializing error:");
            }
            else
            {
                // ROS_INFO("batteryPercent=%f,state=%d",fd_data.batterypercent(),fd_data.state());
                // ROS_INFO("odometry position:(%lf,%lf,%lf)",fd_data.px(),fd_data.py(),fd_data.pz());
                // ROS_INFO("gps position:(%lf,%lf,%lf)",fd_data.lon(),fd_data.lat(),fd_data.alt());
                // ROS_INFO("attitude:(%lf,%lf,%lf)",fd_data.roll(),fd_data.pitch(),fd_data.yaw());
                // ROS_INFO("vx=%lf,vy=%lf,vz=%lf,vw=%lf",fd_data.vx(),fd_data.vy(),fd_data.vz(),fd_data.vw());
                // ROS_INFO("ax=%lf,ay=%lf,az=%lf\n",fd_data.ax(),fd_data.ay(),fd_data.az());

                uav_data_pub.publish(uavmsg);
                
                isNeedToSend = false;
            }
        }
    // }
}

int main(int argc, char *argv[])
{
    /* code for main function */
    ros::init(argc, argv, "m100LocalDataPool");
    ros::NodeHandle nh;

    ROS_INFO("m100LocalDataPool_node started.");
    
    ros::param::get("~axis_id",axisID);

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

    //订阅仿真无人机的状态数据
    //ros::Subscriber odometry_sub = nh.subscribe("/uav1/odom",10,getOdometry);
    ros::Subscriber fix_sub = nh.subscribe(string(uav_name + "/fix").c_str(),10,getFix);
    ros::Subscriber vel_sub = nh.subscribe(string(uav_name + "/fix_velocity").c_str(),10,getVelocity);
    ros::Subscriber imu_sub = nh.subscribe(string(uav_name + "/raw_imu").c_str(),10,getImu);
    ros::Subscriber bias_imu_sub = nh.subscribe(string(uav_name + "/raw_imu/bias").c_str(),10,getBiasImu);
    ros::Subscriber mag_sub = nh.subscribe(string(uav_name + "/magnetic").c_str(),10,getMag);
    ros::Subscriber sonar_sub = nh.subscribe(string(uav_name + "/sonar_height").c_str(),10,getSonar);
    ros::Subscriber alti_sub = nh.subscribe(string(uav_name + "/pressure_height").c_str(),10,getHeight);
    ros::Subscriber euler_sub = nh.subscribe(string(uav_name + "/ground_truth_to_tf/euler").c_str(),10,getEuler);//
    //ros::Subscriber tra_sub = nh.subscribe(string(uav_name + "/trajectory").c_str(),10,getTra);
    ros::Subscriber uav_pos_sub = nh.subscribe((uav_name + posTopic).c_str(), 10, getUavPos);

    uav_attit_pub = nh.advertise<geometry_msgs::Vector3>(string(uav_name + "/attitude").c_str(),10);
    uav_gps_pub = nh.advertise<sensor_msgs::NavSatFix>(string(uav_name + "/gps").c_str(),10);
    uav_vel_pub = nh.advertise<geometry_msgs::Vector3Stamped>(string(uav_name + "/velocity").c_str(),10);
    uav_accel_pub = nh.advertise<geometry_msgs::Vector3>(string(uav_name + "/acceleration").c_str(),10);

    uav_data_pub = nh.advertise<simulation_m100::PubSubUavMsg>(string(uav_name + "/feedback_data").c_str(),10);

    //pthread_t handleFeedbackData_thread;
    //pthread_create(&handleFeedbackData_thread,NULL,handleFeedbackData,NULL);

    openLogFiles();
    
    ros::Rate loop_rate(100);
    while(ros::ok()){
        ros::spinOnce();

        handleFeedbackData();

        loop_rate.sleep();
    }

    //ros::spin();//执行监听函数

    closeLogFiles();

    // pthread_mutex_destroy(&nsf_mutex);
    // pthread_mutex_destroy(&vel_mutex);
    // pthread_mutex_destroy(&imu_mutex);
    // pthread_mutex_destroy(&pos_mutex);

    ros::shutdown();
    return 0;
}