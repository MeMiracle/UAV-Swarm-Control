
(cl:in-package :asdf)

(defsystem "m100_v2-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "CmdResult" :depends-on ("_package_CmdResult"))
    (:file "_package_CmdResult" :depends-on ("_package"))
    (:file "CommInfo" :depends-on ("_package_CommInfo"))
    (:file "_package_CommInfo" :depends-on ("_package"))
    (:file "GimbalInfo" :depends-on ("_package_GimbalInfo"))
    (:file "_package_GimbalInfo" :depends-on ("_package"))
    (:file "ObstacleMsg" :depends-on ("_package_ObstacleMsg"))
    (:file "_package_ObstacleMsg" :depends-on ("_package"))
    (:file "Position" :depends-on ("_package_Position"))
    (:file "_package_Position" :depends-on ("_package"))
    (:file "PubSubUavMsg" :depends-on ("_package_PubSubUavMsg"))
    (:file "_package_PubSubUavMsg" :depends-on ("_package"))
    (:file "UavBaseInfo" :depends-on ("_package_UavBaseInfo"))
    (:file "_package_UavBaseInfo" :depends-on ("_package"))
  ))