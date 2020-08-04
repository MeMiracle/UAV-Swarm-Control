; Auto-generated. Do not edit!


(cl:in-package gcs-msg)


;//! \htmlinclude recv_msg.msg.html

(cl:defclass <recv_msg> (roslisp-msg-protocol:ros-message)
  ((recv_msg
    :reader recv_msg
    :initarg :recv_msg
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 263 :element-type 'cl:fixnum :initial-element 0)))
)

(cl:defclass recv_msg (<recv_msg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <recv_msg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'recv_msg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name gcs-msg:<recv_msg> is deprecated: use gcs-msg:recv_msg instead.")))

(cl:ensure-generic-function 'recv_msg-val :lambda-list '(m))
(cl:defmethod recv_msg-val ((m <recv_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader gcs-msg:recv_msg-val is deprecated.  Use gcs-msg:recv_msg instead.")
  (recv_msg m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <recv_msg>) ostream)
  "Serializes a message object of type '<recv_msg>"
  (cl:map cl:nil #'(cl:lambda (ele) (cl:write-byte (cl:ldb (cl:byte 8 0) ele) ostream))
   (cl:slot-value msg 'recv_msg))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <recv_msg>) istream)
  "Deserializes a message object of type '<recv_msg>"
  (cl:setf (cl:slot-value msg 'recv_msg) (cl:make-array 263))
  (cl:let ((vals (cl:slot-value msg 'recv_msg)))
    (cl:dotimes (i 263)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:aref vals i)) (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<recv_msg>)))
  "Returns string type for a message object of type '<recv_msg>"
  "gcs/recv_msg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'recv_msg)))
  "Returns string type for a message object of type 'recv_msg"
  "gcs/recv_msg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<recv_msg>)))
  "Returns md5sum for a message object of type '<recv_msg>"
  "dc444b49bfa57eb79eb16e3052e264e0")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'recv_msg)))
  "Returns md5sum for a message object of type 'recv_msg"
  "dc444b49bfa57eb79eb16e3052e264e0")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<recv_msg>)))
  "Returns full string definition for message of type '<recv_msg>"
  (cl:format cl:nil "uint8[263] recv_msg~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'recv_msg)))
  "Returns full string definition for message of type 'recv_msg"
  (cl:format cl:nil "uint8[263] recv_msg~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <recv_msg>))
  (cl:+ 0
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'recv_msg) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <recv_msg>))
  "Converts a ROS message object to a list"
  (cl:list 'recv_msg
    (cl:cons ':recv_msg (recv_msg msg))
))
