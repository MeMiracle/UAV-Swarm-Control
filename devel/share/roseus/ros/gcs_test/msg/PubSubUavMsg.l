;; Auto-generated. Do not edit!


(when (boundp 'gcs_test::PubSubUavMsg)
  (if (not (find-package "GCS_TEST"))
    (make-package "GCS_TEST"))
  (shadow 'PubSubUavMsg (find-package "GCS_TEST")))
(unless (find-package "GCS_TEST::PUBSUBUAVMSG")
  (make-package "GCS_TEST::PUBSUBUAVMSG"))

(in-package "ROS")
;;//! \htmlinclude PubSubUavMsg.msg.html


(defclass gcs_test::PubSubUavMsg
  :super ros::object
  :slots (_src_uav_id _component _cmd_priority _playload _start_time _tgt_uav_count ))

(defmethod gcs_test::PubSubUavMsg
  (:init
   (&key
    ((:src_uav_id __src_uav_id) 0)
    ((:component __component) "")
    ((:cmd_priority __cmd_priority) 0)
    ((:playload __playload) "")
    ((:start_time __start_time) 0.0)
    ((:tgt_uav_count __tgt_uav_count) 0)
    )
   (send-super :init)
   (setq _src_uav_id (round __src_uav_id))
   (setq _component (string __component))
   (setq _cmd_priority (round __cmd_priority))
   (setq _playload (string __playload))
   (setq _start_time (float __start_time))
   (setq _tgt_uav_count (round __tgt_uav_count))
   self)
  (:src_uav_id
   (&optional __src_uav_id)
   (if __src_uav_id (setq _src_uav_id __src_uav_id)) _src_uav_id)
  (:component
   (&optional __component)
   (if __component (setq _component __component)) _component)
  (:cmd_priority
   (&optional __cmd_priority)
   (if __cmd_priority (setq _cmd_priority __cmd_priority)) _cmd_priority)
  (:playload
   (&optional __playload)
   (if __playload (setq _playload __playload)) _playload)
  (:start_time
   (&optional __start_time)
   (if __start_time (setq _start_time __start_time)) _start_time)
  (:tgt_uav_count
   (&optional __tgt_uav_count)
   (if __tgt_uav_count (setq _tgt_uav_count __tgt_uav_count)) _tgt_uav_count)
  (:serialization-length
   ()
   (+
    ;; int32 _src_uav_id
    4
    ;; string _component
    4 (length _component)
    ;; int32 _cmd_priority
    4
    ;; string _playload
    4 (length _playload)
    ;; float64 _start_time
    8
    ;; int32 _tgt_uav_count
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _src_uav_id
       (write-long _src_uav_id s)
     ;; string _component
       (write-long (length _component) s) (princ _component s)
     ;; int32 _cmd_priority
       (write-long _cmd_priority s)
     ;; string _playload
       (write-long (length _playload) s) (princ _playload s)
     ;; float64 _start_time
       (sys::poke _start_time (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; int32 _tgt_uav_count
       (write-long _tgt_uav_count s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _src_uav_id
     (setq _src_uav_id (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; string _component
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _component (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; int32 _cmd_priority
     (setq _cmd_priority (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; string _playload
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _playload (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; float64 _start_time
     (setq _start_time (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; int32 _tgt_uav_count
     (setq _tgt_uav_count (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get gcs_test::PubSubUavMsg :md5sum-) "67891a76d6771782154f9e5d90983fb9")
(setf (get gcs_test::PubSubUavMsg :datatype-) "gcs_test/PubSubUavMsg")
(setf (get gcs_test::PubSubUavMsg :definition-)
      "int32 src_uav_id
string component
int32 cmd_priority
string playload
float64 start_time
int32 tgt_uav_count
")



(provide :gcs_test/PubSubUavMsg "67891a76d6771782154f9e5d90983fb9")


