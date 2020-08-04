# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "m100_v2: 8 messages, 1 services")

set(MSG_I_FLAGS "-Im100_v2:/home/uav/ROS/m100/src/m100_v2/msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(m100_v2_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/srv/EnableMotors.srv" NAME_WE)
add_custom_target(_m100_v2_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "m100_v2" "/home/uav/ROS/m100/src/m100_v2/srv/EnableMotors.srv" ""
)

get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/UavBaseInfo.msg" NAME_WE)
add_custom_target(_m100_v2_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "m100_v2" "/home/uav/ROS/m100/src/m100_v2/msg/UavBaseInfo.msg" ""
)

get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/PubSubUavMsg.msg" NAME_WE)
add_custom_target(_m100_v2_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "m100_v2" "/home/uav/ROS/m100/src/m100_v2/msg/PubSubUavMsg.msg" ""
)

get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/Position.msg" NAME_WE)
add_custom_target(_m100_v2_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "m100_v2" "/home/uav/ROS/m100/src/m100_v2/msg/Position.msg" ""
)

get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/CmdResult.msg" NAME_WE)
add_custom_target(_m100_v2_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "m100_v2" "/home/uav/ROS/m100/src/m100_v2/msg/CmdResult.msg" ""
)

get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/CommInfo.msg" NAME_WE)
add_custom_target(_m100_v2_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "m100_v2" "/home/uav/ROS/m100/src/m100_v2/msg/CommInfo.msg" ""
)

get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/ObstacleMsg.msg" NAME_WE)
add_custom_target(_m100_v2_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "m100_v2" "/home/uav/ROS/m100/src/m100_v2/msg/ObstacleMsg.msg" "geometry_msgs/Point32:std_msgs/Header"
)

get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/GimbalInfo.msg" NAME_WE)
add_custom_target(_m100_v2_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "m100_v2" "/home/uav/ROS/m100/src/m100_v2/msg/GimbalInfo.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/UavBaseInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/m100_v2
)
_generate_msg_cpp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/PubSubUavMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/m100_v2
)
_generate_msg_cpp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/m100_v2
)
_generate_msg_cpp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/CmdResult.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/m100_v2
)
_generate_msg_cpp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/m100_v2
)
_generate_msg_cpp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/m100_v2
)
_generate_msg_cpp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/GimbalInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/m100_v2
)

### Generating Services
_generate_srv_cpp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/srv/EnableMotors.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/m100_v2
)

