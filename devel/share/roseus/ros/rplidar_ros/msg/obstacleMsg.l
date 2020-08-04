;; Auto-generated. Do not edit!


(when (boundp 'rplidar_ros::obstacleMsg)
  (if (not (find-package "RPLIDAR_ROS"))
    (make-package "RPLIDAR_ROS"))
  (shadow 'obstacleMsg (find-package "RPLIDAR_ROS")))
(unless (find-package "RPLIDAR_ROS::OBSTACLEMSG")
  (make-package "RPLIDAR_ROS::OBSTACLEMSG"))

(in-package "ROS")
;;//! \htmlinclude obstacleMsg.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass rplidar_ros::obstacleMsg
  :super ros::object
  :slots (_header _center ))

(defmethod rplidar_ros::obstacleMsg
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:center __center) (let (r) (dotimes (i 0) (push (instance geometry_msgs::Point32 :init) r)) r))
    )
   (send-super :init)
   (setq _header __header)
   (setq _center __center)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:center
   (&rest __center)
   (if (keywordp (car __center))
       (send* _center __center)
     (progn
       (if __center (setq _center (car __center)))
       _center)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; geometry_msgs/Point32[] _center
    (apply #'+ (send-all _center :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; geometry_msgs/Point32[] _center
     (write-long (length _center) s)
     (dolist (elem _center)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; geometry_msgs/Point32[] _center
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _center (let (r) (dotimes (i n) (push (instance geometry_msgs::Point32 :init) r)) r))
     (dolist (elem- _center)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get rplidar_ros::obstacleMsg :md5sum-) "b041ba19804c7d506882d542244bd0f6")
(setf (get rplidar_ros::obstacleMsg :datatype-) "rplidar_ros/obstacleMsg")
(setf (get rplidar_ros::obstacleMsg :definition-)
      "Header header
geometry_msgs/Point32[] center

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

================================================================================
MSG: geometry_msgs/Point32
# This contains the position of a point in free space(with 32 bits of precision).
# It is recommeded to use Point wherever possible instead of Point32.  
# 
# This recommendation is to promote interoperability.  
#
# This message is designed to take up less space when sending
# lots of points at once, as in the case of a PointCloud.  

float32 x
float32 y
float32 z
")



(provide :rplidar_ros/obstacleMsg "b041ba19804c7d506882d542244bd0f6")


