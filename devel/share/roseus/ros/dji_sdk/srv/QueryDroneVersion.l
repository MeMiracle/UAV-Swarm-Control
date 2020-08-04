;; Auto-generated. Do not edit!


(when (boundp 'dji_sdk::QueryDroneVersion)
  (if (not (find-package "DJI_SDK"))
    (make-package "DJI_SDK"))
  (shadow 'QueryDroneVersion (find-package "DJI_SDK")))
(unless (find-package "DJI_SDK::QUERYDRONEVERSION")
  (make-package "DJI_SDK::QUERYDRONEVERSION"))
(unless (find-package "DJI_SDK::QUERYDRONEVERSIONREQUEST")
  (make-package "DJI_SDK::QUERYDRONEVERSIONREQUEST"))
(unless (find-package "DJI_SDK::QUERYDRONEVERSIONRESPONSE")
  (make-package "DJI_SDK::QUERYDRONEVERSIONRESPONSE"))

(in-package "ROS")





(defclass dji_sdk::QueryDroneVersionRequest
  :super ros::object
  :slots ())

(defmethod dji_sdk::QueryDroneVersionRequest
  (:init
   (&key
    )
   (send-super :init)
   self)
  (:serialization-length
   ()
   (+
    0
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;;
   self)
  )

(defclass dji_sdk::QueryDroneVersionResponse
  :super ros::object
  :slots (_version ))

(defmethod dji_sdk::QueryDroneVersionResponse
  (:init
   (&key
    ((:version __version) 0)
    )
   (send-super :init)
   (setq _version (round __version))
   self)
  (:version
   (&optional __version)
   (if __version (setq _version __version)) _version)
  (:serialization-length
   ()
   (+
    ;; uint32 _version
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32 _version
       (write-long _version s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32 _version
     (setq _version (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(defclass dji_sdk::QueryDroneVersion
  :super ros::object
  :slots ())

(setf (get dji_sdk::QueryDroneVersion :md5sum-) "b6f0d7266b39df9c66735f73837244fd")
(setf (get dji_sdk::QueryDroneVersion :datatype-) "dji_sdk/QueryDroneVersion")
(setf (get dji_sdk::QueryDroneVersion :request) dji_sdk::QueryDroneVersionRequest)
(setf (get dji_sdk::QueryDroneVersion :response) dji_sdk::QueryDroneVersionResponse)

(defmethod dji_sdk::QueryDroneVersionRequest
  (:response () (instance dji_sdk::QueryDroneVersionResponse :init)))

(setf (get dji_sdk::QueryDroneVersionRequest :md5sum-) "b6f0d7266b39df9c66735f73837244fd")
(setf (get dji_sdk::QueryDroneVersionRequest :datatype-) "dji_sdk/QueryDroneVersionRequest")
(setf (get dji_sdk::QueryDroneVersionRequest :definition-)
      "---
uint32 version
")

(setf (get dji_sdk::QueryDroneVersionResponse :md5sum-) "b6f0d7266b39df9c66735f73837244fd")
(setf (get dji_sdk::QueryDroneVersionResponse :datatype-) "dji_sdk/QueryDroneVersionResponse")
(setf (get dji_sdk::QueryDroneVersionResponse :definition-)
      "---
uint32 version
")



(provide :dji_sdk/QueryDroneVersion "b6f0d7266b39df9c66735f73837244fd")


