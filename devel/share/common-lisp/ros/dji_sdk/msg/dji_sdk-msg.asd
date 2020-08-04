
(cl:in-package :asdf)

(defsystem "dji_sdk-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "Gimbal" :depends-on ("_package_Gimbal"))
    (:file "_package_Gimbal" :depends-on ("_package"))
    (:file "MissionHotpointTask" :depends-on ("_package_MissionHotpointTask"))
    (:file "_package_MissionHotpointTask" :depends-on ("_package"))
    (:file "MissionWaypoint" :depends-on ("_package_MissionWaypoint"))
    (:file "_package_MissionWaypoint" :depends-on ("_package"))
    (:file "MissionWaypointAction" :depends-on ("_package_MissionWaypointAction"))
    (:file "_package_MissionWaypointAction" :depends-on ("_package"))
    (:file "MissionWaypointTask" :depends-on ("_package_MissionWaypointTask"))
    (:file "_package_MissionWaypointTask" :depends-on ("_package"))
    (:file "MobileData" :depends-on ("_package_MobileData"))
    (:file "_package_MobileData" :depends-on ("_package"))
    (:file "Waypoint" :depends-on ("_package_Waypoint"))
    (:file "_package_Waypoint" :depends-on ("_package"))
    (:file "WaypointList" :depends-on ("_package_WaypointList"))
    (:file "_package_WaypointList" :depends-on ("_package"))
  ))