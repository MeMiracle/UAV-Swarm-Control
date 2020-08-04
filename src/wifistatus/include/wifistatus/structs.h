#ifndef __STRUCTS_H
#define __STRUCTS_H

#include <string>

/*
 * This header file defines THE STRUCTS for uav_status and message_status
 */

//uav status
struct s_uav_status
{
	float ft_pos_x;     //x position
	float ft_pos_y;     //y position
	float ft_pos_z;     //z position
	float ft_attitude_x;    
	float ft_attitude_y;
	float ft_attitude_z;
	float ft_velocity_x;
	float ft_velocity_y;
	float ft_velocity_z;
};

//messgaeg status
struct s_msg_status
{
	int i_frame_id;
	int	i_msg_len;
	std::string str_src_address;
};

#endif