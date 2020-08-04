; Auto-generated. Do not edit!


(cl:in-package dji_sdk-srv)


;//! \htmlinclude QueryDroneVersion-request.msg.html

(cl:defclass <QueryDroneVersion-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass QueryDroneVersion-request (<QueryDroneVersion-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <QueryDroneVersion-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'QueryDroneVersion-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name dji_sdk-srv:<QueryDroneVersion-request> is deprecated: use dji_sdk-srv:QueryDroneVersion-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <QueryDroneVersion-request>) ostream)
  "Serializes a message object of type '<QueryDroneVersion-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <QueryDroneVersion-request>) istream)
  "Deserializes a message object of type '<QueryDroneVersion-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<QueryDroneVersion-request>)))
  "Returns string type for a service object of type '<QueryDroneVersion-request>"
  "dji_sdk/QueryDroneVersionRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'QueryDroneVersion-request)))
  "Returns string type for a service object of type 'QueryDroneVersion-request"
  "dji_sdk/QueryDroneVersionRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<QueryDroneVersion-request>)))
  "Returns md5sum for a message object of type '<QueryDroneVersion-request>"
  "b6f0d7266b39df9c66735f73837244fd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'QueryDroneVersion-request)))
  "Returns md5sum for a message object of type 'QueryDroneVersion-request"
  "b6f0d7266b39df9c66735f73837244fd")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<QueryDroneVersion-request>)))
  "Returns full string definition for message of type '<QueryDroneVersion-request>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'QueryDroneVersion-request)))
  "Returns full string definition for message of type 'QueryDroneVersion-request"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <QueryDroneVersion-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <QueryDroneVersion-request>))
  "Converts a ROS message object to a list"
  (cl:list 'QueryDroneVersion-request
))
;//! \htmlinclude QueryDroneVersion-response.msg.html

(cl:defclass <QueryDroneVersion-response> (roslisp-msg-protocol:ros-message)
  ((version
    :reader version
    :initarg :version
    :type cl:integer
    :initform 0))
)

(cl:defclass QueryDroneVersion-response (<QueryDroneVersion-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <QueryDroneVersion-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'QueryDroneVersion-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name dji_sdk-srv:<QueryDroneVersion-response> is deprecated: use dji_sdk-srv:QueryDroneVersion-response instead.")))

(cl:ensure-generic-function 'version-val :lambda-list '(m))
(cl:defmethod version-val ((m <QueryDroneVersion-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader dji_sdk-srv:version-val is deprecated.  Use dji_sdk-srv:version instead.")
  (version m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <QueryDroneVersion-response>) ostream)
  "Serializes a message object of type '<QueryDroneVersion-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'version)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'version)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'version)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'version)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <QueryDroneVersion-response>) istream)
  "Deserializes a message object of type '<QueryDroneVersion-response>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'version)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'version)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'version)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'version)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<QueryDroneVersion-response>)))
  "Returns string type for a service object of type '<QueryDroneVersion-response>"
  "dji_sdk/QueryDroneVersionResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'QueryDroneVersion-response)))
  "Returns string type for a service object of type 'QueryDroneVersion-response"
  "dji_sdk/QueryDroneVersionResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<QueryDroneVersion-response>)))
  "Returns md5sum for a message object of type '<QueryDroneVersion-response>"
  "b6f0d7266b39df9c66735f73837244fd")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'QueryDroneVersion-response)))
  "Returns md5sum for a message object of type 'QueryDroneVersion-response"
  "b6f0d7266b39df9c66735f73837244fd")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<QueryDroneVersion-response>)))
  "Returns full string definition for message of type '<QueryDroneVersion-response>"
  (cl:format cl:nil "uint32 version~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'QueryDroneVersion-response)))
  "Returns full string definition for message of type 'QueryDroneVersion-response"
  (cl:format cl:nil "uint32 version~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <QueryDroneVersion-response>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <QueryDroneVersion-response>))
  "Converts a ROS message object to a list"
  (cl:list 'QueryDroneVersion-response
    (cl:cons ':version (version msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'QueryDroneVersion)))
  'QueryDroneVersion-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'QueryDroneVersion)))
  'QueryDroneVersion-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'QueryDroneVersion)))
  "Returns string type for a service object of type '<QueryDroneVersion>"
  "dji_sdk/QueryDroneVersion")