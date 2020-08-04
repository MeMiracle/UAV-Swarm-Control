; Auto-generated. Do not edit!


(cl:in-package simulation_m100-msg)


;//! \htmlinclude UavBaseInfo.msg.html

(cl:defclass <UavBaseInfo> (roslisp-msg-protocol:ros-message)
  ((cluster_id
    :reader cluster_id
    :initarg :cluster_id
    :type cl:integer
    :initform 0))
)

(cl:defclass UavBaseInfo (<UavBaseInfo>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <UavBaseInfo>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'UavBaseInfo)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name simulation_m100-msg:<UavBaseInfo> is deprecated: use simulation_m100-msg:UavBaseInfo instead.")))

(cl:ensure-generic-function 'cluster_id-val :lambda-list '(m))
(cl:defmethod cluster_id-val ((m <UavBaseInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader simulation_m100-msg:cluster_id-val is deprecated.  Use simulation_m100-msg:cluster_id instead.")
  (cluster_id m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <UavBaseInfo>) ostream)
  "Serializes a message object of type '<UavBaseInfo>"
  (cl:let* ((signed (cl:slot-value msg 'cluster_id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <UavBaseInfo>) istream)
  "Deserializes a message object of type '<UavBaseInfo>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'cluster_id) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<UavBaseInfo>)))
  "Returns string type for a message object of type '<UavBaseInfo>"
  "simulation_m100/UavBaseInfo")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'UavBaseInfo)))
  "Returns string type for a message object of type 'UavBaseInfo"
  "simulation_m100/UavBaseInfo")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<UavBaseInfo>)))
  "Returns md5sum for a message object of type '<UavBaseInfo>"
  "e7dc3c0adeebdd7978013511967e61fa")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'UavBaseInfo)))
  "Returns md5sum for a message object of type 'UavBaseInfo"
  "e7dc3c0adeebdd7978013511967e61fa")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<UavBaseInfo>)))
  "Returns full string definition for message of type '<UavBaseInfo>"
  (cl:format cl:nil "int32 cluster_id~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'UavBaseInfo)))
  "Returns full string definition for message of type 'UavBaseInfo"
  (cl:format cl:nil "int32 cluster_id~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <UavBaseInfo>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <UavBaseInfo>))
  "Converts a ROS message object to a list"
  (cl:list 'UavBaseInfo
    (cl:cons ':cluster_id (cluster_id msg))
))
