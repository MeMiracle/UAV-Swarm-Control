
(cl:in-package :asdf)

(defsystem "rplidar_ros-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "obstacleMsg" :depends-on ("_package_obstacleMsg"))
    (:file "_package_obstacleMsg" :depends-on ("_package"))
  ))