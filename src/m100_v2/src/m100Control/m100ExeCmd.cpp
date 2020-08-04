 /** @file demo_flight_control.cpp
 *  @version 3.3
 *  @date May, 2017
 * 
 *  @brief
 *  demo sample of how to use flight control APIs
 *
 *  @copyright 2017 DJI. All rights reserved.
 *
 */
#include <dji_sdk/dji_sdk.h>
#include <demo_flight_control.h>
#include <m100_v2/CmdResult.h>
//#include <m100_v2/EnableMotors.h>


const float deg2rad = C_PI/180.0;
const float rad2deg = 180.0/C_PI;
const double PI = 3.1415926;

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

//各种控制方式的命令发布器
//ros::Publisher geometry_pub;
ros::Publisher ctrlGenericPub;

//命令结果发布器
ros::Publisher cmdResultPub;

//向sdk的服务请求
ros::ServiceClient sdk_ctrl_authority_service;
ros::ServiceClient query_version_service;
ros::ServiceClient drone_task_service;

//设置控制方式及控制命令
geometry_msgs::Twist glo_move_cmd;

float ctrlInterval = 0.02;//控制间隔20ms

std::map<std::string, int> cmd_pri_map;//指令优先级
string takeoff_cmd = "takeoff_cmd";
string land_cmd = "land_cmd";
string gohome_cmd = "gohome_cmd";
string move_cmd = "move_cmd";
string waypoint_cmd = "waypoint_cmd";

double action_cmd_start_time = -1;
//double speed_cmd_start_time = -1;

pthread_t requestTaskSrvThread;
bool requestTaskSrvThreadRunning = false;//线程是否已经开启
bool requestTaskSrvThreadNeedToWork = false;//线程是否需要工作
void* requestTaskService(void* args);//执行起飞/降落/返航命令的线程函数
bool gotNewActionCmd = false;
uint8_t requestTask;

pthread_t consistPubMoveCmdThread;
bool consistPubMoveCmdThreadRunning = false;
bool consistPubMoveCmdThreadNeedToWork = false;
void* consistPubMoveCmd(void* args);//持续发布指令的线程函数

// global variables for subscribed topics
sensor_msgs::NavSatFix current_gps;//当前gps
sensor_msgs::NavSatFix home_gps;//home的gps点
sensor_msgs::NavSatFix local_gps;//每次航点任务开始时的gps局部坐标原点

m100_v2::Position current_pos;//当前位置点
m100_v2::Position local_pos;//每次航点任务开始时的位置局部坐标原点

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

  ros::param::get("~latitude",home_gps.latitude);
  ros::param::get("~longitude",home_gps.longitude);
  ros::param::get("~altitude",home_gps.altitude);


  cmd_pri_map[takeoff_cmd] = 0;
  cmd_pri_map[land_cmd] = 0;
  cmd_pri_map[gohome_cmd] = 0;
  cmd_pri_map[move_cmd] = 0;
  cmd_pri_map[waypoint_cmd] = 0;

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

  //subscribe from other component
  ros::Subscriber gpsSub      = nh.subscribe(string(uav_name + "/gps").c_str(), 10, &gps_callback);
  ros::Subscriber vel_sub = nh.subscribe(string(uav_name + "/velocity"),10,&velocity_callback);
  ros::Subscriber attitudeSub = nh.subscribe(string(uav_name + "/attitude").c_str(), 10, &attitude_callback);
  ros::Subscriber positionSub = nh.subscribe((uav_name + posTopic).c_str(), 10, &position_callback);
  ros::Subscriber obst_sub = nh.subscribe(string(uav_name + "/obstacle").c_str(),10,&obstacle_callback);
  ros::Subscriber status_sub = nh.subscribe(string(uav_name + "/flight_status").c_str(),10,&flight_status_callback);

  //geometry_pub = nh.advertise<geometry_msgs::Twist>(string(uav_name + "/cmd_vel").c_str(), 10);
  ctrlGenericPub = nh.advertise<sensor_msgs::Joy>("dji_sdk/flight_control_setpoint_generic", 100);
  cmdResultPub = nh.advertise<m100_v2::CmdResult>(string(uav_name + "/command_result").c_str(),10);  


    // Basic services
  sdk_ctrl_authority_service = nh.serviceClient<dji_sdk::SDKControlAuthority> ("dji_sdk/sdk_control_authority");
  query_version_service      = nh.serviceClient<dji_sdk::QueryDroneVersion>("dji_sdk/query_drone_version");
  drone_task_service         = nh.serviceClient<dji_sdk::DroneTaskControl>("dji_sdk/drone_task_control");

  ros::Rate rate(1);
  while(ros::ok())
  {
    if(obtain_control())//获取控制权限
    {
      ROS_INFO("obtaining control succeed!");
    }
    else
    {
      ROS_ERROR("obtaining control failed!");
      rate.sleep();
      continue;
    }

    if(is_M100())
    {
      ROS_INFO("connected to M100!");
      break;
    }else
    {
      ROS_INFO("The drone is not M100!");
      rate.sleep();
      continue;
    }
  }
  
  if(requestTaskSrvThreadRunning == false)//如果服务请求（起飞/降落/返航）线程还未启动，则先启动之
  {
    pthread_create(&requestTaskSrvThread,NULL,requestTaskService,NULL);
    ros::Duration(0.5).sleep();//等待线程启动
    ROS_INFO("requestTaskSrvThread is started.");
  }
  
  if(consistPubMoveCmdThreadRunning == false)
  {
    pthread_create(&consistPubMoveCmdThread,NULL,consistPubMoveCmd,NULL);
    ros::Duration(0.5).sleep();//等待线程启动
    ROS_INFO("consistPubMoveCmdThread is started.");
  }

  ////////////////////////////////////////
  ros::spin();
  /////////////////////////////////////////

  if(requestTaskSrvThreadRunning)
  {
    pthread_cancel(requestTaskSrvThread);
    pthread_join(requestTaskSrvThread,NULL);
        
    if(pthread_kill(requestTaskSrvThread,0) == ESRCH)//检测线程是否真的已经结束
    {
        ROS_INFO("requestTaskSrvThread is stopped.");
        requestTaskSrvThreadRunning = 0;
    }
  }

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



