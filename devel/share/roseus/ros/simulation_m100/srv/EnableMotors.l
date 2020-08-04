;; Auto-generated. Do not edit!


(when (boundp 'simulation_m100::EnableMotors)
  (if (not (find-package "SIMULATION_M100"))
    (make-package "SIMULATION_M100"))
  (shadow 'EnableMotors (find-package "SIMULATION_M100")))
(unless (find-package "SIMULATION_M100::ENABLEMOTORS")
  (make-package "SIMULATION_M100::ENABLEMOTORS"))
(unless (find-package "SIMULATION_M100::ENABLEMOTORSREQUEST")
  (make-package "SIMULATION_M100::ENABLEMOTORSREQUEST"))
(unless (find-package "SIMULATION_M100::ENABLEMOTORSRESPONSE")
  (make-package "SIMULATION_M100::ENABLEMOTORSRESPONSE"))

(in-package "ROS")





(defclass simulation_m100::EnableMotorsRequest
  :super ros::object
  :slots (_enable ))

(defmethod simulation_m100::EnableMotorsRequest
  (:init
   (&key
    ((:enable __enable) nil)
    )
   (send-super :init)
   (setq _enable __enable)
   self)
  (:enable
   (&optional __enable)
   (if __enable (setq _enable __enable)) _enable)
  (:serialization-length
   ()
   (+
    ;; bool _enable
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _enable
       (if _enable (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _enable
     (setq _enable (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(defclass simulation_m100::EnableMotorsResponse
  :super ros::object
  :slots (_success ))

(defmethod simulation_m100::EnableMotorsResponse
  (:init
   (&key
    ((:success __success) nil)
    )
   (send-super :init)
   (setq _success __success)
   self)
  (:success
   (&optional __success)
   (if __success (setq _success __success)) _success)
  (:serialization-length
   ()
   (+
    ;; bool _success
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _success
       (if _success (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _success
     (setq _success (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(defclass simulation_m100::EnableMotors
  :super ros::object
  :slots ())

(setf (get simulation_m100::EnableMotors :md5sum-) "6a0b406242562fc416b2c9c8a3efb051")
(setf (get simulation_m100::EnableMotors :datatype-) "simulation_m100/EnableMotors")
(setf (get simulation_m100::EnableMotors :request) simulation_m100::EnableMotorsRequest)
(setf (get simulation_m100::EnableMotors :response) simulation_m100::EnableMotorsResponse)

(defmethod simulation_m100::EnableMotorsRequest
  (:response () (instance simulation_m100::EnableMotorsResponse :init)))

(setf (get simulation_m100::EnableMotorsRequest :md5sum-) "6a0b406242562fc416b2c9c8a3efb051")
(setf (get simulation_m100::EnableMotorsRequest :datatype-) "simulation_m100/EnableMotorsRequest")
(setf (get simulation_m100::EnableMotorsRequest :definition-)
      "bool enable
---
bool success
")

(setf (get simulation_m100::EnableMotorsResponse :md5sum-) "6a0b406242562fc416b2c9c8a3efb051")
(setf (get simulation_m100::EnableMotorsResponse :datatype-) "simulation_m100/EnableMotorsResponse")
(setf (get simulation_m100::EnableMotorsResponse :definition-)
      "bool enable
---
bool success
")



(provide :simulation_m100/EnableMotors "6a0b406242562fc416b2c9c8a3efb051")