### Generating Module File
_generate_module_cpp(m100_v2
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/m100_v2
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(m100_v2_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(m100_v2_generate_messages m100_v2_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/srv/EnableMotors.srv" NAME_WE)
add_dependencies(m100_v2_generate_messages_cpp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/UavBaseInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_cpp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/PubSubUavMsg.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_cpp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/Position.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_cpp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/CmdResult.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_cpp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/CommInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_cpp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_cpp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/GimbalInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_cpp _m100_v2_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(m100_v2_gencpp)
add_dependencies(m100_v2_gencpp m100_v2_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS m100_v2_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/UavBaseInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/m100_v2
)
_generate_msg_eus(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/PubSubUavMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/m100_v2
)
_generate_msg_eus(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/m100_v2
)
_generate_msg_eus(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/CmdResult.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/m100_v2
)
_generate_msg_eus(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/m100_v2
)
_generate_msg_eus(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/m100_v2
)
_generate_msg_eus(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/GimbalInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/m100_v2
)

### Generating Services
_generate_srv_eus(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/srv/EnableMotors.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/m100_v2
)

### Generating Module File
_generate_module_eus(m100_v2
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/m100_v2
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(m100_v2_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(m100_v2_generate_messages m100_v2_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/srv/EnableMotors.srv" NAME_WE)
add_dependencies(m100_v2_generate_messages_eus _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/UavBaseInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_eus _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/PubSubUavMsg.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_eus _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/Position.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_eus _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/CmdResult.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_eus _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/CommInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_eus _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_eus _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/GimbalInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_eus _m100_v2_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(m100_v2_geneus)
add_dependencies(m100_v2_geneus m100_v2_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS m100_v2_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/UavBaseInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/m100_v2
)
_generate_msg_lisp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/PubSubUavMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/m100_v2
)
_generate_msg_lisp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/m100_v2
)
_generate_msg_lisp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/CmdResult.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/m100_v2
)
_generate_msg_lisp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/m100_v2
)
_generate_msg_lisp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/m100_v2
)
_generate_msg_lisp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/GimbalInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/m100_v2
)

### Generating Services
_generate_srv_lisp(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/srv/EnableMotors.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/m100_v2
)

### Generating Module File
_generate_module_lisp(m100_v2
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/m100_v2
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(m100_v2_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(m100_v2_generate_messages m100_v2_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/srv/EnableMotors.srv" NAME_WE)
add_dependencies(m100_v2_generate_messages_lisp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/UavBaseInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_lisp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/PubSubUavMsg.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_lisp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/Position.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_lisp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/CmdResult.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_lisp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/CommInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_lisp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_lisp _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/GimbalInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_lisp _m100_v2_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(m100_v2_genlisp)
add_dependencies(m100_v2_genlisp m100_v2_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS m100_v2_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/UavBaseInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/m100_v2
)
_generate_msg_nodejs(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/PubSubUavMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/m100_v2
)
_generate_msg_nodejs(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/m100_v2
)
_generate_msg_nodejs(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/CmdResult.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/m100_v2
)
_generate_msg_nodejs(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/m100_v2
)
_generate_msg_nodejs(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/m100_v2
)
_generate_msg_nodejs(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/GimbalInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/m100_v2
)

### Generating Services
_generate_srv_nodejs(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/srv/EnableMotors.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/m100_v2
)

### Generating Module File
_generate_module_nodejs(m100_v2
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/m100_v2
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(m100_v2_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(m100_v2_generate_messages m100_v2_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/srv/EnableMotors.srv" NAME_WE)
add_dependencies(m100_v2_generate_messages_nodejs _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/UavBaseInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_nodejs _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/PubSubUavMsg.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_nodejs _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/Position.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_nodejs _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/CmdResult.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_nodejs _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/CommInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_nodejs _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_nodejs _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/GimbalInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_nodejs _m100_v2_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(m100_v2_gennodejs)
add_dependencies(m100_v2_gennodejs m100_v2_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS m100_v2_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/UavBaseInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/m100_v2
)
_generate_msg_py(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/PubSubUavMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/m100_v2
)
_generate_msg_py(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/m100_v2
)
_generate_msg_py(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/CmdResult.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/m100_v2
)
_generate_msg_py(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/m100_v2
)
_generate_msg_py(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/m100_v2
)
_generate_msg_py(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/msg/GimbalInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/m100_v2
)

### Generating Services
_generate_srv_py(m100_v2
  "/home/uav/ROS/m100/src/m100_v2/srv/EnableMotors.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/m100_v2
)

### Generating Module File
_generate_module_py(m100_v2
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/m100_v2
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(m100_v2_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(m100_v2_generate_messages m100_v2_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/srv/EnableMotors.srv" NAME_WE)
add_dependencies(m100_v2_generate_messages_py _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/UavBaseInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_py _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/PubSubUavMsg.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_py _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/Position.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_py _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/CmdResult.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_py _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/CommInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_py _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_py _m100_v2_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/m100_v2/msg/GimbalInfo.msg" NAME_WE)
add_dependencies(m100_v2_generate_messages_py _m100_v2_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(m100_v2_genpy)
add_dependencies(m100_v2_genpy m100_v2_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS m100_v2_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/m100_v2)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/m100_v2
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(m100_v2_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(m100_v2_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/m100_v2)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/m100_v2
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(m100_v2_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(m100_v2_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/m100_v2)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/m100_v2
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(m100_v2_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(m100_v2_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/m100_v2)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/m100_v2
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(m100_v2_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(m100_v2_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/m100_v2)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/m100_v2\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/m100_v2
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(m100_v2_generate_messages_py geometry_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(m100_v2_generate_messages_py std_msgs_generate_messages_py)
endif()