//通过服务（请求/应答）获取控制权
bool obtain_control()
{
  dji_sdk::SDKControlAuthority authority;
  authority.request.control_enable=1;
  sdk_ctrl_authority_service.call(authority);//调用服务，相当于向sdk请求控制权

  if(!authority.response.result)//处理请求结果
  {
    ROS_ERROR("obtain control failed!");
    return false;
  }

  return true;
}


//获取版本信息
bool is_M100()
{
  dji_sdk::QueryDroneVersion query;
  query_version_service.call(query);

  if(query.response.version == DJISDK::DroneFirmwareVersion::M100_31)
  {
    return true;
  }

  return false;
}

bool higher_cmd_priority(string cmd_name,int priority)
{
	if(priority >= cmd_pri_map[cmd_name])//优先级较高的指令
	{
	  cmd_pri_map[cmd_name] = priority;
	  return true;
	}
	else
	{
	  ROS_INFO("%s not executed for lower priority than %d!",cmd_name.c_str(),cmd_pri_map[cmd_name]);
	  return false;
	}
}


void hover()//悬停
{
  ROS_INFO("hovering...");

  requestTaskSrvThreadNeedToWork = false;
  consistPubMoveCmdThreadNeedToWork = false;
  square_mission.start = false;

  bzero(&glo_move_cmd,sizeof(glo_move_cmd));

  consistPubMoveCmdThreadNeedToWork = true;
}

//状态数据
//  ON_GROUND_STANDBY  = 1,
//  TAKEOFF            = 2,
//  IN_AIR_STANDBY     = 3,
//  LANDING            = 4, 
//  FINISHING_LANDING  = 5
int flight_status;
string flight_status_info[6]{"NULL","ON_GROUND_STANDBY","TAKEOFF","IN_AIR_STANDBY","LANDING","FINISHING_LANDING"};
void flight_status_callback(const std_msgs::UInt8::ConstPtr& msg)
{
  flight_status = msg->data;
}

void takeoff_cmd_callback(const m100_v2::PubSubUavMsg takeoff_cmd_msg)//起飞
{
  if(!higher_cmd_priority(takeoff_cmd,takeoff_cmd_msg.cmd_priority))//优先级较低的指令不执行
  {
    m100_v2::CmdResult result;
    result.component = takeoff_cmd_msg.component;
    result.command = takeoff_cmd;
    result.status = false;

    cmdResultPub.publish(result);//返回结果通告（持续发布指令的节点需要监听结果通告，以防止无效指令持续发布）

    return;
  }

  //计算指令响应延迟（实验用）
  action_cmd_start_time = takeoff_cmd_msg.start_time;

  if(requestTaskSrvThreadRunning == true)
  {
    if(flight_status == DJISDK::M100FlightStatus::M100_STATUS_ON_GROUND ||
                flight_status == DJISDK::M100FlightStatus::M100_STATUS_LANDING ||
                flight_status == DJISDK::M100FlightStatus::M100_STATUS_FINISHED_LANDING)//处于地面就绪/正在降落/完成降落状态时可以执行起飞命令
    {
      requestTaskSrvThreadNeedToWork = false;
      consistPubMoveCmdThreadNeedToWork = false;
      square_mission.start = false;

      requestTask = dji_sdk::DroneTaskControl::Request::TASK_TAKEOFF;

      requestTaskSrvThreadNeedToWork = true;
      gotNewActionCmd = true;
    }
    else
    {
      ROS_INFO("Do not need to take off: flight_status = %d",flight_status);
    }
  }

  else
  {
    ROS_INFO("takeoffCmd is not executed!");
  }
}

void land_cmd_callback(const m100_v2::PubSubUavMsg land_cmd_msg)//降落
{
  if(!higher_cmd_priority(land_cmd,land_cmd_msg.cmd_priority))
  {
    m100_v2::CmdResult result;
    result.component = land_cmd_msg.component;
    result.command = land_cmd;
    result.status = false;

    cmdResultPub.publish(result);

    return;
  }

  //计算指令响应延迟（实验用）
  action_cmd_start_time = land_cmd_msg.start_time;

  if(requestTaskSrvThreadRunning == true)
  {
    // if(flight_status == DJISDK::M100FlightStatus::M100_STATUS_TAKINGOFF ||
    //             flight_status == DJISDK::M100FlightStatus::M100_STATUS_IN_AIR)//处于正在起飞/空中就绪状态时可以执行降落命令
    // {      
      requestTaskSrvThreadNeedToWork = false;
      consistPubMoveCmdThreadNeedToWork = false;
      square_mission.start = false;

      requestTask = dji_sdk::DroneTaskControl::Request::TASK_LAND;

      requestTaskSrvThreadNeedToWork = true;
      gotNewActionCmd = true;
    // }
    // else
    // {
    //   ROS_INFO("Do not need to land: flight_status = %d",flight_status);
    // }   
  }

  else
  {
    ROS_INFO("landCmd is not executed!");
  }
}


