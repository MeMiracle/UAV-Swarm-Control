execute_process(COMMAND "/home/uav/ROS/m100/build/dji_sdk/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/uav/ROS/m100/build/dji_sdk/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
