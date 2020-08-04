# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "simulation_m100: 6 messages, 1 services")

set(MSG_I_FLAGS "-Isimulation_m100:/home/uav/ROS/m100/src/simulation_m100/msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(simulation_m100_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/PubSubUavMsg.msg" NAME_WE)
add_custom_target(_simulation_m100_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "simulation_m100" "/home/uav/ROS/m100/src/simulation_m100/msg/PubSubUavMsg.msg" ""
)

get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/CmdResult.msg" NAME_WE)
add_custom_target(_simulation_m100_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "simulation_m100" "/home/uav/ROS/m100/src/simulation_m100/msg/CmdResult.msg" ""
)

get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/Position.msg" NAME_WE)
add_custom_target(_simulation_m100_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "simulation_m100" "/home/uav/ROS/m100/src/simulation_m100/msg/Position.msg" ""
)

get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/ObstacleMsg.msg" NAME_WE)
add_custom_target(_simulation_m100_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "simulation_m100" "/home/uav/ROS/m100/src/simulation_m100/msg/ObstacleMsg.msg" "geometry_msgs/Point32:std_msgs/Header"
)

get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/srv/EnableMotors.srv" NAME_WE)
add_custom_target(_simulation_m100_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "simulation_m100" "/home/uav/ROS/m100/src/simulation_m100/srv/EnableMotors.srv" ""
)

get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/CommInfo.msg" NAME_WE)
add_custom_target(_simulation_m100_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "simulation_m100" "/home/uav/ROS/m100/src/simulation_m100/msg/CommInfo.msg" ""
)

get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/UavBaseInfo.msg" NAME_WE)
add_custom_target(_simulation_m100_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "simulation_m100" "/home/uav/ROS/m100/src/simulation_m100/msg/UavBaseInfo.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/PubSubUavMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/simulation_m100
)
_generate_msg_cpp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/CmdResult.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/simulation_m100
)
_generate_msg_cpp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/simulation_m100
)
_generate_msg_cpp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/simulation_m100
)
_generate_msg_cpp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/simulation_m100
)
_generate_msg_cpp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/UavBaseInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/simulation_m100
)

### Generating Services
_generate_srv_cpp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/srv/EnableMotors.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/simulation_m100
)

