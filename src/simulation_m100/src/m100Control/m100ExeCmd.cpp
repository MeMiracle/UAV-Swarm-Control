/** @file m100ExeCmd.cpp
 *  @version 2.0
 *  @date May, 2017
 */
//#include <dji_sdk/dji_sdk.h>
#include <demo_flight_control.h>
#include <simulation_m100/CmdResult.h>
#include <simulation_m100/EnableMotors.h>


const float deg2rad = C_PI/180.0;
const float rad2deg = 180.0/C_PI;
const double PI = 3.1415926;

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

//各种控制方式的命令发布器
ros::Publisher geometry_pub;

//命令结果发布器
ros::Publisher cmdResultPub;

//设置控制方式及控制命令
geometry_msgs::Twist glo_move_cmd;

float ctrlInterval = 0.02;//控制间隔

int current_priority = 0;//指令优先级
//std::map<std::string, int> cmd_pri_map;//指令优先级
string takeoff_cmd = "takeoff_cmd";
string land_cmd = "land_cmd";
string gohome_cmd = "gohome_cmd";
string move_cmd = "move_cmd";
string waypoint_cmd = "waypoint_cmd";

//double speed_cmd_start_time = -1;

pthread_t consistPubMoveCmdThread;
bool consistPubMoveCmdThreadRunning = false;
bool consistPubMoveCmdThreadNeedToWork = false;
void* consistPubMoveCmd(void* args);//持续发布指令的线程函数

// global variables for subscribed topics
sensor_msgs::NavSatFix current_gps;//当前gps
sensor_msgs::NavSatFix home_gps;//home的gps点
sensor_msgs::NavSatFix local_gps;//每次航点任务开始时的gps局部坐标原点

simulation_m100::Position current_pos;//当前位置点
simulation_m100::Position home_pos;//home的位置点
simulation_m100::Position local_pos;//每次航点任务开始时的位置局部坐标原点

geometry_msgs::Vector3 current_atti;//当前姿态

myTaskMessage::TaskMessage task_waypoint;//航点序列
Mission square_mission;

string getDirectory();

int main(int argc, char** argv)
{
  ros::init(argc, argv, "m100ExeCmd");
  ros::NodeHandle nh;

  ROS_INFO("m100ExeCmd_node started.");

  home_gps.latitude = -1;
  home_gps.longitude = -1;
  home_gps.altitude = -1;

  ros::param::get("~latitude",home_gps.latitude);//从配置文件读取原点（可以是手动写定，也可以是开机自动获取）
  ros::param::get("~longitude",home_gps.longitude);
  ros::param::get("~altitude",home_gps.altitude);

  // cmd_pri_map[takeoff_cmd] = 0;
  // cmd_pri_map[land_cmd] = 0;
  // cmd_pri_map[gohome_cmd] = 0;
  // cmd_pri_map[move_cmd] = 0;
  // cmd_pri_map[waypoint_cmd] = 0;

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

  ros::param::get("~uav_name",uav_name);

  //订阅控制指令
  ros::Subscriber takeoff_cmd_sub = nh.subscribe(string(uav_name + "/takeoff_cmd").c_str(),10,&takeoff_cmd_callback);
  ros::Subscriber land_cmd_sub = nh.subscribe(string(uav_name + "/land_cmd").c_str(),10,&land_cmd_callback);
  ros::Subscriber gohome_cmd_sub = nh.subscribe(string(uav_name + "/gohome_cmd").c_str(),10,&gohome_cmd_callback);
  ros::Subscriber move_cmd_sub = nh.subscribe(string(uav_name + "/move_cmd").c_str(),10,&move_cmd_callback);
  ros::Subscriber waypoint_cmd_sub = nh.subscribe(string(uav_name + "/waypoint_cmd").c_str(),10,&waypoint_cmd_callback);

  string posTopic("/global_position");
  if(axisID == 2)
  {
      posTopic = string("/local_position");
  }

  // Subscribe to messages 
  ros::Subscriber attitudeSub = nh.subscribe(string(uav_name + "/attitude").c_str(), 10, &attitude_callback);
  ros::Subscriber gpsSub      = nh.subscribe(string(uav_name + "/gps").c_str(), 10, &gps_callback);
  ros::Subscriber positionSub = nh.subscribe((uav_name + posTopic).c_str(), 10, &position_callback);
  ros::Subscriber vel_sub = nh.subscribe(string(uav_name + "/velocity").c_str(),10,&getVelocity);
  ros::Subscriber obst_sub = nh.subscribe(string(uav_name + "/obstacle").c_str(),10,&getObstacle);

  geometry_pub = nh.advertise<geometry_msgs::Twist>(string(uav_name + "/cmd_vel").c_str(), 10);
  cmdResultPub = nh.advertise<simulation_m100::CmdResult>(string(uav_name + "/command_result").c_str(),10);  


  //启动电机
  ros::ServiceClient motorService = nh.serviceClient<simulation_m100::EnableMotors>(string("/" + uav_name + "/enable_motors").c_str());
  simulation_m100::EnableMotors srv;
  srv.request.enable = true;

  int error_count = 0;
  do{
    motorService.call(srv);
    ros::Duration(1).sleep();

    if(srv.response.success == true)
    {
      ROS_INFO("%s true.",string("/" + uav_name + "/enable_motors").c_str());
    }
    else
    {
      error_count++;
      if(error_count > 3)
      {
        ROS_INFO("can not enable motors! please do it manually!");
        break;
      }
      else
      {
        ROS_INFO("%s false %d times! try again...",string("/" + uav_name + "/enable_motors").c_str(),error_count);
      }
    }
  }while(srv.response.success != true);
  
  
  if(consistPubMoveCmdThreadRunning == false)
  {
    pthread_create(&consistPubMoveCmdThread,NULL,consistPubMoveCmd,NULL);
    ros::Duration(0.5).sleep();//等待线程启动
    ROS_INFO("consistPubMoveCmdThread is started.");
  }

  ////////////////////////////////////////
  ros::spin();
  /////////////////////////////////////////

  if(consistPubMoveCmdThreadRunning)
  {
    pthread_cancel(consistPubMoveCmdThread);
    pthread_join(consistPubMoveCmdThread,NULL);
        
    if(pthread_kill(consistPubMoveCmdThread,0) == ESRCH)//检测线程是否真的已经结束
    {
        ROS_INFO("consistPubMoveCmdThread is stopped.");
        consistPubMoveCmdThreadRunning = 0;
    }
  }

  ros::shutdown();
  return 0;
}


