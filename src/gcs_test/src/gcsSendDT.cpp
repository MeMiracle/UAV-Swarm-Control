#include <ros/ros.h>
//#include <zmq.h>
//#include <zhelpers.h>
#include <gcscommand.pb.h>
#include <gcscommand.pb.cc>
#include <taskmessage.pb.h>
#include <taskmessage.pb.cc>
#include <uavmessage.pb.h>
#include <uavmessage.pb.cc>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>

using namespace std;

int fd = 0;
struct termios getter,setter;


int seq_num = 0;
//void *publisher;

string workspace_path;

char gcs_address_config[] = "gcs_test/configure/gcs_address_config.txt";
//char address[30] = "tcp://*:5000";
char fly_plan_path4[] = "gcs_test/flyplan/flypath_dist_waypoint.txt";
char fly_plan_path5[] = "gcs_test/flyplan/flypath_dist_local.txt";
char fly_plan_path6[] = "gcs_test/flyplan/flypath_dist_global.txt";
char fly_plan_path7[] = "gcs_test/flyplan/flypath_gps_global.txt";
char fly_plan_path8[] = "gcs_test/flyplan/path_plan_point.txt";
char fly_plan_path9[] = "gcs_test/flyplan/swarm_target_point.txt";
char fly_plan_path0[] = "gcs_test/flyplan/formation_point.txt";

int tgt_uav_count = 0;
int tgt_uav_id = 0;

double linear_x = 1;
double linear_y = 1;
double linear_z = 1;
double angular_z = 1;

int recvKeyboardInput(char *key_code)
{
    int read_byte = read(fd,key_code,1);
    if(read_byte < 0)
    {
        perror("read error:");
    }

    else
    {
        //ROS_INFO("keyboard input: %c",*key_code);
        if(*key_code == 27)//方向键
        {
            read_byte = read(fd,key_code,1);
            //ROS_INFO("keyboard input: %c",*key_code);

            if(*key_code == 91 || *key_code == 'O')
            {
                //msghead.topic_id = 4;
                //msghead.msg_id = 2;
                //msghead.msg_length = sizeof(geometry_msg);

                read_byte = read(fd,key_code,1);
                //ROS_INFO("key_code=%d.",*key_code);
            }
        }
    }

    return read_byte;
}

