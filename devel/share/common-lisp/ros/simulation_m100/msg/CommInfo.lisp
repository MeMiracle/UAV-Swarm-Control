; Auto-generated. Do not edit!


(cl:in-package simulation_m100-msg)


;//! \htmlinclude CommInfo.msg.html

(cl:defclass <CommInfo> (roslisp-msg-protocol:ros-message)
  ((frame_id
    :reader frame_id
    :initarg :frame_id
    :type cl:integer
    :initform 0)
   (src_address
    :reader src_address
    :initarg :src_address
    :type cl:string
    :initform "")
   (comm_bytes
    :reader comm_bytes
    :initarg :comm_bytes
    :type cl:integer
    :initform 0))
)

(cl:defclass CommInfo (<CommInfo>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <CommInfo>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'CommInfo)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name simulation_m100-msg:<CommInfo> is deprecated: use simulation_m100-msg:CommInfo instead.")))

(cl:ensure-generic-function 'frame_id-val :lambda-list '(m))
(cl:defmethod frame_id-val ((m <CommInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader simulation_m100-msg:frame_id-val is deprecated.  Use simulation_m100-msg:frame_id instead.")
  (frame_id m))

(cl:ensure-generic-function 'src_address-val :lambda-list '(m))
(cl:defmethod src_address-val ((m <CommInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader simulation_m100-msg:src_address-val is deprecated.  Use simulation_m100-msg:src_address instead.")
  (src_address m))

(cl:ensure-generic-function 'comm_bytes-val :lambda-list '(m))
(cl:defmethod comm_bytes-val ((m <CommInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader simulation_m100-msg:comm_bytes-val is deprecated.  Use simulation_m100-msg:comm_bytes instead.")
  (comm_bytes m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <CommInfo>) ostream)
  "Serializes a message object of type '<CommInfo>"
  (cl:let* ((signed (cl:slot-value msg 'frame_id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'src_address))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'src_address))
  (cl:let* ((signed (cl:slot-value msg 'comm_bytes)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <CommInfo>) istream)
  "Deserializes a message object of type '<CommInfo>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'frame_id) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'src_address) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'src_address) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'comm_bytes) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<CommInfo>)))
  "Returns string type for a message object of type '<CommInfo>"
  "simulation_m100/CommInfo")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'CommInfo)))
  "Returns string type for a message object of type 'CommInfo"
  "simulation_m100/CommInfo")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<CommInfo>)))
  "Returns md5sum for a message object of type '<CommInfo>"
  "2925ddf0c2ed340db59c3f89c9d288a7")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'CommInfo)))
  "Returns md5sum for a message object of type 'CommInfo"
  "2925ddf0c2ed340db59c3f89c9d288a7")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<CommInfo>)))
  "Returns full string definition for message of type '<CommInfo>"
  (cl:format cl:nil "int32 frame_id~%string src_address~%int32 comm_bytes~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'CommInfo)))
  "Returns full string definition for message of type 'CommInfo"
  (cl:format cl:nil "int32 frame_id~%string src_address~%int32 comm_bytes~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <CommInfo>))
  (cl:+ 0
     4
     4 (cl:length (cl:slot-value msg 'src_address))
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <CommInfo>))
  "Converts a ROS message object to a list"
  (cl:list 'CommInfo
    (cl:cons ':frame_id (frame_id msg))
    (cl:cons ':src_address (src_address msg))
    (cl:cons ':comm_bytes (comm_bytes msg))
))