bool higher_cmd_priority(string cmd_name,int priority)
{
	// if(priority >= cmd_pri_map[cmd_name])//优先级较高的指令
	// {
	//   cmd_pri_map[cmd_name] = priority;
	//   return true;
	// }
  if(priority >= current_priority){
    current_priority = priority;
    return true;
  }
	else
	{
	  ROS_INFO("%s not executed for lower priority than %d!",cmd_name.c_str(),current_priority);//cmd_pri_map[cmd_name]
	  return false;
	}
}


void hover()//悬停
{
  ROS_INFO("hovering...");

  consistPubMoveCmdThreadNeedToWork = false;
  square_mission.start = false;

  bzero(&glo_move_cmd,sizeof(glo_move_cmd));

  consistPubMoveCmdThreadNeedToWork = true;
}


void takeoff_cmd_callback(const simulation_m100::PubSubUavMsg takeoff_cmd_msg)//起飞
{
  if(!higher_cmd_priority(takeoff_cmd,takeoff_cmd_msg.cmd_priority))//优先级较低的指令不执行
  {
    simulation_m100::CmdResult result;
    result.component = takeoff_cmd_msg.component;
    result.command = takeoff_cmd;
    result.status = false;

    cmdResultPub.publish(result);//返回结果通告（持续发布指令的节点需要监听结果通告，以防止无效指令持续发布）

    return;
  }

  //计算指令响应延迟（实验用）
  //speed_cmd_start_time = takeoff_cmd_msg.start_time;

  if(consistPubMoveCmdThreadRunning == true)
  {
    if(current_pos.z < 0.5)//
    {
      ROS_INFO("taking off...");

      consistPubMoveCmdThreadNeedToWork = false;
      square_mission.start = false;

      task_waypoint.Clear();

      myTaskMessage::MyPoint* mp;
      mp = task_waypoint.add_point();
      mp->set_x(0);
      mp->set_y(0);
      mp->set_z(1.0);
      mp->set_yaw(0);
      mp->set_type(4);
      mp->set_mark(0);

      // for(int i=0;i<task_waypoint.point_size();i++)
      // {
      //   printf("(%f,%f,%f,%f,%d)\n",task_waypoint.point(i).x(),task_waypoint.point(i).y(),task_waypoint.point(i).z(),
      //                             task_waypoint.point(i).yaw(),task_waypoint.point(i).type());
      // }

      if(task_waypoint.point_size() > 0)
      { 
        //local_gps = current_gps;//标定整个航点任务的起始地点
        local_pos = current_pos;//标定整个航点任务的起始地点
        
        square_mission.start = true;//开始航点任务
        square_mission.finished = true;
        square_mission.state = 0;
      }
      else
      {
        hover();//悬停
      }
     }
     else
     {
       ROS_INFO("don't need to take off: height = %lf",current_pos.z);
     }
  }

  else
  {
    ROS_INFO("thread is not running!");
  }
}

void land_cmd_callback(const simulation_m100::PubSubUavMsg land_cmd_msg)//降落
{
  if(!higher_cmd_priority(land_cmd,land_cmd_msg.cmd_priority))
  {
    simulation_m100::CmdResult result;
    result.component = land_cmd_msg.component;
    result.command = land_cmd;
    result.status = false;

    cmdResultPub.publish(result);

    return;
  }

  //计算指令响应延迟（实验用）
  //speed_cmd_start_time = land_cmd_msg.start_time;

  if(consistPubMoveCmdThreadRunning == true)
  {
     if(current_pos.z > 0.5)//
     {
        ROS_INFO("landing...");

        consistPubMoveCmdThreadNeedToWork = false;
        square_mission.start = false;

        task_waypoint.Clear();

        myTaskMessage::MyPoint* mp;
        mp = task_waypoint.add_point();
        mp->set_x(0);
        mp->set_y(0);
        mp->set_z(-current_pos.z);
        mp->set_yaw(0);
        mp->set_type(4);
        mp->set_mark(0);

        // for(int i=0;i<task_waypoint.point_size();i++)
        // {
        //   printf("(%f,%f,%f,%f,%d)\n",task_waypoint.point(i).x(),task_waypoint.point(i).y(),task_waypoint.point(i).z(),
        //                             task_waypoint.point(i).yaw(),task_waypoint.point(i).type());
        // }

        if(task_waypoint.point_size() > 0)
        { 
          //local_gps = current_gps;//标定整个航点任务的起始地点
          local_pos = current_pos;//标定整个航点任务的起始地点
          
          square_mission.start = true;//开始航点任务
          square_mission.finished = true;
          square_mission.state = 0;
        }
        else
        {
          hover();//悬停
        }
     }
     else
     {
       ROS_INFO("don't need to land: height = %lf",current_pos.z);
     }
  }

  else
  {
    ROS_INFO("thread is not running!");
  }
}


