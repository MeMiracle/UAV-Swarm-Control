; Auto-generated. Do not edit!


(cl:in-package gcs_test-msg)


;//! \htmlinclude PubSubUavMsg.msg.html

(cl:defclass <PubSubUavMsg> (roslisp-msg-protocol:ros-message)
  ((src_uav_id
    :reader src_uav_id
    :initarg :src_uav_id
    :type cl:integer
    :initform 0)
   (component
    :reader component
    :initarg :component
    :type cl:string
    :initform "")
   (cmd_priority
    :reader cmd_priority
    :initarg :cmd_priority
    :type cl:integer
    :initform 0)
   (playload
    :reader playload
    :initarg :playload
    :type cl:string
    :initform "")
   (start_time
    :reader start_time
    :initarg :start_time
    :type cl:float
    :initform 0.0)
   (tgt_uav_count
    :reader tgt_uav_count
    :initarg :tgt_uav_count
    :type cl:integer
    :initform 0))
)

(cl:defclass PubSubUavMsg (<PubSubUavMsg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PubSubUavMsg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PubSubUavMsg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name gcs_test-msg:<PubSubUavMsg> is deprecated: use gcs_test-msg:PubSubUavMsg instead.")))

(cl:ensure-generic-function 'src_uav_id-val :lambda-list '(m))
(cl:defmethod src_uav_id-val ((m <PubSubUavMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader gcs_test-msg:src_uav_id-val is deprecated.  Use gcs_test-msg:src_uav_id instead.")
  (src_uav_id m))

(cl:ensure-generic-function 'component-val :lambda-list '(m))
(cl:defmethod component-val ((m <PubSubUavMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader gcs_test-msg:component-val is deprecated.  Use gcs_test-msg:component instead.")
  (component m))

(cl:ensure-generic-function 'cmd_priority-val :lambda-list '(m))
(cl:defmethod cmd_priority-val ((m <PubSubUavMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader gcs_test-msg:cmd_priority-val is deprecated.  Use gcs_test-msg:cmd_priority instead.")
  (cmd_priority m))

(cl:ensure-generic-function 'playload-val :lambda-list '(m))
(cl:defmethod playload-val ((m <PubSubUavMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader gcs_test-msg:playload-val is deprecated.  Use gcs_test-msg:playload instead.")
  (playload m))

(cl:ensure-generic-function 'start_time-val :lambda-list '(m))
(cl:defmethod start_time-val ((m <PubSubUavMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader gcs_test-msg:start_time-val is deprecated.  Use gcs_test-msg:start_time instead.")
  (start_time m))

(cl:ensure-generic-function 'tgt_uav_count-val :lambda-list '(m))
(cl:defmethod tgt_uav_count-val ((m <PubSubUavMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader gcs_test-msg:tgt_uav_count-val is deprecated.  Use gcs_test-msg:tgt_uav_count instead.")
  (tgt_uav_count m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PubSubUavMsg>) ostream)
  "Serializes a message object of type '<PubSubUavMsg>"
  (cl:let* ((signed (cl:slot-value msg 'src_uav_id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'component))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'component))
  (cl:let* ((signed (cl:slot-value msg 'cmd_priority)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'playload))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'playload))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'start_time))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let* ((signed (cl:slot-value msg 'tgt_uav_count)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PubSubUavMsg>) istream)
  "Deserializes a message object of type '<PubSubUavMsg>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'src_uav_id) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'component) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'component) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'cmd_priority) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'playload) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'playload) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'start_time) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'tgt_uav_count) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PubSubUavMsg>)))
  "Returns string type for a message object of type '<PubSubUavMsg>"
  "gcs_test/PubSubUavMsg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PubSubUavMsg)))
  "Returns string type for a message object of type 'PubSubUavMsg"
  "gcs_test/PubSubUavMsg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PubSubUavMsg>)))
  "Returns md5sum for a message object of type '<PubSubUavMsg>"
  "67891a76d6771782154f9e5d90983fb9")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PubSubUavMsg)))
  "Returns md5sum for a message object of type 'PubSubUavMsg"
  "67891a76d6771782154f9e5d90983fb9")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PubSubUavMsg>)))
  "Returns full string definition for message of type '<PubSubUavMsg>"
  (cl:format cl:nil "int32 src_uav_id~%string component~%int32 cmd_priority~%string playload~%float64 start_time~%int32 tgt_uav_count~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PubSubUavMsg)))
  "Returns full string definition for message of type 'PubSubUavMsg"
  (cl:format cl:nil "int32 src_uav_id~%string component~%int32 cmd_priority~%string playload~%float64 start_time~%int32 tgt_uav_count~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PubSubUavMsg>))
  (cl:+ 0
     4
     4 (cl:length (cl:slot-value msg 'component))
     4
     4 (cl:length (cl:slot-value msg 'playload))
     8
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PubSubUavMsg>))
  "Converts a ROS message object to a list"
  (cl:list 'PubSubUavMsg
    (cl:cons ':src_uav_id (src_uav_id msg))
    (cl:cons ':component (component msg))
    (cl:cons ':cmd_priority (cmd_priority msg))
    (cl:cons ':playload (playload msg))
    (cl:cons ':start_time (start_time msg))
    (cl:cons ':tgt_uav_count (tgt_uav_count msg))
))
