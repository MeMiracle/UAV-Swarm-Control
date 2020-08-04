;; Auto-generated. Do not edit!


(when (boundp 'm100_v2::EnableMotors)
  (if (not (find-package "M100_V2"))
    (make-package "M100_V2"))
  (shadow 'EnableMotors (find-package "M100_V2")))
(unless (find-package "M100_V2::ENABLEMOTORS")
  (make-package "M100_V2::ENABLEMOTORS"))
(unless (find-package "M100_V2::ENABLEMOTORSREQUEST")
  (make-package "M100_V2::ENABLEMOTORSREQUEST"))
(unless (find-package "M100_V2::ENABLEMOTORSRESPONSE")
  (make-package "M100_V2::ENABLEMOTORSRESPONSE"))

(in-package "ROS")





(defclass m100_v2::EnableMotorsRequest
  :super ros::object
  :slots (_enable ))

(defmethod m100_v2::EnableMotorsRequest
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

(defclass m100_v2::EnableMotorsResponse
  :super ros::object
  :slots (_success ))

(defmethod m100_v2::EnableMotorsResponse
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

(defclass m100_v2::EnableMotors
  :super ros::object
  :slots ())

(setf (get m100_v2::EnableMotors :md5sum-) "6a0b406242562fc416b2c9c8a3efb051")
(setf (get m100_v2::EnableMotors :datatype-) "m100_v2/EnableMotors")
(setf (get m100_v2::EnableMotors :request) m100_v2::EnableMotorsRequest)
(setf (get m100_v2::EnableMotors :response) m100_v2::EnableMotorsResponse)

(defmethod m100_v2::EnableMotorsRequest
  (:response () (instance m100_v2::EnableMotorsResponse :init)))

(setf (get m100_v2::EnableMotorsRequest :md5sum-) "6a0b406242562fc416b2c9c8a3efb051")
(setf (get m100_v2::EnableMotorsRequest :datatype-) "m100_v2/EnableMotorsRequest")
(setf (get m100_v2::EnableMotorsRequest :definition-)
      "bool enable
---
bool success
")

(setf (get m100_v2::EnableMotorsResponse :md5sum-) "6a0b406242562fc416b2c9c8a3efb051")
(setf (get m100_v2::EnableMotorsResponse :datatype-) "m100_v2/EnableMotorsResponse")
(setf (get m100_v2::EnableMotorsResponse :definition-)
      "bool enable
---
bool success
")



(provide :m100_v2/EnableMotors "6a0b406242562fc416b2c9c8a3efb051")


