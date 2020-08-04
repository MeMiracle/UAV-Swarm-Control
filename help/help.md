[TOC]

# Topics
## Command Topics
### /cmd_vel
### /land_cmd
### /takeoff_cmd
### gohome_cmd

## Resource Pool Data Topics
### /attitude
返回当前无人机绕着x,y,z三轴的姿态信息,即旋转角度.
当`axisID == 2`时,返回局部坐标系下相对于起始无人机姿态的z轴角度信息,即北偏西+0～+180，北偏东-0～~180;
否则返回全局坐标下的姿态信息.
- 发送频率:10
- 数据类型:
```
geometry_msgs::Vector3

std_msgs/Header header
  uint32 seq
  time stamp
  string frame_id
geometry_msgs/Quaternion quaternion
  float64 x
  float64 y
  float64 z
  float64 w

```
- 例程:
```
geometry_msgs::Vector3 glo_attitude;
```

### /gps
返回当前无人机所在位置的GPS坐标,在该数据类型中只使用longitude、latitude、altitude坐标信息.
- 发送频率:10
- 数据类型:
```
sensor_msgs::NavSatFix

uint8 COVARIANCE_TYPE_UNKNOWN=0
uint8 COVARIANCE_TYPE_APPROXIMATED=1
uint8 COVARIANCE_TYPE_DIAGONAL_KNOWN=2
uint8 COVARIANCE_TYPE_KNOWN=3
std_msgs/Header header
  uint32 seq
  time stamp
  string frame_id
sensor_msgs/NavSatStatus status
  int8 STATUS_NO_FIX=-1
  int8 STATUS_FIX=0
  int8 STATUS_SBAS_FIX=1
  int8 STATUS_GBAS_FIX=2
  uint16 SERVICE_GPS=1
  uint16 SERVICE_GLONASS=2
  uint16 SERVICE_COMPASS=4
  uint16 SERVICE_GALILEO=8
  int8 status
  uint16 service
float64 latitude
float64 longitude
float64 altitude
float64[9] position_covariance
uint8 position_covariance_type
```
- 例程:
```
sensor_msgs::NavSatFix glo_current_gps;
```

### /velocity
返回无人机飞行速度.
当`axisID == 2`时返回相对于场地的速度,否则返回相对于地面速度.
- 发送频率:10
- 数据类型:geometry_msgs::Vector3Stamped
- 例程:
```
geometry_msgs::Vector3Stamped glo_velocity;
```

### /acceleration
返回无人机IMU数据.
当`axisID == 2`时返回相对于场地的加速度,否则返回相对于地面加速度.
- 发送频率:10
- 数据类型:geometry_msgs::Vector3
- 例程:
```

```

### /flight_status
主要有五中状态,不同状态对应标志位如下:
ON_GROUND_STANDBY  = 1,
TAKEOFF            = 2,
IN_AIR_STANDBY     = 3,
LANDING            = 4, 
/FINISHING_LANDING  = 5


- 发送频率:10
- 数据类型:std_msgs::UInt8
- 例程:
```
std_msgs::UInt8 glo_flight_status;
```

### /feedback_data
- 发送频率:10
- 数据类型:
```
m100_v2::PubSubUavMsg

int32 src_uav_id
string component
int32 cmd_priority
string playload
float64 start_time
```

- 例程:
```

```
## Other Data Topics
### /fix
### /magnetic
### /raw_imu
### /raw_imu/blas
### /flx_velocity
### /sonar_height
### /pressure_height
### /waypoint_cmd
### /obstacle
- 发送频率:1000
- 数据类型:m100_v2::ObstacleMsg
- 数据类型:
```
Header header
geometry_msgs/Point32[] center
```
- 例程:
```
// subcribe
ros::Subscriber obst_sub = nh.subscribe(string(uav_name + "/obstacle").c_str(),10,&getObstacle);

simulation_m100::ObstacleMsg temp_obstacle = obstacle;
for(int i=0;i<temp_obstacle.center.size();i++)
{
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
}
```
### /scan
- 发送频率:1000
- 数据类型:sensor_msgs::LaserScan
- 数据类型:
```
std_msgs/Header header
  uint32 seq
  time stamp
  string frame_id
float32 angle_min
float32 angle_max
float32 angle_increment
float32 time_increment
float32 scan_time
float32 range_min
float32 range_max
float32[] ranges
float32[] intensities
```
- 例程:
```
int count = scan->ranges.size();
cloud_msg->height = 1;
cloud_msg->width  = count;
cloud_msg->points.resize(cloud_msg->width * cloud_msg->height);
for(int i = 0; i < count; i++) {
	float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);  
	if(scan->ranges[i] && (_isinf(scan->ranges[i])==0)){
	    cloud_msg->points[i].x = scan->ranges[i]*cos(DEG2RAD(degree));
	    cloud_msg->points[i].y = scan->ranges[i]*sin(DEG2RAD(degree));
	    cloud_msg->points[i].z = 0;
	}
	else
	{
	    cloud_msg->points[i].x = 0;
	    cloud_msg->points[i].y = 0;
	    cloud_msg->points[i].z = 0;
	}
}
``` 
1
### /global_position
### /my_mapbuffer
### /other_uav_feedback_data
### /ground_truth/state
### /feedback_waypoint
### /pathplan_cmd
### /command_result
### /swarm_waypoint
### /move_cmd
### /my_formation_info
### /other_uavmapbuffer
### /other_uav_pos
### /other_uav_fd_waypoint
### /formation_cmd
### /other_uav_formation_info
### /robot_state_publisher
### /estop_relay
### /controller_spawner

# Reference
![](./images/ros_topics.svg) 

# m100ExeCmd.cpp
main()
初始化完成后创建两个线程:
requestTaskService
consistPubMoveCmd

正在线程是否正在运行标志位
1.requestTaskSrvThreadRunning 
2.consistPubMoveCmdThreadRunning

requestTaskSrvThreadNeedToWork
requestTaskSrvThreadNeedToWork

land_cmd_callback在被调用、执行时需要输入控制优先级,应该是接收到protobuf数据后才调用该函数.

452行::square_mission.finished = true; 应该为false?

# NODE in simulation_m100
## ExeCmd
```
main()
{
	Parameter initialization
	1._longitude=-1,_latitude=-1,_altitude=-1,未指定时,无人机开机位置即为起始位置(更新home_gps数据)
	
	Coordinate System Conversion:实验场地坐标系在自然全局坐标系原点以东为正x、以北为正y、北偏东夹角为正（度）,最新的校正后x轴正向指向正门,Y轴正向从B3指向B4
	
	Enable motors获取电机控制权

	Registration "consistPubMoveCmdThread" thread, 对应函数consistPubMoveCmd
	
}
```
```
consistPubMoveCmd
{
	记录当前时间到log文件
	while(consistPubMoveCmdThreadNeedToWork)
	{
		speed_cmd_start_time>0时记录时间,主要测试线程相应速度
		
		如果无人机没有接收到控制命令或者处于悬停状态超过2s,则将之前接收到控制命令的优先级全部置0,便于接收新的控制指令.
		
		
	}
}
```

### Subscribe Control Topics
#### /takeoff_cmd
#### /land_cmd
#### /gohome_cmd
#### /move_cmd
#### /waypoint_cmd

### Other Topic
#### /attitude
#### /gps
#### posTopic(/global_position or /local_position)
default (Now in use)
axisID == 1
posTopic="/global_position";

if axisID == 2
posTopic="/local_position";

#### /velocity
#### /obstacle

### Advertise topics
#### /cmd_vel
#### /command_result

### Service
#### /enable_motors


