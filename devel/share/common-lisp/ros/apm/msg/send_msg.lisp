; Auto-generated. Do not edit!


(cl:in-package apm-msg)


;//! \htmlinclude send_msg.msg.html

(cl:defclass <send_msg> (roslisp-msg-protocol:ros-message)
  ((send_msg
    :reader send_msg
    :initarg :send_msg
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 259 :element-type 'cl:fixnum :initial-element 0)))
)

(cl:defclass send_msg (<send_msg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <send_msg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'send_msg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name apm-msg:<send_msg> is deprecated: use apm-msg:send_msg instead.")))

(cl:ensure-generic-function 'send_msg-val :lambda-list '(m))
(cl:defmethod send_msg-val ((m <send_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader apm-msg:send_msg-val is deprecated.  Use apm-msg:send_msg instead.")
  (send_msg m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <send_msg>) ostream)
  "Serializes a message object of type '<send_msg>"
  (cl:map cl:nil #'(cl:lambda (ele) (cl:write-byte (cl:ldb (cl:byte 8 0) ele) ostream))
   (cl:slot-value msg 'send_msg))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <send_msg>) istream)
  "Deserializes a message object of type '<send_msg>"
  (cl:setf (cl:slot-value msg 'send_msg) (cl:make-array 259))
  (cl:let ((vals (cl:slot-value msg 'send_msg)))
    (cl:dotimes (i 259)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:aref vals i)) (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<send_msg>)))
  "Returns string type for a message object of type '<send_msg>"
  "apm/send_msg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'send_msg)))
  "Returns string type for a message object of type 'send_msg"
  "apm/send_msg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<send_msg>)))
  "Returns md5sum for a message object of type '<send_msg>"
  "c80dcf834ebc4faedd2627bb33ad2ae3")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'send_msg)))
  "Returns md5sum for a message object of type 'send_msg"
  "c80dcf834ebc4faedd2627bb33ad2ae3")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<send_msg>)))
  "Returns full string definition for message of type '<send_msg>"
  (cl:format cl:nil "uint8[259] send_msg~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'send_msg)))
  "Returns full string definition for message of type 'send_msg"
  (cl:format cl:nil "uint8[259] send_msg~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <send_msg>))
  (cl:+ 0
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'send_msg) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <send_msg>))
  "Converts a ROS message object to a list"
  (cl:list 'send_msg
    (cl:cons ':send_msg (send_msg msg))
))