void gohome_cmd_callback(const m100_v2::PubSubUavMsg gohome_cmd_msg)//返航
{
  if(!higher_cmd_priority(gohome_cmd,gohome_cmd_msg.cmd_priority))
  {
    m100_v2::CmdResult result;
    result.component = gohome_cmd_msg.component;
    result.command = gohome_cmd;
    result.status = false;

    cmdResultPub.publish(result);

    return;
  }

  //计算指令响应延迟（实验用）
  action_cmd_start_time = gohome_cmd_msg.start_time;

  if(requestTaskSrvThreadRunning == true)
  {
    if(flight_status == DJISDK::M100FlightStatus::M100_STATUS_TAKINGOFF ||
                flight_status == DJISDK::M100FlightStatus::M100_STATUS_IN_AIR ||
                flight_status == DJISDK::M100FlightStatus::M100_STATUS_LANDING)//处于正在起飞/空中就绪/正在降落状态时可以执行返航命令
    {
      // {//使用大疆提供的返航方式：先上升至10米，再返回原点，最后降落（当当前位置经纬度与原点经纬度相差不大时，则是直接降落）
      //   requestTaskSrvThreadNeedToWork = false;
      //   consistPubMoveCmdThreadNeedToWork = false;
      //   square_mission.start = false;

      //   requestTask = dji_sdk::DroneTaskControl::Request::TASK_GOHOME;

      //   requestTaskSrvThreadNeedToWork = true;
      //   gotNewActionCmd = true;
      // }

      {//使用自定义的返航方式：先上升至x米，再返回原点，最后悬停
        requestTaskSrvThreadNeedToWork = false;
        consistPubMoveCmdThreadNeedToWork = false;
        square_mission.start = false;

        task_waypoint.Clear();
        
        myTaskMessage::MyPoint* mp0;
        mp0 = task_waypoint.add_point();
        mp0->set_x(current_pos.x);
        mp0->set_y(current_pos.y);
        mp0->set_z(15);
        mp0->set_yaw(0);
        mp0->set_type(2);

        myTaskMessage::MyPoint* mp1;
        mp1 = task_waypoint.add_point();
        mp1->set_x(25);
        mp1->set_y(5);
        mp1->set_z(15);
        mp1->set_yaw(0);
        mp1->set_type(2);

        if(task_waypoint.point_size() > 0)
        { 
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
    }   
    else
    {
      ROS_INFO("Can not go home: flight_status = %d",flight_status);
    }
  }

  else
  {
    ROS_INFO("gohomeCmd is not executed!");
  }
}


void move_cmd_callback(const m100_v2::PubSubUavMsg move_cmd_msg)//订阅并分析控制指令
{
  //ROS_INFO("current priority:%d   move_cmd priority:%d",cmd_pri_map[move_cmd],move_cmd_msg.cmd_priority);
  if(!higher_cmd_priority(move_cmd,move_cmd_msg.cmd_priority))
  {
    m100_v2::CmdResult result;
    result.component = move_cmd_msg.component;
    result.command = move_cmd;
    result.status = false;

    cmdResultPub.publish(result);

    return;
  }
  //ROS_INFO("current priority:%d   move_cmd priority:%d",cmd_pri_map[move_cmd],move_cmd_msg.cmd_priority);

  //计算指令响应延迟（实验用）
 // speed_cmd_start_time = move_cmd_msg.start_time;

  gcsCommand::Twist twist;
  twist.ParseFromString(move_cmd_msg.playload);

  if(consistPubMoveCmdThreadRunning == true)
  {
    if(flight_status == DJISDK::M100FlightStatus::M100_STATUS_IN_AIR)//空中就绪状态时可以执行移动控制指令
    {
      requestTaskSrvThreadNeedToWork = false;
      consistPubMoveCmdThreadNeedToWork = false;
      square_mission.start = false;

      m100_v2::CmdResult result;
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
m100_v2::PubSubUavMsg wp_cmd_msg;
void waypoint_cmd_callback(const m100_v2::PubSubUavMsg waypoint_cmd_msg)//订阅并分析控制指令
{
  if(!higher_cmd_priority(waypoint_cmd,waypoint_cmd_msg.cmd_priority))
  {
    m100_v2::CmdResult result;
    result.component = waypoint_cmd_msg.component;
    result.command = waypoint_cmd;
    result.status = false;

    cmdResultPub.publish(result);

    return;
  }

  char finished_point[] = "m100_v2/log/finished_point.txt";
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
    if(flight_status == DJISDK::M100FlightStatus::M100_STATUS_IN_AIR)//处于正在起飞/空中就绪/正在降落状态时可以执行航点飞行任务
    {
      requestTaskSrvThreadNeedToWork = false;
      consistPubMoveCmdThreadNeedToWork = false;
      square_mission.start = false;

      task_waypoint.Clear();
      task_waypoint.ParseFromString(waypoint_cmd_msg.playload);

      // for(int i=0;i<task_waypoint.point_size();i++)
      // {
      //   printf("(%f,%f,%f,%f,%d)\n",task_waypoint.point(i).x(),task_waypoint.point(i).y(),task_waypoint.point(i).z(),
      //                             task_waypoint.point(i).yaw(),task_waypoint.point(i).type());
      // }

      if(task_waypoint.point_size() > 0)
      { 
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

      m100_v2::CmdResult result;
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

double ctlInterval = 0.01;
 //起飞/降落/返航
void* requestTaskService(void* args)
{
  requestTaskSrvThreadRunning = true;
  
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);//线程可被取消
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); //设置立即取消 

  //char action_cmd_delay[] = "m100_v2/log/action_cmd_delay.txt";
  //ofstream delay_str;
  //string action_cmd_delay_file(getDirectory() + action_cmd_delay);

  //delay_str.open(action_cmd_delay_file.c_str(),ios::out | ios::app);
  //delay_str << ros::Time::now() << endl;

  while(ros::ok())
  {
    while(requestTaskSrvThreadNeedToWork)
    {
      //动作指令响应延迟，实验用
      //if(delay_str && action_cmd_start_time>0)
      //{
      //    delay_str << (ros::Time::now().toSec()-action_cmd_start_time)*1000 << endl;
      //    action_cmd_start_time = -1;
      //}

      if(gotNewActionCmd == true)
      {
        gotNewActionCmd = false;

        int i;
        for(i=0;i<5;i++)//服务请求失败时，重复尝试5次
        {
          dji_sdk::DroneTaskControl droneTaskControl;
          droneTaskControl.request.task = requestTask;
          drone_task_service.call(droneTaskControl);//执行过程中无法被更改

          ros::Duration(0.5).sleep();//等待命令响应
          //ros::spinOnce();

          if(!droneTaskControl.response.result)
          {
            ROS_ERROR("actionCmd executing failed %d times! Trying again...",i+1);
            continue;
          }
          else
          {
            ROS_INFO("actionCmd executing succeed!");
            break;
          }
        }

        if(i >= 5)//5次失败后则任务该指令执行失败
        {
          ROS_ERROR("actionCmd executing failed! Something wrong with the drone_task_service!");
          continue;
        }


        ////////////////服务请求成功后，检测实际执行效果///////////////////
        //起飞命令，需要判断是否真的已经起飞到指定高度
        if(requestTask == dji_sdk::DroneTaskControl::Request::TASK_TAKEOFF)
        {
          ROS_INFO("m100 is taking off,please wait...");

          // Step 1: If M100 is not in the air after 5 seconds, fail.
          ros::Time start_time = ros::Time::now();
          while (ros::Time::now() - start_time < ros::Duration(5))
          {
            pthread_testcancel();//the thread can be killed only here（设置取消点，确保线程可取消）

            if(requestTaskSrvThreadNeedToWork == true && gotNewActionCmd == true)//检测起飞过程中被其他线程中止执行， 或者接到新指令
            {
              ROS_INFO("m100 taking off is interrupted!");
              break;
            }

            ros::Duration(0.5).sleep();

            if(flight_status == DJISDK::M100FlightStatus::M100_STATUS_IN_AIR && //通过订阅飞行状态消息来检测飞行状态
              current_pos.z >= 0.2)
            {
              ROS_INFO("m100 taking off succeed!");
              cmd_pri_map[takeoff_cmd] = 0;
              break;
            }
          }

          if(flight_status != DJISDK::M100FlightStatus::M100_STATUS_IN_AIR || //通过订阅飞行状态消息来检测飞行状态
              current_pos.z < 0.2)
          {
            ROS_ERROR("m100 taking off failed! flight_status = %s, current position is (%lf,%lf,%lf)",flight_status_info[flight_status].c_str(),current_pos.x,current_pos.y,current_pos.z);
            continue;
          }
        }

        //降落命令，需要判断是否真的已经降落到指定高度
        if(requestTask == dji_sdk::DroneTaskControl::Request::TASK_LAND)
        {
          ROS_INFO("m100 is landing,please wait...");

          ros::Time start_time = ros::Time::now();
          float last_altitude = current_pos.z;
          while(ros::ok())
          {
            while (ros::Time::now() - start_time < ros::Duration(2))
            {
              pthread_testcancel();//the thread can be killed only here（设置取消点，确保线程可取消）
                
              if(requestTaskSrvThreadNeedToWork == true && gotNewActionCmd == true)//检测降落过程中被其他线程中止执行， 或者接到新指令
              {
                ROS_INFO("m100 landing is interrupted!");
                break;
              }
              ros::Duration(0.5).sleep();
            }

            if(flight_status == DJISDK::M100FlightStatus::M100_STATUS_LANDING)//正在降落
            {
              if(last_altitude - current_pos.z > 0.2)//处在正在降落状态时能否2秒下降0.2米？
              { 
                last_altitude = current_pos.z; 
                start_time = ros::Time::now();
                continue;
              }
              else//处在降落状态时实际高度却没有明显下降
              {
                ROS_ERROR("m100 landing failed! flight_status = %s, current position is (%lf,%lf,%lf)",flight_status_info[flight_status].c_str(),current_pos.x,current_pos.y,current_pos.z);
                break;
              }
            }

            else if(flight_status == DJISDK::M100FlightStatus::M100_STATUS_FINISHED_LANDING ||
              flight_status == DJISDK::M100FlightStatus::M100_STATUS_ON_GROUND)//完成降落
            {
              ROS_INFO("m100 landing succeed!");
              cmd_pri_map[land_cmd] = 0;
              break;
            }

            else//其他状态
            {
              ROS_ERROR("m100 landing failed! flight_status = %s, current position is (%lf,%lf,%lf)",flight_status_info[flight_status].c_str(),current_pos.x,current_pos.y,current_pos.z);
              break;
            } 
          }
        }

        //返航(并降落)，需要判断是否真的返回到指定地点
        if(requestTask == dji_sdk::DroneTaskControl::Request::TASK_GOHOME)
        {
          ROS_INFO("m100 is going home,please wait...");

          ros::Time start_time = ros::Time::now();
          m100_v2::Position last_pos = current_pos;
          int slow_step_count = 0;
          while(ros::ok())
          {
            while (ros::Time::now() - start_time < ros::Duration(2))
            {
              pthread_testcancel();//the thread can be killed only here（设置取消点，确保线程可取消）

              if(requestTaskSrvThreadNeedToWork == true && gotNewActionCmd == true)//检测返航过程中被其他线程中止执行， 或者接到新指令
              {
                ROS_INFO("m100 going home is interrupted!");
                break;
              }
              ros::Duration(0.5).sleep();
            }

            if(flight_status == DJISDK::M100FlightStatus::M100_STATUS_FINISHED_LANDING ||
              flight_status == DJISDK::M100FlightStatus::M100_STATUS_ON_GROUND)//飞机已着陆
            {
              if(abs(current_pos.x) < 3 && abs(current_pos.y) < 3 && abs(current_pos.z) < 3)//从距离上判断是否返航成功
              {
                ROS_INFO("m100 going home succeed!");
                cmd_pri_map[gohome_cmd] = 0;
                break;
              }
              else
              {
                ROS_ERROR("m100 going home failed! flight_status = %s, current position is (%lf,%lf,%lf)",flight_status_info[flight_status].c_str(),current_pos.x,current_pos.y,current_pos.z);
                break;
              }
              
            }
            else//飞机未着陆
            {
              if(abs(current_pos.x-last_pos.x) > 0.1 || abs(current_pos.y-last_pos.y) > 0.1 || abs(current_pos.z-last_pos.z) > 0.1)//正在返航
              {
                slow_step_count = 0;
              }
              else
              {
                if(slow_step_count < 5)
                {
                  slow_step_count++;
                }
                else//连续5个周期没有明显移动
                {
                  ROS_ERROR("m100 going home failed! flight_status = %s, current position is (%lf,%lf,%lf)",flight_status_info[flight_status].c_str(),current_pos.x,current_pos.y,current_pos.z);
                  break;
                }
              }

              last_pos = current_pos;
              start_time = ros::Time::now();
            }
          }
        }
      }

      ros::Duration(ctrlInterval).sleep();
    }

    ros::Duration(0.5).sleep();
  }

  //delay_str.close();

  requestTaskSrvThreadRunning = false;
}

geometry_msgs::Vector3Stamped current_vel;
void velocity_callback(const geometry_msgs::Vector3Stamped& vel)//全局对地速度
{
    //ROS_INFO("v_x=%lf,v_y=%lf,v_z=%lf\n",vel.vector.x,vel.vector.y,vel.vector.z);
    //pthread_mutex_lock(&vel_mutex);
    current_vel = vel;
    //pthread_mutex_unlock(&vel_mutex);
}

m100_v2::ObstacleMsg obstacle;
void obstacle_callback(const m100_v2::ObstacleMsg& obst)
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

int backCmd = 0;
geometry_msgs::Twist last_ctl_vel,back_ctl_vel;
void emergencyObstacleAvoidance(geometry_msgs::Twist& ctl_vel)//应急避障
{
  geometry_msgs::Vector3Stamped temp_current_vel = current_vel;
  if((temp_current_vel.vector.x == 0 && temp_current_vel.vector.y == 0)
    || (ctl_vel.linear.x == 0 && ctl_vel.linear.y == 0))
  {
    return;
  }

  //计算当前运动速率与方向
  double move_speed = sqrt(temp_current_vel.vector.x*temp_current_vel.vector.x + temp_current_vel.vector.y*temp_current_vel.vector.y);//当前合速率
  double uav_radius = 1;  // 0.6

  //避障
  m100_v2::ObstacleMsg temp_obstacle = obstacle;
  double new_move_orient = 0;
  //ROS_INFO("deteced %ld obstacles.",temp_obstacle.center.size());

  if (backCmd && last_ctl_vel.linear.x!=ctl_vel.linear.x && last_ctl_vel.linear.y!=ctl_vel.linear.y)
  {
    backCmd -- ;
    ctl_vel.linear.x = back_ctl_vel.linear.x;
    ctl_vel.linear.y = back_ctl_vel.linear.y;
    ROS_INFO("Direct back");
    
    return;
  }
  else
    backCmd = 0;

  for(int i=0;i<temp_obstacle.center.size();i++)
  {
    //避障距离阈值
    double dist_threshold1 = uav_radius;
    double dist_threshold2 = 2*move_speed>dist_threshold1 ? 2*move_speed:2.5;
    double selfObstacle = 0.5;
    dist_threshold2 = 3;
    ros::param::get("~dist_threshold1",dist_threshold1);
    ros::param::get("~dist_threshold1",dist_threshold2);
    ros::param::get("~selfObstacle",selfObstacle);

    //障碍物角度转换到0～2PI内
    if(temp_obstacle.center[i].y < 0)
    {
      temp_obstacle.center[i].y += 360;
    }

    if(temp_obstacle.center[i].z < 0)
    {
      temp_obstacle.center[i].z += 360;
    }
    temp_obstacle.center[i].y *= deg2rad;
    temp_obstacle.center[i].z *= deg2rad;

    if(temp_obstacle.center[i].x < dist_threshold1 && temp_obstacle.center[i].x>selfObstacle)//小于机身半径的障碍物忽略不计
    {
      if (temp_obstacle.center[i].y<temp_obstacle.center[i].z)
        new_move_orient = (temp_obstacle.center[i].y + temp_obstacle.center[i].z + 2*PI)/2 + PI;
      else
        new_move_orient = (temp_obstacle.center[i].y + temp_obstacle.center[i].z)/2 + PI;
      if (new_move_orient>=2*PI)
        new_move_orient -= 2*PI;
      if (new_move_orient<0)
        new_move_orient +=2*PI;
        
      double new_move_speed = sqrt(ctl_vel.linear.x*ctl_vel.linear.x + ctl_vel.linear.y*ctl_vel.linear.y);
      ctl_vel.linear.x = new_move_speed * sin(new_move_orient);
      ctl_vel.linear.y = new_move_speed * cos(new_move_orient);
      back_ctl_vel = ctl_vel;

      backCmd = 5;
      ROS_INFO("Need Back!!! %lf, %lf, %lf", temp_obstacle.center[i].y*rad2deg, temp_obstacle.center[i].z*rad2deg, new_move_orient*rad2deg);
    }
    else if(temp_obstacle.center[i].x < dist_threshold2)//移动避障
    {
      ROS_INFO("obstacle %d Avoidancing, (%lf,%lf,%lf)",i,temp_obstacle.center[i].x,temp_obstacle.center[i].y,temp_obstacle.center[i].z);
      //当前运动方向,在全局坐标系内，顺时针递增，0~2PI
      double move_orient = 0;
      if(temp_current_vel.vector.y == 0)
      {
        move_orient = temp_current_vel.vector.x>0? 90*deg2rad:-90*deg2rad;
      }
      else
      {
        if(temp_current_vel.vector.x >= 0 && temp_current_vel.vector.y > 0)
        {
          move_orient = atan(temp_current_vel.vector.x/temp_current_vel.vector.y);
        }
        else if(temp_current_vel.vector.x >= 0 && temp_current_vel.vector.y < 0)
        {
          move_orient = atan(temp_current_vel.vector.x/temp_current_vel.vector.y) + PI;
        }
        else if(temp_current_vel.vector.x < 0 && temp_current_vel.vector.y < 0)
        {
          move_orient = atan(temp_current_vel.vector.x/temp_current_vel.vector.y) + PI;
        }
        else
        {
          move_orient = atan(temp_current_vel.vector.x/temp_current_vel.vector.y) + 2*PI;
        }
      }
      //ROS_INFO("speed=%lf,orient=%lf",move_speed,move_orient);

      //为了便于与障碍物角度作比较，将当前机身偏航角转换到顺时针递增的0~2PI内
      double yawInRad = current_atti.z;
      if(yawInRad < 0)
      {
        yawInRad = -yawInRad;
      }
      else
      {
        yawInRad = -yawInRad + 2*PI;
      }
      //ROS_INFO("yawInRad=%lf",yawInRad);

      //对地运动方向转为机身运动方向（与无人机机身y轴的夹角，偏右为正，单位为弧度）
      move_orient = move_orient - yawInRad;
      if(move_orient < 0)
      {
        move_orient = move_orient + 2*PI;
      }
      //ROS_INFO("move_orient=%lf",move_orient);

      //意图控制运动方向,顺时针递增，0~2PI
      double ctl_orient = 0;
      if(ctl_vel.linear.y == 0)
      {
        ctl_orient = ctl_vel.linear.x>0? 90*deg2rad:-90*deg2rad;
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
      //ROS_INFO("ctl_orient=%lf",ctl_orient);

      double expand_angle = 2*asin(2*uav_radius/temp_obstacle.center[i].x);//机身尺寸导致的避障物膨胀

      temp_obstacle.center[i].y += expand_angle;
      temp_obstacle.center[i].z -= expand_angle;

      if(temp_obstacle.center[i].y > 2*PI)   
      {
        temp_obstacle.center[i].y -= 2*PI;
      }

      if(temp_obstacle.center[i].z < 0)
      {
        temp_obstacle.center[i].z += 2*PI;
      }

      // ROS_INFO("move=%lf,ctl=%lf,f_deg=%lf,s_deg=%lf",move_orient,ctl_orient,temp_obstacle.center[i].y,temp_obstacle.center[i].z);

      //判断当前运动方向和意图控制方向是否有障碍物，如果有，则调整运动方向
      bool obstacle_in_front = false;
      // double new_move_orient = 0;
      new_move_orient = 0;
      if(temp_obstacle.center[i].y >= temp_obstacle.center[i].z)
      {
        if((move_orient >= temp_obstacle.center[i].z && move_orient <= temp_obstacle.center[i].y)//当前运动方向在障碍物夹角之内
          ||(ctl_orient >= temp_obstacle.center[i].z && ctl_orient <= temp_obstacle.center[i].y))//意图控制方向在障碍物夹角内
        {
          obstacle_in_front = true;

          if(abs(temp_obstacle.center[i].y-move_orient) <= abs(move_orient-temp_obstacle.center[i].z))//选择离障碍物边缘最近的角度为新的运动方向
          {
            new_move_orient = temp_obstacle.center[i].y;
            //new_move_orient = ctl_orient + PI/2;
          }
          else
          {
            new_move_orient = temp_obstacle.center[i].z;
            //new_move_orient = ctl_orient + PI/2;
          }
        }
        else//当前运动方向或控制方向不在障碍物夹角之内
        {
          obstacle_in_front = false;
        }
      }
      else
      {
        // if((move_orient >= temp_obstacle.center[i].z)//当前运动方向在障碍物夹角之内
        if((move_orient >= temp_obstacle.center[i].z )//当前运动方向在障碍物夹角之内
          && (ctl_orient >= temp_obstacle.center[i].z || ctl_orient <= temp_obstacle.center[i].y))//意图控制方向也在障碍物夹角内
        {
          obstacle_in_front = true;

          if(abs(temp_obstacle.center[i].y+2*PI-move_orient) <= abs(move_orient-temp_obstacle.center[i].z))//here
          {
            new_move_orient = temp_obstacle.center[i].y;
            // new_move_orient = ctl_orient + PI/2;
          }
          else
          {
            new_move_orient = temp_obstacle.center[i].z;
            // new_move_orient = ctl_orient + PI/2;
          }
        }

        else if((move_orient <= temp_obstacle.center[i].y)//当前运动方向在障碍物夹角之内
            && (ctl_orient >= temp_obstacle.center[i].z || ctl_orient <= temp_obstacle.center[i].y))//意图控制方向也在障碍物夹角内
        {
          obstacle_in_front = true;

          if(abs(temp_obstacle.center[i].y-move_orient) <= abs(move_orient+2*PI-temp_obstacle.center[i].z))
          {
            new_move_orient = temp_obstacle.center[i].y;
            //new_move_orient = ctl_orient + PI/2;
          }
          else
          {
            new_move_orient = temp_obstacle.center[i].z;
            //new_move_orient = ctl_orient + PI/2;
          }
        }
        else//当前运动方向或控制方向不在障碍物夹角之内
        {
          obstacle_in_front = false;
        }
      }
      last_ctl_vel = ctl_vel;   // 记录上次控制指令
      //避障：根据新的运动方向调整控制指令
      if(obstacle_in_front)
      {
        ROS_INFO("avoiding obstacle:(%lf,%lf,%lf)",temp_obstacle.center[i].x,temp_obstacle.center[i].y,temp_obstacle.center[i].z);
        ROS_INFO("move_orient=%lf,new_orient=%lf",move_orient,new_move_orient);

        double new_move_speed = sqrt(ctl_vel.linear.x*ctl_vel.linear.x + ctl_vel.linear.y*ctl_vel.linear.y);
        ctl_vel.linear.x = new_move_speed * sin(new_move_orient);
        ctl_vel.linear.y = new_move_speed * cos(new_move_orient);
        break;
      }
      else
      {
        ROS_INFO("ignore obstacle:(%lf,%lf,%lf),move_orient=%lf",temp_obstacle.center[i].x,temp_obstacle.center[i].y,temp_obstacle.center[i].z,move_speed);
      }
    }
    else//由上层任务处理避障
    {
      ROS_INFO("up obstacle %d, obstacle:(%lf,%lf,%lf),move_orient=%lf",i,temp_obstacle.center[i].x,temp_obstacle.center[i].y,temp_obstacle.center[i].z);
    }
  }
  //ROS_INFO("resault:new:%1f(%lf, %lf)",new_move_orient, ctl_vel.linear.x, ctl_vel.linear.y);
  //cout << "-----------------" << endl;
}

//移动
void* consistPubMoveCmd(void* args)//持续发布指令
{
  consistPubMoveCmdThreadRunning = true;

  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);//线程可被取消
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); //设置立即取消 

  char speed_cmd_delay[] = "m100_v2/log/speed_cmd_delay.txt";
  ofstream delay_str;
  string speed_cmd_delay_file(getDirectory() + speed_cmd_delay);

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
          delay_str << (ros::Time::now().toSec()-speed_cmd_start_time)*1000 << endl;
          speed_cmd_start_time = ros::Time::now().toSec();
      //}

	    static ros::Time start_time = ros::Time::now();
  	  ros::Duration elapsed_time = ros::Time::now() - start_time;//两次回调的时间间隔

      static ros::Time hover_start_time = ros::Time::now();
      static bool needToRestart = true;//悬停计时过程中有某个方向的速度不为零，则重新计时
      if(glo_move_cmd.linear.x+glo_move_cmd.linear.y+glo_move_cmd.linear.z+glo_move_cmd.angular.z == 0)
      {
        if(needToRestart)
        {
          hover_start_time = ros::Time::now();
          needToRestart = false;
        }

        if(ros::Time::now() - hover_start_time > ros::Duration(2.0)) //悬停超过一段时间则重置所有指令的优先级，以确保可以响应其他指令
        {
          cmd_pri_map[takeoff_cmd] = 0;
          cmd_pri_map[land_cmd] = 0;
          cmd_pri_map[gohome_cmd] = 0;
          cmd_pri_map[move_cmd] = 0;
          cmd_pri_map[waypoint_cmd] = 0;

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
          double yawInRad = current_atti.z;

          ctl_cmd.linear.x = glo_move_cmd.linear.x*cos(yawInRad) + glo_move_cmd.linear.y*sin(yawInRad);
          ctl_cmd.linear.y = -glo_move_cmd.linear.x*sin(yawInRad) + glo_move_cmd.linear.y*cos(yawInRad);
        }

        ctl_cmd.linear.z = glo_move_cmd.linear.z;
        ctl_cmd.angular.z = glo_move_cmd.angular.z;

        emergencyObstacleAvoidance(ctl_cmd);

        //ROS_INFO("vx=%lf,vy=%lf,vz=%lf,vyaw=%lf",ctl_cmd.linear.x,ctl_cmd.linear.y,ctl_cmd.linear.z,ctl_cmd.angular.z);

        //geometry_pub.publish(ctl_cmd);

        uint8_t flag = (DJISDK::VERTICAL_VELOCITY   |
                    DJISDK::HORIZONTAL_VELOCITY |
                    DJISDK::YAW_RATE            |
                    DJISDK::HORIZONTAL_BODY   |
                    DJISDK::STABLE_ENABLE);

        sensor_msgs::Joy moveCtrlCmd;
        moveCtrlCmd.axes.push_back(ctl_cmd.linear.x);
        moveCtrlCmd.axes.push_back(ctl_cmd.linear.y);
        moveCtrlCmd.axes.push_back(ctl_cmd.linear.z);
        moveCtrlCmd.axes.push_back(ctl_cmd.angular.z);
        moveCtrlCmd.axes.push_back(flag);

        ctrlGenericPub.publish(moveCtrlCmd);

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
void axisOffsetAnglePoint(myTaskMessage::MyPoint* mp)//自然坐标换成实验场地坐标转（针对gps坐标）
{
	double temp_x = mp->x() * cosOffsetAngle - mp->y() * sinOffsetAngle - axisOffsetX;
	double temp_y =-mp->x() * sinOffsetAngle + mp->y() * cosOffsetAngle - axisOffsetY;
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

        m100_v2::Position deltaDist;
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

        m100_v2::Position deltaDist = current_pos;

        mp->set_x(mp->x()-deltaDist.x);
        mp->set_y(mp->y()-deltaDist.y);
        mp->set_z(mp->z()-deltaDist.z);
      }
      break;
    case 1:
      /* 全局gps坐标转成航点距离坐标 */
      {
		    m100_v2::Position currPoint = current_pos;

        sensor_msgs::NavSatFix targetGps;
        targetGps.longitude = mp->x();
        targetGps.latitude = mp->y();
        targetGps.altitude = mp->z() + home_gps.altitude;

        geometry_msgs::Vector3 targetDist;
        localOffsetFromGpsOffset(targetDist, targetGps, home_gps);

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
void attitude_callback(geometry_msgs::Vector3 attitude)
{
  current_atti = attitude;

}

void gps_callback(sensor_msgs::NavSatFix msg)
{
  current_gps = msg;//按回调频率更新数据

  if(home_gps.longitude < 0 && home_gps.latitude < 0 && home_gps.altitude < 0)
  { 
    //如果没有手动设置home点，则在开机初始化时获取当前位置作为家的位置
    home_gps = current_gps;
  }
}

void position_callback(m100_v2::Position pos)//基于自然坐标系的位置更新
{
  current_pos = pos;
  
  static ros::Time start_time = ros::Time::now();
  ros::Duration elapsed_time = ros::Time::now() - start_time;//两次回调的时间间隔

  // Down sampled to 50Hz loop(两次执行的时间间隔大于20ms)
  if(elapsed_time > ros::Duration(0.02))
  {
    if(square_mission.start)
    {
      if(square_mission.finished)//完成一个航点
      {
        square_mission.reset();//清空相关控制变量
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

          cmd_pri_map[waypoint_cmd] = 0;


          m100_v2::CmdResult result;
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

    start_time = ros::Time::now();//重新计时
  }
}


void Mission::step()
{
  m100_v2::Position localOffset;
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
       else//距离小于2米时，速度为0.5m/s
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
//根据两对gps坐标换算出距离坐标（针对大疆m100的x轴和y轴转置做了计算调整）
void localOffsetFromGpsOffset(geometry_msgs::Vector3&  deltaNed,
                         sensor_msgs::NavSatFix& target,
                         sensor_msgs::NavSatFix& origin)
{
  double deltaLon = target.longitude - origin.longitude;
  double deltaLat = target.latitude - origin.latitude;

  deltaNed.x = deltaLon * deg2rad * C_EARTH * cos(deg2rad*target.latitude);
  deltaNed.y = deltaLat * deg2rad * C_EARTH;
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