void gohome_cmd_callback(const simulation_m100::PubSubUavMsg gohome_cmd_msg)//返航
{
  if(!higher_cmd_priority(gohome_cmd,gohome_cmd_msg.cmd_priority))
  {
    simulation_m100::CmdResult result;
    result.component = gohome_cmd_msg.component;
    result.command = gohome_cmd;
    result.status = false;

    cmdResultPub.publish(result);

    return;
  }

  //计算指令响应延迟（实验用）
  //speed_cmd_start_time = gohome_cmd_msg.start_time;
  
  if(consistPubMoveCmdThreadRunning == true)
  {
     if(current_pos.z > 0.5)//
     {
        ROS_INFO("going home...");

        consistPubMoveCmdThreadNeedToWork = false;
        square_mission.start = false;

        task_waypoint.Clear();

        myTaskMessage::MyPoint* mp;
        mp = task_waypoint.add_point();
        mp->set_x(0);
        mp->set_y(0);
        mp->set_z(current_pos.z);
        mp->set_yaw(0);
        mp->set_type(2);
        mp->set_mark(0);

        // for(int i=0;i<task_waypoint.point_size();i++)
        // {
        //   printf("(%f,%f,%f,%f,%d)\n",task_waypoint.point(i).x(),task_waypoint.point(i).y(),task_waypoint.point(i).z(),
        //                             task_waypoint.point(i).yaw(),task_waypoint.point(i).type());
        // }

        if(task_waypoint.point_size() > 0)
        { 
          //local_gps = current_gps;//标定整个航点任务的起始地点
          local_pos = current_pos;//标定整个航点任务的起始地点
          
          square_mission.start = true;//开始航点任务
          square_mission.finished = true;
          square_mission.state = 0;
        }
        else
        {
          hover();//悬停
        }
     }
     else
     {
       ROS_INFO("can't go home: height = %lf",current_pos.z);
     }
  }

  else
  {
    ROS_INFO("thread is not running!");
  }
}


void move_cmd_callback(const simulation_m100::PubSubUavMsg move_cmd_msg)//订阅并分析控制指令
{
  //ROS_INFO("current priority:%d   move_cmd priority:%d",cmd_pri_map[move_cmd],move_cmd_msg.cmd_priority);
  if(!higher_cmd_priority(move_cmd,move_cmd_msg.cmd_priority))
  {
    simulation_m100::CmdResult result;
    result.component = move_cmd_msg.component;
    result.command = move_cmd;
    result.status = false;

    cmdResultPub.publish(result);

    return;
  }
  //ROS_INFO("current priority:%d   move_cmd priority:%d",cmd_pri_map[move_cmd],move_cmd_msg.cmd_priority);

  //计算指令响应延迟（实验用）
  //speed_cmd_start_time = move_cmd_msg.start_time;

  gcsCommand::Twist twist;
  twist.ParseFromString(move_cmd_msg.playload);

  if(consistPubMoveCmdThreadRunning == true)
  {
    if(current_pos.z > 0)//空中就绪状态时可以执行移动控制指令
    {
      consistPubMoveCmdThreadNeedToWork = false;
      square_mission.start = false;

      simulation_m100::CmdResult result;
      result.component = "m100PathPlan";
      result.command = waypoint_cmd;
      result.status = false;
      cmdResultPub.publish(result);

      bzero(&glo_move_cmd,sizeof(glo_move_cmd));

      glo_move_cmd.linear.x = twist.linear_x();
      glo_move_cmd.linear.y = twist.linear_y();
      glo_move_cmd.linear.z = twist.linear_z();
      glo_move_cmd.angular.x = twist.angular_x();
      glo_move_cmd.angular.y = twist.angular_y();
      glo_move_cmd.angular.z = twist.angular_z();

      if(strcmp(move_cmd_msg.component.c_str(),"Recv") == 0)//地面站速度控制指令
      {
        ROS_INFO("vx=%lf,vy=%lf,vz=%lf,ax=%lf,ay=%lf,az=%lf",glo_move_cmd.linear.x,glo_move_cmd.linear.y,glo_move_cmd.linear.z,
                                                           glo_move_cmd.angular.x,glo_move_cmd.angular.y,glo_move_cmd.angular.z);
      }

      consistPubMoveCmdThreadNeedToWork = true;

      //float delta_time = ros::Time::now().toSec()*1000 -move_cmd_msg.start_time;
      //printf("%f\n",delta_time);
    }
    else
    {
      ROS_INFO("move command denied: height = %lf",current_pos.z);
    }
  }

  else
  {
    ROS_INFO("thread is not running!");
  }
}

ofstream finished_point_str;
simulation_m100::PubSubUavMsg wp_cmd_msg;
void waypoint_cmd_callback(const simulation_m100::PubSubUavMsg waypoint_cmd_msg)//订阅并分析控制指令
{
  if(!higher_cmd_priority(waypoint_cmd,waypoint_cmd_msg.cmd_priority))
  {
    simulation_m100::CmdResult result;
    result.component = waypoint_cmd_msg.component;
    result.command = waypoint_cmd;
    result.status = false;

    cmdResultPub.publish(result);

    return;
  }
  char finished_point[] = "simulation_m100/log/finished_point.txt";
  string finished_point_file(getDirectory() + finished_point);

  finished_point_str.open(finished_point_file.c_str(),ios::out | ios::app);
  if(finished_point_str)
  {
    finished_point_str << ros::Time::now() << endl;
  }

  wp_cmd_msg = waypoint_cmd_msg;
  //计算指令响应延迟（实验用）
  //speed_cmd_start_time = waypoint_cmd_msg.start_time;
  
  if(consistPubMoveCmdThreadRunning == true)
  {
    if(current_pos.z > 0.5)//处于正在起飞/空中就绪/正在降落状态时可以执行航点飞行任务
    {
      task_waypoint.Clear();
      task_waypoint.ParseFromString(waypoint_cmd_msg.playload);

      // for(int i=0;i<task_waypoint.point_size();i++)
      // {
      //   printf("(%f,%f,%f,%f,%d)\n",task_waypoint.point(i).x(),task_waypoint.point(i).y(),task_waypoint.point(i).z(),
      //                             task_waypoint.point(i).yaw(),task_waypoint.point(i).type());
      // }

      if(task_waypoint.point_size() > 0)
      { 
         consistPubMoveCmdThreadNeedToWork = false;
         square_mission.start = false;

        //local_gps = current_gps;//标定整个航点任务的起始地点
        local_pos = current_pos;//标定整个航点任务的起始地点
        
        square_mission.start = true;//开始航点任务
        square_mission.finished = true;
        square_mission.state = 0;
      }
      else
      {
        hover();//悬停
      }
    }
    else
    {
      ROS_INFO("waypoint command denied: height = %lf",current_pos.z);

      simulation_m100::CmdResult result;
      result.component = waypoint_cmd_msg.component;
      result.command = waypoint_cmd;
      result.status = false;

      cmdResultPub.publish(result);
    }
  }
  else
  {
    ROS_INFO("thread is not running!");
  }  
}