### Generating Module File
_generate_module_cpp(simulation_m100
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/simulation_m100
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(simulation_m100_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(simulation_m100_generate_messages simulation_m100_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/PubSubUavMsg.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_cpp _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/CmdResult.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_cpp _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/Position.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_cpp _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_cpp _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/srv/EnableMotors.srv" NAME_WE)
add_dependencies(simulation_m100_generate_messages_cpp _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/CommInfo.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_cpp _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/UavBaseInfo.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_cpp _simulation_m100_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(simulation_m100_gencpp)
add_dependencies(simulation_m100_gencpp simulation_m100_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS simulation_m100_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/PubSubUavMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/simulation_m100
)
_generate_msg_eus(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/CmdResult.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/simulation_m100
)
_generate_msg_eus(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/simulation_m100
)
_generate_msg_eus(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/simulation_m100
)
_generate_msg_eus(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/simulation_m100
)
_generate_msg_eus(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/UavBaseInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/simulation_m100
)

### Generating Services
_generate_srv_eus(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/srv/EnableMotors.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/simulation_m100
)

### Generating Module File
_generate_module_eus(simulation_m100
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/simulation_m100
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(simulation_m100_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(simulation_m100_generate_messages simulation_m100_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/PubSubUavMsg.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_eus _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/CmdResult.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_eus _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/Position.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_eus _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_eus _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/srv/EnableMotors.srv" NAME_WE)
add_dependencies(simulation_m100_generate_messages_eus _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/CommInfo.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_eus _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/UavBaseInfo.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_eus _simulation_m100_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(simulation_m100_geneus)
add_dependencies(simulation_m100_geneus simulation_m100_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS simulation_m100_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/PubSubUavMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/simulation_m100
)
_generate_msg_lisp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/CmdResult.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/simulation_m100
)
_generate_msg_lisp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/simulation_m100
)
_generate_msg_lisp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/simulation_m100
)
_generate_msg_lisp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/simulation_m100
)
_generate_msg_lisp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/UavBaseInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/simulation_m100
)

### Generating Services
_generate_srv_lisp(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/srv/EnableMotors.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/simulation_m100
)

### Generating Module File
_generate_module_lisp(simulation_m100
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/simulation_m100
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(simulation_m100_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(simulation_m100_generate_messages simulation_m100_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/PubSubUavMsg.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_lisp _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/CmdResult.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_lisp _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/Position.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_lisp _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_lisp _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/srv/EnableMotors.srv" NAME_WE)
add_dependencies(simulation_m100_generate_messages_lisp _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/CommInfo.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_lisp _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/UavBaseInfo.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_lisp _simulation_m100_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(simulation_m100_genlisp)
add_dependencies(simulation_m100_genlisp simulation_m100_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS simulation_m100_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/PubSubUavMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/simulation_m100
)
_generate_msg_nodejs(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/CmdResult.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/simulation_m100
)
_generate_msg_nodejs(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/simulation_m100
)
_generate_msg_nodejs(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/simulation_m100
)
_generate_msg_nodejs(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/simulation_m100
)
_generate_msg_nodejs(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/UavBaseInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/simulation_m100
)

### Generating Services
_generate_srv_nodejs(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/srv/EnableMotors.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/simulation_m100
)

### Generating Module File
_generate_module_nodejs(simulation_m100
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/simulation_m100
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(simulation_m100_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(simulation_m100_generate_messages simulation_m100_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/PubSubUavMsg.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_nodejs _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/CmdResult.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_nodejs _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/Position.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_nodejs _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_nodejs _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/srv/EnableMotors.srv" NAME_WE)
add_dependencies(simulation_m100_generate_messages_nodejs _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/CommInfo.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_nodejs _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/UavBaseInfo.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_nodejs _simulation_m100_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(simulation_m100_gennodejs)
add_dependencies(simulation_m100_gennodejs simulation_m100_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS simulation_m100_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/PubSubUavMsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/simulation_m100
)
_generate_msg_py(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/CmdResult.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/simulation_m100
)
_generate_msg_py(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/simulation_m100
)
_generate_msg_py(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/simulation_m100
)
_generate_msg_py(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/simulation_m100
)
_generate_msg_py(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/msg/UavBaseInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/simulation_m100
)

### Generating Services
_generate_srv_py(simulation_m100
  "/home/uav/ROS/m100/src/simulation_m100/srv/EnableMotors.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/simulation_m100
)

### Generating Module File
_generate_module_py(simulation_m100
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/simulation_m100
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(simulation_m100_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(simulation_m100_generate_messages simulation_m100_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/PubSubUavMsg.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_py _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/CmdResult.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_py _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/Position.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_py _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_py _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/srv/EnableMotors.srv" NAME_WE)
add_dependencies(simulation_m100_generate_messages_py _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/CommInfo.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_py _simulation_m100_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/simulation_m100/msg/UavBaseInfo.msg" NAME_WE)
add_dependencies(simulation_m100_generate_messages_py _simulation_m100_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(simulation_m100_genpy)
add_dependencies(simulation_m100_genpy simulation_m100_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS simulation_m100_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/simulation_m100)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/simulation_m100
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(simulation_m100_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(simulation_m100_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/simulation_m100)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/simulation_m100
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(simulation_m100_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(simulation_m100_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/simulation_m100)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/simulation_m100
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(simulation_m100_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(simulation_m100_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/simulation_m100)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/simulation_m100
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(simulation_m100_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(simulation_m100_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/simulation_m100)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/simulation_m100\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/simulation_m100
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(simulation_m100_generate_messages_py geometry_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(simulation_m100_generate_messages_py std_msgs_generate_messages_py)
endif()
