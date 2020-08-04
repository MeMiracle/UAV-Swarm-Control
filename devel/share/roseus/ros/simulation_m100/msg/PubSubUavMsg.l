;; Auto-generated. Do not edit!


(when (boundp 'simulation_m100::PubSubUavMsg)
  (if (not (find-package "SIMULATION_M100"))
    (make-package "SIMULATION_M100"))
  (shadow 'PubSubUavMsg (find-package "SIMULATION_M100")))
(unless (find-package "SIMULATION_M100::PUBSUBUAVMSG")
  (make-package "SIMULATION_M100::PUBSUBUAVMSG"))

(in-package "ROS")
;;//! \htmlinclude PubSubUavMsg.msg.html


(defclass simulation_m100::PubSubUavMsg
  :super ros::object
  :slots (_src_uav_id _component _cmd_priority _playload _start_time _seq_num _time_stamp _tgt_uav_count ))

(defmethod simulation_m100::PubSubUavMsg
  (:init
   (&key
    ((:src_uav_id __src_uav_id) 0)
    ((:component __component) "")
    ((:cmd_priority __cmd_priority) 0)
    ((:playload __playload) "")
    ((:start_time __start_time) 0.0)
    ((:seq_num __seq_num) 0)
    ((:time_stamp __time_stamp) 0.0)
    ((:tgt_uav_count __tgt_uav_count) 0)
    )
   (send-super :init)
   (setq _src_uav_id (round __src_uav_id))
   (setq _component (string __component))
   (setq _cmd_priority (round __cmd_priority))
   (setq _playload (string __playload))
   (setq _start_time (float __start_time))
   (setq _seq_num (round __seq_num))
   (setq _time_stamp (float __time_stamp))
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
  (:seq_num
   (&optional __seq_num)
   (if __seq_num (setq _seq_num __seq_num)) _seq_num)
  (:time_stamp
   (&optional __time_stamp)
   (if __time_stamp (setq _time_stamp __time_stamp)) _time_stamp)
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
    ;; int64 _seq_num
    8
    ;; float64 _time_stamp
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
     ;; int64 _seq_num
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _seq_num (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _seq_num) (= (length (_seq_num . bv)) 2)) ;; bignum
              (write-long (ash (elt (_seq_num . bv) 0) 0) s)
              (write-long (ash (elt (_seq_num . bv) 1) -1) s))
             ((and (class _seq_num) (= (length (_seq_num . bv)) 1)) ;; big1
              (write-long (elt (_seq_num . bv) 0) s)
              (write-long (if (>= _seq_num 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _seq_num s)(write-long (if (>= _seq_num 0) 0 #xffffffff) s)))
     ;; float64 _time_stamp
       (sys::poke _time_stamp (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
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
   ;; int64 _seq_num
#+(or :alpha :irix6 :x86_64)
      (setf _seq_num (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _seq_num (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;; float64 _time_stamp
     (setq _time_stamp (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; int32 _tgt_uav_count
     (setq _tgt_uav_count (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get simulation_m100::PubSubUavMsg :md5sum-) "00172b158c3e0d8e7ba9d8219ae8233f")
(setf (get simulation_m100::PubSubUavMsg :datatype-) "simulation_m100/PubSubUavMsg")
(setf (get simulation_m100::PubSubUavMsg :definition-)
      "int32 src_uav_id
string component
int32 cmd_priority
string playload
float64 start_time
int64 seq_num
float64 time_stamp
int32 tgt_uav_count
")



(provide :simulation_m100/PubSubUavMsg "00172b158c3e0d8e7ba9d8219ae8233f")