geometry_msgs::Vector3Stamped current_vel;
void getVelocity(const geometry_msgs::Vector3Stamped& vel)//全局对地速度
{
    //ROS_INFO("v_x=%lf,v_y=%lf,v_z=%lf\n",vel.vector.x,vel.vector.y,vel.vector.z);
    //pthread_mutex_lock(&vel_mutex);
    current_vel = vel;
    //pthread_mutex_unlock(&vel_mutex);
}

simulation_m100::ObstacleMsg obstacle;
void getObstacle(const simulation_m100::ObstacleMsg& obst)
{
  obstacle = obst;
}

double DEG_PI_TO_2PI(double angle)
{
  if(angle < 0)
    angle += 360;
  if(angle == 360)
    angle = 0;
  return angle;
}

int stopFlag = 0,decelerateFlag = 0,safetyFlag=0;
void emergencyObstacleAvoidance(geometry_msgs::Twist& ctl_vel)//应急避障
{
  geometry_msgs::Vector3Stamped temp_current_vel = current_vel;
  if((temp_current_vel.vector.x == 0 && temp_current_vel.vector.y == 0)
    || (ctl_vel.linear.x == 0 && ctl_vel.linear.y == 0))
  {
    return;
  }
  safetyFlag = 0;
  //计算当前运动速率与方向
  double move_speed = sqrt(temp_current_vel.vector.x*temp_current_vel.vector.x + temp_current_vel.vector.y*temp_current_vel.vector.y);//当前合速率
  double uav_radius = 0.6;
  double deceleration_factor = 0.5;

  //避障
  simulation_m100::ObstacleMsg temp_obstacle = obstacle;
  //ROS_INFO("orignal cmd %f %f",ctl_vel.linear.x,ctl_vel.linear.y);

  //意图控制运动方向,0~2PI
  double ctl_orient = 0;
  if(ctl_vel.linear.y == 0)
  {
    ctl_orient = ctl_vel.linear.x>0? 90*deg2rad:270*deg2rad;
  }
  else
  {
    if(ctl_vel.linear.x >= 0 && ctl_vel.linear.y > 0)
    {
      ctl_orient = atan(ctl_vel.linear.x/ctl_vel.linear.y);
    }
    else if(ctl_vel.linear.x >= 0 && ctl_vel.linear.y < 0)
    {
      ctl_orient = atan(ctl_vel.linear.x/ctl_vel.linear.y) + PI;
    }
    else if(ctl_vel.linear.x < 0 && ctl_vel.linear.y < 0)
    {
      ctl_orient = atan(ctl_vel.linear.x/ctl_vel.linear.y) + PI;
    }
    else
    {
      ctl_orient = atan(ctl_vel.linear.x/ctl_vel.linear.y) + 2*PI;
    }
  }
  
  //避障距离阈值
  double decelerationArea = 3; //进入障碍物2m范围减速
  double stopArea = 2;  //进入危险区域停止
  double danger_degree1,danger_degree2;
  ros::param::get("~decelerationArea",decelerationArea);
  ros::param::get("~stopArea",stopArea);
  // double stopArea = 2*move_speed>decelerationArea ? 2*move_speed:1.5;
  // ctl_orient = 2*PI - ctl_orient; //转换为机头0°逆时针

  // ROS_INFO("ctl_orient %f",ctl_orient);
  //ROS_INFO("---------------------------");
  for(int i=0;i<temp_obstacle.center.size();i++)  // 障碍物检测
  {
    //ROS_INFO("recrive obstacle %d degree %f %f %f",i, temp_obstacle.center[i].y,temp_obstacle.center[i].z,temp_obstacle.center[i].x);
    if(temp_obstacle.center[i].x < stopArea) // 判断运行指令，如果朝向障碍物，则停止前进
    {
      double temp_rad1 = DEG_PI_TO_2PI(temp_obstacle.center[i].y);
      double temp_rad2 = DEG_PI_TO_2PI(temp_obstacle.center[i].z);
      // double mid_degree ;

      // if (temp_rad1>temp_rad2)
      //   mid_degree = (temp_rad1+temp_rad2)/2;
      // else
      // {
      //   mid_degree = (temp_rad1+temp_rad2+360)/2;
      // }

      // if (mid_degree>=360)
      //   mid_degree -= 360;

      // temp_rad1 = mid_degree+90;
      // temp_rad2 = mid_degree-90;
      // if (temp_rad1>=360)
      //   temp_rad1 -= 360;
      // if (temp_rad2>=360)
      //   temp_rad2 -= 360;
      // if (temp_rad1<0)
      //   temp_rad1 += 360;
      // if (temp_rad2<0)
      //   temp_rad2 += 360;
      
      // ROS_INFO("mid %f   first %f   second %f   cmd %f  %f",mid_degree,temp_rad1,temp_rad2,ctl_orient*rad2deg,ctl_orient);
      temp_rad1 = temp_rad1*deg2rad;
      temp_rad2 = temp_rad2*deg2rad;

      if(temp_rad1<temp_rad2)   // 障碍物角度过0和360
      {
        if(ctl_orient<temp_rad1 || ctl_orient>temp_rad2) 
        {
          //ROS_INFO("obstacle over 0/360");
          stopFlag = 10;
          break;
        }
        else
        {
          safetyFlag++;  //没有朝着障碍物飞，不用停止
        }
      }
      else  // 障碍物角度不过0和360
      {
        if(ctl_orient<temp_rad1 && ctl_orient>temp_rad2)  
        {
          //ROS_INFO("normal obstalce");
          stopFlag = 10;
          break; 
        }
        else
        {
          safetyFlag++;  //没有朝着障碍物飞，不用停止
        }
      }
    }
    else if(temp_obstacle.center[i].x < decelerationArea)
    {
      // 减速层障碍物
      //ROS_INFO("decelerate");
      decelerateFlag = 10;
      safetyFlag ++; //减速但是安全，不用停止
    }
    else  // 无障碍物情况
    {
      //ROS_INFO("no obstacle");
      safetyFlag ++;
    }
  }
 
  if (safetyFlag >= temp_obstacle.center.size())
  {
    if (stopFlag)
        stopFlag--;
    if (decelerateFlag)
      decelerateFlag--;
  }

  //ROS_INFO("FLAG STOP %d , DEC %d , safe %d", stopFlag, decelerateFlag, safetyFlag);

  if( stopFlag ) 
  {   
    // 停止
    //ROS_INFO("CMD STOP");
    ctl_vel.linear.x = 0;
    ctl_vel.linear.y = 0;
  }
  else if( decelerateFlag )
  {
    //ROS_INFO("CMD DECELERATE");
    ctl_vel.linear.x = ctl_vel.linear.x*deceleration_factor;
    ctl_vel.linear.y = ctl_vel.linear.y*deceleration_factor;
  }
  else
  {
    //ROS_INFO("CMD NORMAL");
    ctl_vel.linear.x = ctl_vel.linear.x;
    ctl_vel.linear.y = ctl_vel.linear.y;
  }
  //ROS_INFO("obstacle cmd %f %f",ctl_vel.linear.x,ctl_vel.linear.y);
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

//移动
void* consistPubMoveCmd(void* args)//持续发布指令
{
  consistPubMoveCmdThreadRunning = true;

  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);//线程可被取消
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); //设置立即取消 

  char speed_cmd_delay[] = "simulation_m100/log/speed_cmd_delay.txt";
  string speed_cmd_delay_file(getDirectory() + speed_cmd_delay);

  ofstream delay_str;
  delay_str.open(speed_cmd_delay_file.c_str(),ios::out | ios::app);
  delay_str << ros::Time::now() << endl;

  while(ros::ok())
  {
    while(consistPubMoveCmdThreadNeedToWork)
    {
      //速度指令响应延迟，实验用
      //if(delay_str && speed_cmd_start_time>0)
      //{
          static double speed_cmd_start_time = ros::Time::now().toSec();
          //ROS_INFO("now:%lf  start:%lf",ros::Time::now().toSec(),speed_cmd_start_time);
          delay_str << (ros::Time::now().toSec()-speed_cmd_start_time)*1000 << endl;
          speed_cmd_start_time = ros::Time::now().toSec();
      //}

	    static ros::Time start_time = ros::Time::now();
  	  ros::Duration elapsed_time = ros::Time::now() - start_time;//两次回调的时间间隔

      static ros::Time hover_start_time = ros::Time::now();
      static bool needToRestart = true;
      if(glo_move_cmd.linear.x+glo_move_cmd.linear.y+glo_move_cmd.linear.z+glo_move_cmd.angular.z == 0)
      {
        if(needToRestart)
        {
          hover_start_time = ros::Time::now();
          needToRestart = false;
        }

        if(ros::Time::now() - hover_start_time > ros::Duration(2.0)) //悬停超过一段时间则重置所有指令的优先级，以确保可以响应其他指令
        {
          // cmd_pri_map[takeoff_cmd] = 0;
          // cmd_pri_map[land_cmd] = 0;
          // cmd_pri_map[gohome_cmd] = 0;
          // cmd_pri_map[move_cmd] = 0;
          // cmd_pri_map[waypoint_cmd] = 0;
          current_priority = 0;

          needToRestart = true;
        }
      }
      else
      {
        needToRestart = true;
      }

	    if(elapsed_time > ros::Duration(0.02))
  	  {
        geometry_msgs::Twist ctl_cmd;
        bzero(&ctl_cmd,sizeof(ctl_cmd));

        //不同控制方式转换成机身速度控制
        if(glo_move_cmd.angular.x == 0)//机身速度控制
        {
          ctl_cmd.linear.x = glo_move_cmd.linear.x;
          ctl_cmd.linear.y = glo_move_cmd.linear.y;
        }
        else if(glo_move_cmd.angular.x == 1)//全局坐标系内，对地速度控制
        {
          double yawInRad = current_atti.z;

          if(axisID == 2)
          {
            yawInRad = yawInRad*rad2deg + axisOffsetAngle;

            //转换为：北偏西+0～+180，北偏东-0～~180
            if(abs(yawInRad) > 180)
            {
              int n = abs(yawInRad) / 360 + 1;
              n = yawInRad>0?-n:n;
              yawInRad = yawInRad + 360 * n;//旋转角度控制
            }

            yawInRad = yawInRad * deg2rad;
          }

          ctl_cmd.linear.x = glo_move_cmd.linear.x*cos(yawInRad) + glo_move_cmd.linear.y*sin(yawInRad);
          ctl_cmd.linear.y = -glo_move_cmd.linear.x*sin(yawInRad) + glo_move_cmd.linear.y*cos(yawInRad);
        }
        else if(glo_move_cmd.angular.x == 2)//实验坐标系内，对地速度控制
        {
          double yawInRad = current_atti.z;//

          ctl_cmd.linear.x = glo_move_cmd.linear.x*cos(yawInRad) + glo_move_cmd.linear.y*sin(yawInRad);
          ctl_cmd.linear.y = -glo_move_cmd.linear.x*sin(yawInRad) + glo_move_cmd.linear.y*cos(yawInRad);
        }

        ctl_cmd.linear.z = glo_move_cmd.linear.z;
        ctl_cmd.angular.z = glo_move_cmd.angular.z;

        emergencyObstacleAvoidance(ctl_cmd);

        //ROS_INFO("vx=%lf,vy=%lf,vz=%lf,vyaw=%lf",ctl_cmd.linear.x,ctl_cmd.linear.y,ctl_cmd.linear.z,ctl_cmd.angular.z);

        geometry_pub.publish(ctl_cmd);

        pthread_testcancel();//the thread can be killed only here（设置取消点，确保线程可取消）
        
		    start_time = ros::Time::now();//重新计时
  	  }

      ros::Duration(ctrlInterval).sleep();
    }

    ros::Duration(0.5).sleep();
  }

  delay_str.close();

  consistPubMoveCmdThreadRunning = false;
}

