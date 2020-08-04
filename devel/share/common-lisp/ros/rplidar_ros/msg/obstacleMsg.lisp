; Auto-generated. Do not edit!


(cl:in-package rplidar_ros-msg)


;//! \htmlinclude obstacleMsg.msg.html

(cl:defclass <obstacleMsg> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (center
    :reader center
    :initarg :center
    :type (cl:vector geometry_msgs-msg:Point32)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:Point32 :initial-element (cl:make-instance 'geometry_msgs-msg:Point32))))
)

(cl:defclass obstacleMsg (<obstacleMsg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <obstacleMsg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'obstacleMsg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name rplidar_ros-msg:<obstacleMsg> is deprecated: use rplidar_ros-msg:obstacleMsg instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <obstacleMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rplidar_ros-msg:header-val is deprecated.  Use rplidar_ros-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'center-val :lambda-list '(m))
(cl:defmethod center-val ((m <obstacleMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader rplidar_ros-msg:center-val is deprecated.  Use rplidar_ros-msg:center instead.")
  (center m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <obstacleMsg>) ostream)
  "Serializes a message object of type '<obstacleMsg>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'center))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'center))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <obstacleMsg>) istream)
  "Deserializes a message object of type '<obstacleMsg>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'center) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'center)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:Point32))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<obstacleMsg>)))
  "Returns string type for a message object of type '<obstacleMsg>"
  "rplidar_ros/obstacleMsg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'obstacleMsg)))
  "Returns string type for a message object of type 'obstacleMsg"
  "rplidar_ros/obstacleMsg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<obstacleMsg>)))
  "Returns md5sum for a message object of type '<obstacleMsg>"
  "b041ba19804c7d506882d542244bd0f6")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'obstacleMsg)))
  "Returns md5sum for a message object of type 'obstacleMsg"
  "b041ba19804c7d506882d542244bd0f6")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<obstacleMsg>)))
  "Returns full string definition for message of type '<obstacleMsg>"
  (cl:format cl:nil "Header header~%geometry_msgs/Point32[] center~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Point32~%# This contains the position of a point in free space(with 32 bits of precision).~%# It is recommeded to use Point wherever possible instead of Point32.  ~%# ~%# This recommendation is to promote interoperability.  ~%#~%# This message is designed to take up less space when sending~%# lots of points at once, as in the case of a PointCloud.  ~%~%float32 x~%float32 y~%float32 z~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'obstacleMsg)))
  "Returns full string definition for message of type 'obstacleMsg"
  (cl:format cl:nil "Header header~%geometry_msgs/Point32[] center~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/Point32~%# This contains the position of a point in free space(with 32 bits of precision).~%# It is recommeded to use Point wherever possible instead of Point32.  ~%# ~%# This recommendation is to promote interoperability.  ~%#~%# This message is designed to take up less space when sending~%# lots of points at once, as in the case of a PointCloud.  ~%~%float32 x~%float32 y~%float32 z~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <obstacleMsg>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'center) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <obstacleMsg>))
  "Converts a ROS message object to a list"
  (cl:list 'obstacleMsg
    (cl:cons ':header (header msg))
    (cl:cons ':center (center msg))
))
