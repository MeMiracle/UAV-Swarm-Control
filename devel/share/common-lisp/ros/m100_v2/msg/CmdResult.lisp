; Auto-generated. Do not edit!


(cl:in-package m100_v2-msg)


;//! \htmlinclude CmdResult.msg.html

(cl:defclass <CmdResult> (roslisp-msg-protocol:ros-message)
  ((component
    :reader component
    :initarg :component
    :type cl:string
    :initform "")
   (command
    :reader command
    :initarg :command
    :type cl:string
    :initform "")
   (status
    :reader status
    :initarg :status
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass CmdResult (<CmdResult>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <CmdResult>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'CmdResult)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name m100_v2-msg:<CmdResult> is deprecated: use m100_v2-msg:CmdResult instead.")))

(cl:ensure-generic-function 'component-val :lambda-list '(m))
(cl:defmethod component-val ((m <CmdResult>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader m100_v2-msg:component-val is deprecated.  Use m100_v2-msg:component instead.")
  (component m))

(cl:ensure-generic-function 'command-val :lambda-list '(m))
(cl:defmethod command-val ((m <CmdResult>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader m100_v2-msg:command-val is deprecated.  Use m100_v2-msg:command instead.")
  (command m))

(cl:ensure-generic-function 'status-val :lambda-list '(m))
(cl:defmethod status-val ((m <CmdResult>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader m100_v2-msg:status-val is deprecated.  Use m100_v2-msg:status instead.")
  (status m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <CmdResult>) ostream)
  "Serializes a message object of type '<CmdResult>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'component))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'component))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'command))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'command))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'status) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <CmdResult>) istream)
  "Deserializes a message object of type '<CmdResult>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'component) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'component) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'command) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'command) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:setf (cl:slot-value msg 'status) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<CmdResult>)))
  "Returns string type for a message object of type '<CmdResult>"
  "m100_v2/CmdResult")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'CmdResult)))
  "Returns string type for a message object of type 'CmdResult"
  "m100_v2/CmdResult")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<CmdResult>)))
  "Returns md5sum for a message object of type '<CmdResult>"
  "50fc9051b66559e2902987e4c3f3c68f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'CmdResult)))
  "Returns md5sum for a message object of type 'CmdResult"
  "50fc9051b66559e2902987e4c3f3c68f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<CmdResult>)))
  "Returns full string definition for message of type '<CmdResult>"
  (cl:format cl:nil "string component~%string command~%bool status~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'CmdResult)))
  "Returns full string definition for message of type 'CmdResult"
  (cl:format cl:nil "string component~%string command~%bool status~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <CmdResult>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'component))
     4 (cl:length (cl:slot-value msg 'command))
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <CmdResult>))
  "Converts a ROS message object to a list"
  (cl:list 'CmdResult
    (cl:cons ':component (component msg))
    (cl:cons ':command (command msg))
    (cl:cons ':status (status msg))
))