/*
void axisOffsetAnglePoint(myTaskMessage::MyPoint* mp)//实验场地坐标转换成自然坐标
{
	double temp_x = mp->x() * cosOffsetAngle + mp->y() * sinOffsetAngle + axisOffsetX;
	double temp_y =-mp->x() * sinOffsetAngle + mp->y() * cosOffsetAngle + axisOffsetY;
	double temp_z = mp->z() + axisOffsetZ;

	mp->set_x(temp_x);
	mp->set_y(temp_y);
	mp->set_z(temp_z);
}
*/

//航点坐标变换：转换成基于航点距离坐标
void transformWP(myTaskMessage::MyPoint* mp)
{
  double temp_yaw = mp->yaw();

  if(abs(temp_yaw) > 180)
  {
    int n = abs(temp_yaw) / 360 + 1;
    n = temp_yaw>0?-n:n;
    temp_yaw = temp_yaw + 360 * n;//旋转角度控制
  }
  mp->set_yaw(temp_yaw);

  switch (mp->type())
  {
    case 4:
      /* 航点距离坐标(以每个航点任务开始时的无人机坐标为原点) */
      // if(axisID == 2)
      // {
      //   axisOffsetAnglePoint(mp);
      // }
      break;
    case 3:
      /* 局部距离坐标转成航点距离坐标 */
      {
		    // if(axisID == 2)
        // {
        //   axisOffsetAnglePoint(mp);
        // }

        simulation_m100::Position deltaDist;
        deltaDist.x = current_pos.x - local_pos.x;
        deltaDist.y = current_pos.y - local_pos.y;
        deltaDist.z = current_pos.z - local_pos.z;

        mp->set_x(mp->x()-deltaDist.x);
        mp->set_y(mp->y()-deltaDist.y);
        mp->set_z(mp->z()-deltaDist.z);
      }
      break;
    case 2:
      /* 全局距离坐标转成航点距离坐标 */
      {
		    // if(axisID == 2)
        // {
        //   axisOffsetAnglePoint(mp);
        // }

        simulation_m100::Position deltaDist = current_pos;

        mp->set_x(mp->x()-deltaDist.x);
        mp->set_y(mp->y()-deltaDist.y);
        mp->set_z(mp->z()-deltaDist.z);
      }
      break;
    case 1:
      /* 全局gps坐标转成航点距离坐标 */
      {
		    simulation_m100::Position currPoint = current_pos;

        sensor_msgs::NavSatFix targetGps;
        targetGps.longitude = mp->x();
        targetGps.latitude = mp->y();
        targetGps.altitude = mp->z() + home_gps.altitude;

        geometry_msgs::Vector3 targetDist;
        localOffsetFromGpsOffset(targetDist, targetGps, home_gps);//gps坐标转成全局距离坐标

        if(axisID == 2)
        {
          	double temp_x = targetDist.x * cosOffsetAngle - targetDist.y * sinOffsetAngle - axisOffsetX;
            double temp_y = targetDist.x * sinOffsetAngle + targetDist.y * cosOffsetAngle - axisOffsetY;
            double temp_z = targetDist.z - axisOffsetZ;

            targetDist.x = temp_x;
            targetDist.y = temp_y;
            targetDist.z = temp_z;
        }
        //ROS_INFO("x=%lf,y=%lf,z=%lf",targetDist.x,targetDist.y,targetDist.z);

        mp->set_x(targetDist.x-currPoint.x);
        mp->set_y(targetDist.y-currPoint.y);
        mp->set_z(targetDist.z-currPoint.z);
      }
      break;
    default:
      /* Default Code */
      break;
  }
}

