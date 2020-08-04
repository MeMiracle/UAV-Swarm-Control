;; Auto-generated. Do not edit!


(when (boundp 'm100_v2::CommInfo)
  (if (not (find-package "M100_V2"))
    (make-package "M100_V2"))
  (shadow 'CommInfo (find-package "M100_V2")))
(unless (find-package "M100_V2::COMMINFO")
  (make-package "M100_V2::COMMINFO"))

(in-package "ROS")
;;//! \htmlinclude CommInfo.msg.html


(defclass m100_v2::CommInfo
  :super ros::object
  :slots (_frame_id _src_address _comm_bytes ))

(defmethod m100_v2::CommInfo
  (:init
   (&key
    ((:frame_id __frame_id) 0)
    ((:src_address __src_address) "")
    ((:comm_bytes __comm_bytes) 0)
    )
   (send-super :init)
   (setq _frame_id (round __frame_id))
   (setq _src_address (string __src_address))
   (setq _comm_bytes (round __comm_bytes))
   self)
  (:frame_id
   (&optional __frame_id)
   (if __frame_id (setq _frame_id __frame_id)) _frame_id)
  (:src_address
   (&optional __src_address)
   (if __src_address (setq _src_address __src_address)) _src_address)
  (:comm_bytes
   (&optional __comm_bytes)
   (if __comm_bytes (setq _comm_bytes __comm_bytes)) _comm_bytes)
  (:serialization-length
   ()
   (+
    ;; int32 _frame_id
    4
    ;; string _src_address
    4 (length _src_address)
    ;; int32 _comm_bytes
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _frame_id
       (write-long _frame_id s)
     ;; string _src_address
       (write-long (length _src_address) s) (princ _src_address s)
     ;; int32 _comm_bytes
       (write-long _comm_bytes s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _frame_id
     (setq _frame_id (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; string _src_address
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _src_address (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; int32 _comm_bytes
     (setq _comm_bytes (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get m100_v2::CommInfo :md5sum-) "2925ddf0c2ed340db59c3f89c9d288a7")
(setf (get m100_v2::CommInfo :datatype-) "m100_v2/CommInfo")
(setf (get m100_v2::CommInfo :definition-)
      "int32 frame_id
string src_address
int32 comm_bytes
")



(provide :m100_v2/CommInfo "2925ddf0c2ed340db59c3f89c9d288a7")


