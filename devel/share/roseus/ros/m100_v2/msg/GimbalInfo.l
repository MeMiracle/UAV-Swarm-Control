;; Auto-generated. Do not edit!


(when (boundp 'm100_v2::GimbalInfo)
  (if (not (find-package "M100_V2"))
    (make-package "M100_V2"))
  (shadow 'GimbalInfo (find-package "M100_V2")))
(unless (find-package "M100_V2::GIMBALINFO")
  (make-package "M100_V2::GIMBALINFO"))

(in-package "ROS")
;;//! \htmlinclude GimbalInfo.msg.html


(defclass m100_v2::GimbalInfo
  :super ros::object
  :slots (_target_x _target_y _gimbal_x _gimbal_y ))

(defmethod m100_v2::GimbalInfo
  (:init
   (&key
    ((:target_x __target_x) 0.0)
    ((:target_y __target_y) 0.0)
    ((:gimbal_x __gimbal_x) 0.0)
    ((:gimbal_y __gimbal_y) 0.0)
    )
   (send-super :init)
   (setq _target_x (float __target_x))
   (setq _target_y (float __target_y))
   (setq _gimbal_x (float __gimbal_x))
   (setq _gimbal_y (float __gimbal_y))
   self)
  (:target_x
   (&optional __target_x)
   (if __target_x (setq _target_x __target_x)) _target_x)
  (:target_y
   (&optional __target_y)
   (if __target_y (setq _target_y __target_y)) _target_y)
  (:gimbal_x
   (&optional __gimbal_x)
   (if __gimbal_x (setq _gimbal_x __gimbal_x)) _gimbal_x)
  (:gimbal_y
   (&optional __gimbal_y)
   (if __gimbal_y (setq _gimbal_y __gimbal_y)) _gimbal_y)
  (:serialization-length
   ()
   (+
    ;; float64 _target_x
    8
    ;; float64 _target_y
    8
    ;; float64 _gimbal_x
    8
    ;; float64 _gimbal_y
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _target_x
       (sys::poke _target_x (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _target_y
       (sys::poke _target_y (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _gimbal_x
       (sys::poke _gimbal_x (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _gimbal_y
       (sys::poke _gimbal_y (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _target_x
     (setq _target_x (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _target_y
     (setq _target_y (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _gimbal_x
     (setq _gimbal_x (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _gimbal_y
     (setq _gimbal_y (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get m100_v2::GimbalInfo :md5sum-) "622571951a464a0a539fdb797d0d7edd")
(setf (get m100_v2::GimbalInfo :datatype-) "m100_v2/GimbalInfo")
(setf (get m100_v2::GimbalInfo :definition-)
      "float64 target_x
float64 target_y
float64 gimbal_x
float64 gimbal_y
")



(provide :m100_v2/GimbalInfo "622571951a464a0a539fdb797d0d7edd")