void encodeCommand(uavMessage::Message &message,char key_code)
{
    //bzero(&msghead,sizeof(msghead));
    //bzero(&twist,sizeof(twist));

    message.clear_msghead();
    //message.clear_twist();

    message.mutable_msghead()->set_stx(254);
    message.mutable_msghead()->set_msg_type(1);
    message.mutable_msghead()->set_src_uav_id(0);
    message.mutable_msghead()->set_seq_num(seq_num++);
    message.mutable_msghead()->set_reserved(0);
            
    message.mutable_msghead()->set_tgt_uav_count(tgt_uav_count);
    message.mutable_msghead()->add_tgt_uav_id(tgt_uav_id);//对所有无人机

    message.mutable_msghead()->set_topic_id(4);//先默认设置为移动控制指令
    message.mutable_msghead()->set_msg_id(2);

    gcsCommand::Twist twist;
    myTaskMessage::TaskMessage task_waypoint;
    
    task_waypoint.Clear();
    twist.Clear();

    switch(key_code)
    {
        case 65://上升
            twist.set_linear_z(linear_z);
            //geometry_msg.linear.z = 0.5;
        break;

        case 66:
            twist.set_linear_z(-linear_z);
            //geometry_msg.linear.z = -0.5;
        break;

        case 67://旋转
            twist.set_angular_z(-angular_z);
            //geometry_msg.angular.z = -0.1;
        break;

        case 68:
            twist.set_angular_z(angular_z);
            //geometry_msg.angular.z = 0.1;
        break;

        case '1':
            message.mutable_msghead()->set_topic_id(1);//1，起飞
            message.mutable_msghead()->set_msg_id(1);//更正为动作控制指令
        break;

        case '2':
            message.mutable_msghead()->set_topic_id(2);//2,降落
            message.mutable_msghead()->set_msg_id(1);                        
        break;

        case '3':
            message.mutable_msghead()->set_topic_id(3);//3，重置或返航
            message.mutable_msghead()->set_msg_id(1);
        break;

        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
        {   
            message.mutable_msghead()->set_topic_id(9);//任务
            message.mutable_msghead()->set_msg_id(7);

            ifstream ifs;//读取文件
            string line;//每次读取一行
            string file_name;
            myTaskMessage::MyPoint* mp;
            float xdist,ydist,zdist,yawdist;//执行飞行脚本时的各方向距离和悬停时间
            uint32_t waypoint_type=0;
            uint32_t mark = 0;

			task_waypoint.Clear();
        
            if(key_code == 52)//4
            {
				task_waypoint.set_kind(0);

                file_name = workspace_path + fly_plan_path4;
                ifs.open(file_name.c_str(),ios::in|ios::out); 
            }
            else if(key_code == 53)//5
            {
				task_waypoint.set_kind(0);

                file_name = workspace_path + fly_plan_path5;
                ifs.open(file_name.c_str(),ios::in|ios::out);
            }
            else if(key_code == 54)//6
            {
				task_waypoint.set_kind(0);

                file_name = workspace_path + fly_plan_path6;
                ifs.open(file_name.c_str(),ios::in|ios::out);
            }
            else if(key_code == 55)//7
            {
				task_waypoint.set_kind(0);

                file_name = workspace_path + fly_plan_path7;
                ifs.open(file_name.c_str(),ios::in|ios::out);
            }
            else if(key_code == 56)//8,航迹规划
            {
				task_waypoint.set_kind(1);
				ROS_INFO("path plan cmd.");

                file_name = workspace_path + fly_plan_path8;
                ifs.open(file_name.c_str(),ios::in|ios::out);		
            }
            else if(key_code == 57)//9,群体智能
            {
				task_waypoint.set_kind(2);
				ROS_INFO("swarm intelligence cmd.");

                file_name = workspace_path + fly_plan_path9;
                ifs.open(file_name.c_str(),ios::in|ios::out);
            }
            else if(key_code == 48)//0,形成编队
            {
				task_waypoint.set_kind(3);
				ROS_INFO("formation cmd.");

                file_name = workspace_path + fly_plan_path0;
                ifs.open(file_name.c_str(),ios::in|ios::out);
            }

            if(!ifs)
            {
                perror("file open error:");
            }

            else
            {         
                while(getline(ifs,line))
                {
                    istringstream is(line);
                    is>>xdist>>ydist>>zdist>>yawdist>>waypoint_type>>mark;

                    mp = task_waypoint.add_point();
                    mp->set_x(xdist);
                    mp->set_y(ydist);
                    mp->set_z(zdist);
                    mp->set_yaw(yawdist);
                    mp->set_type(waypoint_type);
                    mp->set_mark(mark);
                }

                task_waypoint.set_clusterid(1);

                //ROS_INFO("point_count = %d",task_waypoint.point_size());

                ifs.close();
            }
        }
        break;

        case 'd'://机身速度控制
            //geometry_msg.linear.x = 0.1;//w
            twist.set_linear_x(linear_x);
        break;

        case 'a':
            //geometry_msg.linear.x = -0.1;//s
            twist.set_linear_x(-linear_x);
        break;

        case 'w':
            //geometry_msg.linear.y = 0.1;//a
            twist.set_linear_y(linear_y);
        break;

        case 's':
            //geometry_msg.linear.y = -0.1;//d
            twist.set_linear_y(-linear_y);
        break;

        case 'h'://全局坐标系内，对地速度控制
            //geometry_msg.linear.x = 0.1;//w
            twist.set_linear_x(linear_x);
            twist.set_angular_x(1);//用于区别机身速度和对地速度
        break;

        case 'f':
            //geometry_msg.linear.x = -0.1;//s
            twist.set_linear_x(-linear_x);
            twist.set_angular_x(1);
        break;

        case 't':
            //geometry_msg.linear.y = 0.1;//a
            twist.set_linear_y(linear_y);
            twist.set_angular_x(1);
        break;

        case 'g':
            //geometry_msg.linear.y = -0.1;//d
            twist.set_linear_y(-linear_y);
            twist.set_angular_x(1);
        break;

        case 'l'://实验坐标系内，对地速度控制
            //geometry_msg.linear.x = 0.1;//w
            twist.set_linear_x(linear_x);
            twist.set_angular_x(2);//用于区别机身速度和对地速度
        break;

        case 'j':
            //geometry_msg.linear.x = -0.1;//s
            twist.set_linear_x(-linear_x);
            twist.set_angular_x(2);
        break;

        case 'i':
            //geometry_msg.linear.y = 0.1;//a
            twist.set_linear_y(linear_y);
            twist.set_angular_x(2);
        break;

        case 'k':
            //geometry_msg.linear.y = -0.1;//d
            twist.set_linear_y(-linear_y);
            twist.set_angular_x(2);
        break;

        case 'z':
            linear_x++;//调节速度
            if(linear_x > 22)
            {
                linear_x = 22;
            }
            ROS_INFO("vx = %lf",linear_x);
            message.mutable_msghead()->set_stx(0);//不发送指令
        break;

        case 'x':
            linear_x--;
            if(linear_x < 0)
            {
                linear_x = 0;
            }
            ROS_INFO("vx = %lf",linear_x);
            message.mutable_msghead()->set_stx(0);
        break;

        case 'c':
            linear_y++;
            if(linear_y > 22)
            {
                linear_y = 22;
            }
            ROS_INFO("vy = %lf",linear_y);
            message.mutable_msghead()->set_stx(0);
        break;

         case 'v':
            linear_y--;
            if(linear_y < 0)
            {
                linear_y = 0;
            }
            ROS_INFO("vy = %lf",linear_y);
            message.mutable_msghead()->set_stx(0);
        break;

         case 'b':
            linear_z++;
            if(linear_z > 5)
            {
                linear_z = 5;
            }
            ROS_INFO("vz = %lf",linear_z);
            message.mutable_msghead()->set_stx(0);
        break;

        case 'n':
            linear_z--;
            if(linear_z < 0)
            {
                linear_z = 0;
            }
            ROS_INFO("vz = %lf",linear_z);
            message.mutable_msghead()->set_stx(0);
        break;

        case 'm':
            angular_z++;
            if(angular_z > 5)
            {
                angular_z = 5;
            }
            ROS_INFO("vyaw = %lf",angular_z);
            message.mutable_msghead()->set_stx(0);
        break;

        case ',':
            angular_z--;
            if(angular_z < 0)
            {
                angular_z = 0;
            }
            ROS_INFO("vyaw = %lf",angular_z);
            message.mutable_msghead()->set_stx(0);
        break;

        case 'P'://f1，控制uav1
            tgt_uav_count = 1;
            tgt_uav_id = 1;
            ROS_INFO("controling uav1...");
            message.mutable_msghead()->set_stx(0);
        break;

        case 'Q'://f2，控制uav2
            tgt_uav_count = 1;
            tgt_uav_id = 2;
            ROS_INFO("controling uav2...");
            message.mutable_msghead()->set_stx(0);
        break;

        case 'R'://f3，控制uav3
            tgt_uav_count = 1;
            tgt_uav_id = 3;
            ROS_INFO("controling uav3...");
            message.mutable_msghead()->set_stx(0);
        break;

        case 'S'://f4，控制所有uav
            tgt_uav_count = 0;
            tgt_uav_id = 0;
            ROS_INFO("controling all uavs...");
            message.mutable_msghead()->set_stx(0);
        break;

        case ' '://空格，悬停
            //bzero(&geometry_msg,sizeof(geometry_msg));
            //bzero((void*) &message.twist(),sizeof( message.twist()) );//不能用bzero清零一个类
            twist.Clear();
        break;

        default:
            //bzero(&message,sizeof(message));//不能用bzero清零一个类
            message.clear_msghead();
        break;
    }

    if(message.msghead().stx() != 0)
    {
        if(message.msghead().msg_id() == 1)//动作指令
        {
            message.mutable_msghead()->set_msg_length(0);
        }

        else if(message.msghead().msg_id() == 2)//移动指令
        {
            string twist_str;
            if(!twist.SerializeToString(&twist_str))
            {
                perror("serialize error:");
                return;
            }

            message.set_playload(twist_str);
            message.mutable_msghead()->set_msg_length(twist_str.size());
        }

        else if(message.msghead().msg_id() == 7 || message.msghead().msg_id() == 9)//航点/航迹规划指令
        {
            string task_waypoint_str;
            if(!task_waypoint.SerializeToString(&task_waypoint_str))
            {
                perror("serialize error:");
                return;
            }

            message.set_playload(task_waypoint_str);
            message.mutable_msghead()->set_msg_length(task_waypoint_str.size());
        }

        else
        {
            message.mutable_msghead()->set_stx(0);
        }
        
    }      
}

