# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "wifistatus: 2 messages, 0 services")

set(MSG_I_FLAGS "-Iwifistatus:/home/uav/ROS/m100/src/wifistatus/msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(wifistatus_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/uav/ROS/m100/src/wifistatus/msg/CommInfo.msg" NAME_WE)
add_custom_target(_wifistatus_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "wifistatus" "/home/uav/ROS/m100/src/wifistatus/msg/CommInfo.msg" ""
)

get_filename_component(_filename "/home/uav/ROS/m100/src/wifistatus/msg/Position.msg" NAME_WE)
add_custom_target(_wifistatus_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "wifistatus" "/home/uav/ROS/m100/src/wifistatus/msg/Position.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(wifistatus
  "/home/uav/ROS/m100/src/wifistatus/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/wifistatus
)
_generate_msg_cpp(wifistatus
  "/home/uav/ROS/m100/src/wifistatus/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/wifistatus
)

### Generating Services

### Generating Module File
_generate_module_cpp(wifistatus
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/wifistatus
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(wifistatus_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(wifistatus_generate_messages wifistatus_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/wifistatus/msg/CommInfo.msg" NAME_WE)
add_dependencies(wifistatus_generate_messages_cpp _wifistatus_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/wifistatus/msg/Position.msg" NAME_WE)
add_dependencies(wifistatus_generate_messages_cpp _wifistatus_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(wifistatus_gencpp)
add_dependencies(wifistatus_gencpp wifistatus_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS wifistatus_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(wifistatus
  "/home/uav/ROS/m100/src/wifistatus/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/wifistatus
)
_generate_msg_eus(wifistatus
  "/home/uav/ROS/m100/src/wifistatus/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/wifistatus
)

### Generating Services

### Generating Module File
_generate_module_eus(wifistatus
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/wifistatus
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(wifistatus_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(wifistatus_generate_messages wifistatus_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/wifistatus/msg/CommInfo.msg" NAME_WE)
add_dependencies(wifistatus_generate_messages_eus _wifistatus_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/wifistatus/msg/Position.msg" NAME_WE)
add_dependencies(wifistatus_generate_messages_eus _wifistatus_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(wifistatus_geneus)
add_dependencies(wifistatus_geneus wifistatus_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS wifistatus_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(wifistatus
  "/home/uav/ROS/m100/src/wifistatus/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/wifistatus
)
_generate_msg_lisp(wifistatus
  "/home/uav/ROS/m100/src/wifistatus/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/wifistatus
)

### Generating Services

### Generating Module File
_generate_module_lisp(wifistatus
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/wifistatus
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(wifistatus_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(wifistatus_generate_messages wifistatus_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/wifistatus/msg/CommInfo.msg" NAME_WE)
add_dependencies(wifistatus_generate_messages_lisp _wifistatus_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/wifistatus/msg/Position.msg" NAME_WE)
add_dependencies(wifistatus_generate_messages_lisp _wifistatus_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(wifistatus_genlisp)
add_dependencies(wifistatus_genlisp wifistatus_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS wifistatus_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(wifistatus
  "/home/uav/ROS/m100/src/wifistatus/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/wifistatus
)
_generate_msg_nodejs(wifistatus
  "/home/uav/ROS/m100/src/wifistatus/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/wifistatus
)

### Generating Services

### Generating Module File
_generate_module_nodejs(wifistatus
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/wifistatus
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(wifistatus_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(wifistatus_generate_messages wifistatus_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/wifistatus/msg/CommInfo.msg" NAME_WE)
add_dependencies(wifistatus_generate_messages_nodejs _wifistatus_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/wifistatus/msg/Position.msg" NAME_WE)
add_dependencies(wifistatus_generate_messages_nodejs _wifistatus_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(wifistatus_gennodejs)
add_dependencies(wifistatus_gennodejs wifistatus_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS wifistatus_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(wifistatus
  "/home/uav/ROS/m100/src/wifistatus/msg/CommInfo.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/wifistatus
)
_generate_msg_py(wifistatus
  "/home/uav/ROS/m100/src/wifistatus/msg/Position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/wifistatus
)

### Generating Services

### Generating Module File
_generate_module_py(wifistatus
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/wifistatus
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(wifistatus_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(wifistatus_generate_messages wifistatus_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/uav/ROS/m100/src/wifistatus/msg/CommInfo.msg" NAME_WE)
add_dependencies(wifistatus_generate_messages_py _wifistatus_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/uav/ROS/m100/src/wifistatus/msg/Position.msg" NAME_WE)
add_dependencies(wifistatus_generate_messages_py _wifistatus_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(wifistatus_genpy)
add_dependencies(wifistatus_genpy wifistatus_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS wifistatus_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/wifistatus)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/wifistatus
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(wifistatus_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/wifistatus)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/wifistatus
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(wifistatus_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/wifistatus)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/wifistatus
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(wifistatus_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/wifistatus)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/wifistatus
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(wifistatus_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/wifistatus)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/wifistatus\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/wifistatus
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(wifistatus_generate_messages_py std_msgs_generate_messages_py)
endif()