//姿态数据（四元组）
void attitude_callback(geometry_msgs::Vector3 attit)
{
  current_atti = attit;

}

void gps_callback(sensor_msgs::NavSatFix msg)
{
  current_gps = msg;//按回调频率更新数据

  if(home_gps.latitude < 0 && home_gps.longitude < 0 && home_gps.altitude < 0)
  { 
    //如果没有手动设置home点，则在开机初始化时获取当前位置作为家的位置
    home_gps = current_gps;
  }
}

void position_callback(simulation_m100::Position pos)//基于自然坐标系的位置更新
{
  current_pos = pos;
  
  static ros::Time start_time = ros::Time::now();
  ros::Duration elapsed_time = ros::Time::now() - start_time;//两次回调的时间间隔

  // Down sampled to 50Hz loop(两次执行的时间间隔大于20ms)
  //if(elapsed_time > ros::Duration(0.02))
  //{
    if(square_mission.start)
    {
      if(square_mission.finished)//完成一个航点
      {
        square_mission.reset();//清空相关控制变量
        //square_mission.start_gps_location = current_gps;
        square_mission.start_pos_location = current_pos;
        square_mission.state++;
       
        if(square_mission.state <= task_waypoint.point_size())//任务未完成，则继续下一个航点
        {
          myTaskMessage::MyPoint mp;
          mp = task_waypoint.point(square_mission.state-1);

          ROS_INFO("origin waypoint %d: (%f, %f, %f, %f, %d)", square_mission.state,mp.x(),mp.y(),mp.z(),mp.yaw(),mp.type());//打印原始坐标

          transformWP(&mp);

          ROS_INFO("target waypoint %d: (%f, %f, %f, %f, 4)\n", square_mission.state,mp.x(),mp.y(),mp.z(),mp.yaw());//打印变换后的坐标

          square_mission.setTarget(mp.x(), mp.y(), mp.z(), mp.yaw());//机身距离坐标（以每个航点任务开始时的机身坐标为参考原点）
          square_mission.finished = false;
        }
        else//完成所有航点
        {
          hover();//悬停

          ROS_INFO("all waypoints Finished!");
          square_mission.start = false;
          square_mission.finished = true;
          square_mission.state = 0;

          //cmd_pri_map[waypoint_cmd] = 0;
          current_priority = 0;


          simulation_m100::CmdResult result;
          result.component = wp_cmd_msg.component;
          result.command = waypoint_cmd;
          result.status = false;

          cmdResultPub.publish(result);

          if(finished_point_str)
          {
            finished_point_str.close();
          }
        }
      }

      if(!square_mission.finished)//航点正在执行
      {
        square_mission.step();
      }
    }

  //  start_time = ros::Time::now();//重新计时
  //}
}