int sendCommandToUAV(int sock_toserver,uavMessage::Message message)
{
    int send_byte = 0;

    if(message.msghead().stx() != 0)
    {
        //message.set_allocated_msghead(&msghead);
        //message.set_allocated_twist(&twist);
        //cout<<"linear_z="<<message.twist().linear_z()<<endl;

        string send_str;// = malloc(msg_size);
                        
        //消息序列化成string
        //cout<<"serializing..."<<endl;
        if(!message.SerializeToString(&send_str))
        {
            perror("serialize error:");
            return -1;
        }

        int bytes = write(sock_toserver,send_str.c_str(),send_str.size());
        ROS_INFO("send %d bytes:%s",bytes,send_str.c_str());

/*      //打印string
        ROS_INFO("stirng %d bytes:",send_str.size());
        for(int i=0;i<send_str.size();i++)
        {
            ROS_INFO("%d ",send_str[i]);
        }
        ROS_INFO("\n");
*/

/*      //string转成char*，并打印char*
        char *send_ch = (char*)malloc(send_str.size());
        memcpy(send_ch,send_str.c_str(),send_str.size());
        ROS_INFO("char %d bytes:",send_str.size());
        for(int i=0;i<send_str.size();i++)
        {
            ROS_INFO("%d ",send_ch[i]);
        }
        ROS_INFO("\n");
*/   

        // //string*转成zmq_mst_t*
        // zmq_msg_t send_msg;
        // zmq_msg_init_size(&send_msg,send_str.size());
        // memcpy(zmq_msg_data(&send_msg),send_str.c_str(),send_str.size());
                    
        //free(send_ch);


/*      //打印zmq_msg_t
        char *p = (char*)zmq_msg_data(&send_msg);
        ROS_INFO("zmq_msg_t %d byte:",zmq_msg_size(&send_msg));
        for(int i=0;i<zmq_msg_size(&send_msg);i++)
        {
            ROS_INFO("%d ",p[i]);
        }
        ROS_INFO("\n");
*/

        //发送zmq_msg_t
        // send_byte = zmq_msg_send(&send_msg,publisher,0);
        // double time = (double)ros::Time::now().toSec()*1000;
        // ROS_INFO("send %d bytes.",send_byte);
                        
        // zmq_msg_close(&send_msg);


/*      //test demo
        int zipcode = 254,temperature=27,relhumidity=30;
        char update[20];
        sprintf(update,"%05d %d %d",zipcode,temperature,relhumidity);

        int send_byte = s_send(publisher,update);
        ROS_INFO("send %d byte.",send_byte);
*/
    }

    else
    {
        ROS_INFO("command not found.");
    }

    return send_byte;
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

int connectToDataTran()
{
    ROS_INFO("trying to connect to server");

    int sock_toserver = -1;

    while(ros::ok())
    {
        sock_toserver = socket(AF_INET,SOCK_STREAM,0);
        if(-1 == sock_toserver)
        {
            ROS_INFO("creating socket failed!");
            break;
        }

        struct sockaddr_in server_addr;
        bzero(&server_addr,sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(42766);
        server_addr.sin_addr.s_addr = inet_addr("192.168.4.101");

        ros::Rate rate(1);
        while(ros::ok())
        {
            if(-1 == connect(sock_toserver,(struct sockaddr*)(&server_addr),sizeof(struct sockaddr)))
            {
                ROS_ERROR("connecting failed:");
                rate.sleep();
                continue;
            }

            else
            {
                ROS_INFO("connected to server!");
                return sock_toserver;
            }
        }
    }

    return sock_toserver;
}

int main(int argc, char *argv[])
{
    /* code for main function */
    ros::init(argc, argv, "gcsSendDT");
    ros::NodeHandle nh;

    ROS_INFO("gcsSendDT_node starting.");

    workspace_path = getDirectory();

    int sock_toserver = connectToDataTran();

    
    tcgetattr(fd,&getter);
    memcpy(&setter,&getter,sizeof(struct termios));
    setter.c_lflag &= ~(ICANON | ECHO);
    setter.c_cc[VEOL] = 1;
    setter.c_cc[VEOF] = 2;
    tcsetattr(fd,TCSANOW,&setter);

    ROS_INFO("operating by keyboad...");
    ROS_INFO("1:TakeOff  2:Land  3:GoHome");
    ROS_INFO("4~7:Waypoints  8:PathPlan");
    ROS_INFO("9:SwarmIntel   0:Formation");
    ROS_INFO("key_up:Up  key_down:Down");
    ROS_INFO("key_left:TurnLeft key_right:TurnRight");
    ROS_INFO("SelfCtl w:+Vy    s:-Vy d:+Vx d:-Vx");
    ROS_INFO("GlobalCtl t:North  g:South h:East f:West");
    ROS_INFO("LocalCtl i:+Vy   k:-Vy l:+Vx j:-Vx");
    ROS_INFO("SelfCtl w:+Vy  s:-Vy d:+Vx d:-Vx");
    ROS_INFO("z:Vx++  x:Vx--  c:Vy++  v:Vy--");
    ROS_INFO("b:Vz++  n:Vz--  m:yaw++  <:yaw--");

    //struct MsgHead msghead;
    //std_msgs::Empty empty_msg;
    //geometry_msgs::Twist geometry_msg;
    
    char key_code;

    while(ros::ok())
    {
        uavMessage::Message message;
        int read_byte = recvKeyboardInput(&key_code);

        if(read_byte > 0)
        {
            encodeCommand(message,key_code);

            if(message.msghead().stx() != 0)
            {
                sendCommandToUAV(sock_toserver,message);     
            }
        }
    }

    //message.clear_msghead();
    //message.clear_playload();

    tcsetattr(fd,TCSANOW,&getter);

    ros::shutdown();
    return 0;
}