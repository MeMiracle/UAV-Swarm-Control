;; Auto-generated. Do not edit!


(when (boundp 'simulation_m100::UavBaseInfo)
  (if (not (find-package "SIMULATION_M100"))
    (make-package "SIMULATION_M100"))
  (shadow 'UavBaseInfo (find-package "SIMULATION_M100")))
(unless (find-package "SIMULATION_M100::UAVBASEINFO")
  (make-package "SIMULATION_M100::UAVBASEINFO"))

(in-package "ROS")
;;//! \htmlinclude UavBaseInfo.msg.html


(defclass simulation_m100::UavBaseInfo
  :super ros::object
  :slots (_cluster_id ))

(defmethod simulation_m100::UavBaseInfo
  (:init
   (&key
    ((:cluster_id __cluster_id) 0)
    )
   (send-super :init)
   (setq _cluster_id (round __cluster_id))
   self)
  (:cluster_id
   (&optional __cluster_id)
   (if __cluster_id (setq _cluster_id __cluster_id)) _cluster_id)
  (:serialization-length
   ()
   (+
    ;; int32 _cluster_id
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _cluster_id
       (write-long _cluster_id s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _cluster_id
     (setq _cluster_id (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get simulation_m100::UavBaseInfo :md5sum-) "e7dc3c0adeebdd7978013511967e61fa")
(setf (get simulation_m100::UavBaseInfo :datatype-) "simulation_m100/UavBaseInfo")
(setf (get simulation_m100::UavBaseInfo :definition-)
      "int32 cluster_id
")



(provide :simulation_m100/UavBaseInfo "e7dc3c0adeebdd7978013511967e61fa")


