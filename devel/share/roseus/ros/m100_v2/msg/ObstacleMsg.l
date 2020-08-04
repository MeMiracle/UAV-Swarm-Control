;; Auto-generated. Do not edit!


(when (boundp 'm100_v2::ObstacleMsg)
  (if (not (find-package "M100_V2"))
    (make-package "M100_V2"))
  (shadow 'ObstacleMsg (find-package "M100_V2")))
(unless (find-package "M100_V2::OBSTACLEMSG")
  (make-package "M100_V2::OBSTACLEMSG"))

(in-package "ROS")
;;//! \htmlinclude ObstacleMsg.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass m100_v2::ObstacleMsg
  :super ros::object
  :slots (_header _center _point1 _point2 _minRadius ))

(defmethod m100_v2::ObstacleMsg
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:center __center) (let (r) (dotimes (i 0) (push (instance geometry_msgs::Point32 :init) r)) r))
    ((:point1 __point1) (let (r) (dotimes (i 0) (push (instance geometry_msgs::Point32 :init) r)) r))
    ((:point2 __point2) (let (r) (dotimes (i 0) (push (instance geometry_msgs::Point32 :init) r)) r))
    ((:minRadius __minRadius) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _header __header)
   (setq _center __center)
   (setq _point1 __point1)
   (setq _point2 __point2)
   (setq _minRadius __minRadius)
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
  (:point1
   (&rest __point1)
   (if (keywordp (car __point1))
       (send* _point1 __point1)
     (progn
       (if __point1 (setq _point1 (car __point1)))
       _point1)))
  (:point2
   (&rest __point2)
   (if (keywordp (car __point2))
       (send* _point2 __point2)
     (progn
       (if __point2 (setq _point2 (car __point2)))
       _point2)))
  (:minRadius
   (&optional __minRadius)
   (if __minRadius (setq _minRadius __minRadius)) _minRadius)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; geometry_msgs/Point32[] _center
    (apply #'+ (send-all _center :serialization-length)) 4
    ;; geometry_msgs/Point32[] _point1
    (apply #'+ (send-all _point1 :serialization-length)) 4
    ;; geometry_msgs/Point32[] _point2
    (apply #'+ (send-all _point2 :serialization-length)) 4
    ;; float32[] _minRadius
    (* 4    (length _minRadius)) 4
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
     ;; geometry_msgs/Point32[] _point1
     (write-long (length _point1) s)
     (dolist (elem _point1)
       (send elem :serialize s)
       )
     ;; geometry_msgs/Point32[] _point2
     (write-long (length _point2) s)
     (dolist (elem _point2)
       (send elem :serialize s)
       )
     ;; float32[] _minRadius
     (write-long (length _minRadius) s)
     (dotimes (i (length _minRadius))
       (sys::poke (elt _minRadius i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
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
   ;; geometry_msgs/Point32[] _point1
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _point1 (let (r) (dotimes (i n) (push (instance geometry_msgs::Point32 :init) r)) r))
     (dolist (elem- _point1)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; geometry_msgs/Point32[] _point2
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _point2 (let (r) (dotimes (i n) (push (instance geometry_msgs::Point32 :init) r)) r))
     (dolist (elem- _point2)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; float32[] _minRadius
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _minRadius (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _minRadius i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;;
   self)
  )

(setf (get m100_v2::ObstacleMsg :md5sum-) "1f9c26cc298db55081856b7b6f06a61c")
(setf (get m100_v2::ObstacleMsg :datatype-) "m100_v2/ObstacleMsg")
(setf (get m100_v2::ObstacleMsg :definition-)
      "Header header
geometry_msgs/Point32[] center
geometry_msgs/Point32[] point1
geometry_msgs/Point32[] point2
float32[] minRadius

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



(provide :m100_v2/ObstacleMsg "1f9c26cc298db55081856b7b6f06a61c")


