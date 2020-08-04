; Auto-generated. Do not edit!


(cl:in-package m100_v2-msg)


;//! \htmlinclude GimbalInfo.msg.html

(cl:defclass <GimbalInfo> (roslisp-msg-protocol:ros-message)
  ((target_x
    :reader target_x
    :initarg :target_x
    :type cl:float
    :initform 0.0)
   (target_y
    :reader target_y
    :initarg :target_y
    :type cl:float
    :initform 0.0)
   (gimbal_x
    :reader gimbal_x
    :initarg :gimbal_x
    :type cl:float
    :initform 0.0)
   (gimbal_y
    :reader gimbal_y
    :initarg :gimbal_y
    :type cl:float
    :initform 0.0))
)

(cl:defclass GimbalInfo (<GimbalInfo>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <GimbalInfo>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'GimbalInfo)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name m100_v2-msg:<GimbalInfo> is deprecated: use m100_v2-msg:GimbalInfo instead.")))

(cl:ensure-generic-function 'target_x-val :lambda-list '(m))
(cl:defmethod target_x-val ((m <GimbalInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader m100_v2-msg:target_x-val is deprecated.  Use m100_v2-msg:target_x instead.")
  (target_x m))

(cl:ensure-generic-function 'target_y-val :lambda-list '(m))
(cl:defmethod target_y-val ((m <GimbalInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader m100_v2-msg:target_y-val is deprecated.  Use m100_v2-msg:target_y instead.")
  (target_y m))

(cl:ensure-generic-function 'gimbal_x-val :lambda-list '(m))
(cl:defmethod gimbal_x-val ((m <GimbalInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader m100_v2-msg:gimbal_x-val is deprecated.  Use m100_v2-msg:gimbal_x instead.")
  (gimbal_x m))

(cl:ensure-generic-function 'gimbal_y-val :lambda-list '(m))
(cl:defmethod gimbal_y-val ((m <GimbalInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader m100_v2-msg:gimbal_y-val is deprecated.  Use m100_v2-msg:gimbal_y instead.")
  (gimbal_y m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <GimbalInfo>) ostream)
  "Serializes a message object of type '<GimbalInfo>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'target_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'target_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'gimbal_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'gimbal_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <GimbalInfo>) istream)
  "Deserializes a message object of type '<GimbalInfo>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'target_x) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'target_y) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'gimbal_x) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'gimbal_y) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<GimbalInfo>)))
  "Returns string type for a message object of type '<GimbalInfo>"
  "m100_v2/GimbalInfo")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'GimbalInfo)))
  "Returns string type for a message object of type 'GimbalInfo"
  "m100_v2/GimbalInfo")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<GimbalInfo>)))
  "Returns md5sum for a message object of type '<GimbalInfo>"
  "622571951a464a0a539fdb797d0d7edd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'GimbalInfo)))
  "Returns md5sum for a message object of type 'GimbalInfo"
  "622571951a464a0a539fdb797d0d7edd")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<GimbalInfo>)))
  "Returns full string definition for message of type '<GimbalInfo>"
  (cl:format cl:nil "float64 target_x~%float64 target_y~%float64 gimbal_x~%float64 gimbal_y~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'GimbalInfo)))
  "Returns full string definition for message of type 'GimbalInfo"
  (cl:format cl:nil "float64 target_x~%float64 target_y~%float64 gimbal_x~%float64 gimbal_y~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <GimbalInfo>))
  (cl:+ 0
     8
     8
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <GimbalInfo>))
  "Converts a ROS message object to a list"
  (cl:list 'GimbalInfo
    (cl:cons ':target_x (target_x msg))
    (cl:cons ':target_y (target_y msg))
    (cl:cons ':gimbal_x (gimbal_x msg))
    (cl:cons ':gimbal_y (gimbal_y msg))
))