void Mission::step()
{
  //根据当前gps坐标和每个航点任务开始时的起始坐标，计算出已经飞行的距离
  //geometry_msgs::Vector3     localOffset;
  //localOffsetFromGpsOffset(localOffset, current_gps, start_gps_location);

  simulation_m100::Position localOffset;
  localOffset.x = current_pos.x - start_pos_location.x;
  localOffset.y = current_pos.y - start_pos_location.y;
  localOffset.z = current_pos.z - start_pos_location.z;

  //还需飞行的距离（剩余距离）
  double xOffsetRemaining = target_offset_x - localOffset.x;
  double yOffsetRemaining = target_offset_y - localOffset.y;
  double zOffsetRemaining = target_offset_z - localOffset.z;

  //根据剩余距离和速度因子的大小决定每次步进的速度
  float speedFactor         = 2;
  float xCmd, yCmd, zCmd, yawCmd;

  if (abs(xOffsetRemaining) >= speedFactor)
    xCmd = (xOffsetRemaining>0) ? speedFactor : -1 * speedFactor;
  else
    xCmd = xOffsetRemaining;

  if (abs(yOffsetRemaining) >= speedFactor)
    yCmd = (yOffsetRemaining>0) ? speedFactor : -1 * speedFactor;
  else
    yCmd = yOffsetRemaining;

  //zCmd = start_gps_location.altitude + target_offset_z;
  zCmd = start_pos_location.z + target_offset_z;
  //zCmd = zOffsetRemaining;


  float yawThresholdInDeg   = 2;

  double yawDesiredRad     = deg2rad * target_yaw;//预期的偏航角
  double yawThresholdInRad = deg2rad * yawThresholdInDeg;//误差阈值偏航角
  double yawInRad          = current_atti.z;//当前偏航角 //toEulerAngle根据四元组姿态数据，解算出绕三个轴旋转的角度


  // static int info_counter = 0;
  // info_counter++;
  // if(info_counter > 50)//每50次步进打印一次信息
  // {
  //   info_counter = 0;
     //ROS_INFO("fihished:x=%f, y=%f, z=%f, yaw=%f", localOffset.x,localOffset.y, localOffset.z,yawInRad);
     //ROS_INFO("need fly:dx=%f, dy=%f, dz=%f, dyaw=%f", xOffsetRemaining,yOffsetRemaining, zOffsetRemaining,yawInRad - yawDesiredRad);
  // }


   ///////////////确定不同阶段无人机的飞行速度//////////////////////////
   float vx, vy, vz, vyaw;
   float xspeed_factor;
   float yspeed_factor;
   float zspeed_factor;
   if(abs(xOffsetRemaining) >= abs(yOffsetRemaining))
   {
       if(abs(xOffsetRemaining) > 2)//根据距离长度大致确定不同飞行速度，距离大于2米时速度为1m/s
       {
           xspeed_factor = 1.0;
       }
       else//距离小于2米时，速度为0.3m/s
       {
          xspeed_factor = 0.3;

          if(abs(xOffsetRemaining) <= 0.5 )
          {
            xspeed_factor = 0.3;

            if(abs(xOffsetRemaining) <= 0.3 )
            {
              xspeed_factor = abs(xOffsetRemaining);
            }
          }
       }

		if(xOffsetRemaining != 0)
		{
       		yspeed_factor = (abs(yOffsetRemaining/xOffsetRemaining)) * xspeed_factor;//计算x轴和y轴的速度比例，以获得较合适的合速度  
		}
		else
		{
			yspeed_factor = (abs(yOffsetRemaining/1)) * xspeed_factor;
		}           
   }
   else
   {
       if(abs(yOffsetRemaining) > 2)
       {
           yspeed_factor = 1.0;
       }
       else
       {
          yspeed_factor = 0.3;

          if(abs(yOffsetRemaining) <= 0.5 )
          {
            yspeed_factor = 0.3;

            if(abs(yOffsetRemaining) <= 0.3 )
            {
              yspeed_factor = abs(yOffsetRemaining);
            }
          }
       }

		if(yOffsetRemaining != 0)
		{
       		xspeed_factor = (abs(xOffsetRemaining/yOffsetRemaining)) * yspeed_factor;
		}
		else
		{
			xspeed_factor = (abs(xOffsetRemaining/1)) * yspeed_factor;
		}
   }

   if(abs(zOffsetRemaining) > 2)
   {
       zspeed_factor = 1.0;
   }
   else
   {
      zspeed_factor = 0.3;

      if(abs(zOffsetRemaining) <= 0.5 )
      {
        zspeed_factor = 0.3;

        if(abs(zOffsetRemaining) <= 0.3 )
        {
          zspeed_factor = abs(zOffsetRemaining);
        }
      }
   }

   vx = (xOffsetRemaining>0) ? xspeed_factor : -1 * xspeed_factor;
   vy = (yOffsetRemaining>0) ? yspeed_factor : -1 * yspeed_factor;
   vz = (zOffsetRemaining>0) ? zspeed_factor : -1 * zspeed_factor;

/*
    ///////////////////////// 确定无人机飞行的误差允许范围////////////////////////////
   float x_ctl_err_range = 0.2;
   float y_ctl_err_range = 0.2;
   float z_ctl_err_range = 0.2;
   if(abs(xOffsetRemaining) > x_ctl_err_range)
   {
       vx = xspeed_factor * (xOffsetRemaining / abs(xOffsetRemaining));
   }
   else//达到目标点正负0.1米范围时速度减为0
   {
       vx = xOffsetRemaining;
   }

   if(abs(yOffsetRemaining) > y_ctl_err_range)
   {
       vy = yspeed_factor * (yOffsetRemaining / abs(yOffsetRemaining));
   }
   else
   {
       vy = yOffsetRemaining;
   }

   if(abs(zOffsetRemaining) > z_ctl_err_range)
   {
      vz = zspeed_factor * (zOffsetRemaining / abs(zOffsetRemaining));
   }
   else
   {
       vz = zOffsetRemaining;
   }
*/  

   ////////////////////旋转至目标角度/////////////////////////////
   double delta_yaw = yawDesiredRad - yawInRad;

   float yawspeed_factor = 0.5;
   if(abs(delta_yaw) > PI)
   {
       yawspeed_factor = -yawspeed_factor;
   }

   float yaw_ctl_err_range = 0.05;
   if(abs(delta_yaw) > yaw_ctl_err_range)
   {
		if(delta_yaw != 0)
		{
       		vyaw = yawspeed_factor * (delta_yaw / abs(delta_yaw));
		}
		else
		{
			vyaw = delta_yaw;
		}
   }
   else
   {
       vyaw = delta_yaw;
   }

  
  ///////////////飞行期的实时检测//////////////////////
  if (std::abs(xOffsetRemaining) < 0.2 &&
      std::abs(yOffsetRemaining) < 0.2 &&
      std::abs(zOffsetRemaining) < 0.2 &&
      std::abs(yawInRad - yawDesiredRad) < yawThresholdInRad)
  { //接近目标点（进入误差限定区域），开始微调（计算在限定区域内的稳定次数和飘离次数）
    //! 1. We are within bounds; start incrementing our in-bound counter
    //inbound_counter ++;
    
	//vx = 0;
	//vy = 0;
	//vz = 0;
	//vyaw = 0;

	break_counter ++;
  }
 /* else//还未接近目标点或者从目标点飘离
  {
    if (inbound_counter != 0)//已接近目标点但是从目标点飘离，计算飘离次数，以增加容错性，减少误判
    {
      //! 2. Start incrementing an out-of-bounds counter
      outbound_counter ++;
    }
  }

  //! 3. Reset withinBoundsCounter if necessary
  if (outbound_counter > 10)//飘离次数过多，重新计数（重新微调）
  {
    ROS_INFO("out of bound, reset....");
    inbound_counter  = 0;
    outbound_counter = 0;
  }

  if (inbound_counter > 10)//微调时稳定次数大于50，控制间隔为20ms，即稳定时间超过1s，则认为正式到达目标点，开始尝试悬停1s
  {
    ROS_INFO("start to loiter....");
    break_counter = 1;
  }
*/


   //////////执行飞行指令///////////
  if(break_counter <= 1) //飞行期（步进期）break_counter = 0, not in break stage
  {
    bzero(&glo_move_cmd,sizeof(glo_move_cmd));

    glo_move_cmd.linear.x = vx;
    glo_move_cmd.linear.y = vy;
    glo_move_cmd.linear.z = vz;
    glo_move_cmd.angular.x = axisID;//场地坐标系，对地速度控制
    glo_move_cmd.angular.y = 0;
    glo_move_cmd.angular.z = vyaw;

    consistPubMoveCmdThreadNeedToWork = true;
  }
/*  else if(break_counter > 0 && break_counter <= 10)//悬停期
  {
    hover();//悬停

    break_counter++;
    return;
  }
*/  else//(break_counter > 50)//完成一个航点
  {
    ROS_INFO("waypoint %d finished!", state);

    if(finished_point_str)
    {
      finished_point_str << current_pos.x << " " << current_pos.y << " " << current_pos.z << endl;
    }

    finished = true;
	  break_counter = 0;
    return;
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


//根据四元组姿态数据，解算出绕三个轴旋转的角度
geometry_msgs::Vector3 toEulerAngle(geometry_msgs::Quaternion quat)
{
  geometry_msgs::Vector3 ans;

  tf::Matrix3x3 R_FLU2ENU(tf::Quaternion(quat.x, quat.y, quat.z, quat.w));
  R_FLU2ENU.getRPY(ans.x, ans.y, ans.z);
  return ans;
}
