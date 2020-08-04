#include "wifistatus/structs.h"
#include "wifistatus/callback.h"

extern "C"
{
	#include "wifistatus/iw.h"
};

extern "C"
{
	extern int get_wifi_status(void);
};

extern struct s_wifi_status __wifi;
//struct for uav status
struct s_uav_status __uav;
//struct for send message
struct s_msg_status __send_msg;
//struct for recv message
struct s_msg_status __recv_msg;


void attitude_callback(geometry_msgs::Vector3 msg_attit)
{
    __uav.ft_attitude_x = msg_attit.x;
    __uav.ft_attitude_y = msg_attit.y;
    __uav.ft_attitude_z = msg_attit.z;
}

//基于自然坐标系的位置更新
void position_callback(wifistatus::Position msg_pos)
{
    __uav.ft_pos_x = msg_pos.x;
    __uav.ft_pos_y = msg_pos.y;
    __uav.ft_pos_z = msg_pos.z;
}

void velocity_callback(geometry_msgs::Vector3Stamped msg_velocity)
{
    __uav.ft_velocity_x = msg_velocity.vector.x;
    __uav.ft_velocity_y = msg_velocity.vector.y;
    __uav.ft_velocity_z = msg_velocity.vector.z;
}

void sendmsg_callback(wifistatus::CommInfo comm_send_info)
{
    __send_msg.i_frame_id = comm_send_info.frame_id;
    __send_msg.str_src_address  = comm_send_info.src_address;
    __send_msg.i_msg_len = comm_send_info.comm_bytes;

    // ROS_INFO("send_address:%s send_bytes:%d",__send_msg.str_src_address.c_str(),__send_msg.i_msg_len);
    save_wifi_uav_status(true);
    
}

void  recvmsg_callback(wifistatus::CommInfo comm_recv_info)
{
    __recv_msg.i_frame_id = comm_recv_info.frame_id;
    __recv_msg.str_src_address  = comm_recv_info.src_address;
    __recv_msg.i_msg_len = comm_recv_info.comm_bytes;

    // ROS_INFO("recv_address:%s recv_bytes:%d",__recv_msg.str_src_address.c_str(),__recv_msg.i_msg_len);
    
    save_wifi_uav_status(false);
}

static int save_wifi_uav_status(bool is_send)
{
    struct timeval tm_current;
    gettimeofday(&tm_current, NULL);

    char sz_log_fileName[] = "m100_v2/log/uav_wifi_status.txt";
	std::string str_workspace_path = getDirectory();
	std::string str_log_file(str_workspace_path + sz_log_fileName);

    std::ofstream of_log_file;
	of_log_file.open(str_log_file.c_str(), std::ios::out|std::ios::app);//open log file

    std::string str_log_header = (is_send ? "[SEND]": "[RECV]");
    
    std::string str_time = "Time:(" + TO_STRING(tm_current.tv_sec) + "." + TO_STRING(tm_current.tv_usec) +")";
    struct s_msg_status s_msg = (is_send ? __send_msg: __recv_msg);
    
    std::string str_msg("Message:(");
    str_msg += TO_STRING(s_msg.i_frame_id) + "," + s_msg.str_src_address + "," +  TO_STRING(s_msg.i_msg_len) + ")";

    std::string str_uav("Uav:(");
    str_uav += TO_STRING(__uav.ft_pos_x) + "," + TO_STRING(__uav.ft_pos_y) + "," + TO_STRING(__uav.ft_pos_z) + ")" \
                + "(" + TO_STRING(__uav.ft_attitude_x) + "," + TO_STRING(__uav.ft_attitude_y) + "," + TO_STRING(__uav.ft_attitude_z) + ")" \
                + "(" + TO_STRING(__uav.ft_velocity_x) + "," + TO_STRING(__uav.ft_velocity_y) + "," + TO_STRING(__uav.ft_velocity_z) + ")";
    
    get_wifi_status();
    std::string str_wifi("Wifi:(");
    str_wifi +=  __wifi.mac_addr;
    str_wifi += ",";
    str_wifi += __wifi.ifname;
    str_wifi += ",";
    str_wifi += TO_STRING(__wifi.freq) + ","\
                + TO_STRING(__wifi.rx_bytes_64) + "," + TO_STRING(__wifi.rx_bytes_32) + "," + TO_STRING(__wifi.rx_packages) \
                + TO_STRING(__wifi.tx_bytes_64) + "," + TO_STRING(__wifi.tx_bytes_32) + "," + TO_STRING(__wifi.tx_packages) \
                + TO_STRING(__wifi.tx_retries) + "," + TO_STRING(__wifi.tx_fails) + "," + TO_STRING(__wifi.beacon_loss) \
                + TO_STRING(__wifi.signal) + "," + TO_STRING(__wifi.expect_throughput) \
                + __wifi.rx_bitrate + "," + __wifi.tx_bitrate \
                + ")";

    std::string str_log = str_log_header + "\t" + str_time + "\t" +str_msg + "\t" + str_uav + "\t" + str_wifi;

    if(of_log_file)
    {
		of_log_file << str_log << std::endl;
	}

    of_log_file.close();
}

static std::string getDirectory()
{
    char abs_path[1024];
    int cnt = readlink("/proc/self/exe", abs_path, 1024);//获取可执行程序的绝对路径
    if(cnt < 0|| cnt >= 1024)
    {
        return NULL;
    }

    //最后一个'/' 后面是可执行程序名，去掉devel/lib/m100_v2/exe，只保留前面部分路径
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

    std::string path(abs_path);

    path = path + "src/";

    return path;
}
