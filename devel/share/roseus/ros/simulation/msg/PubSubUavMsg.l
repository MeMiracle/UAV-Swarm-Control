;; Auto-generated. Do not edit!


(when (boundp 'simulation::PubSubUavMsg)
  (if (not (find-package "SIMULATION"))
    (make-package "SIMULATION"))
  (shadow 'PubSubUavMsg (find-package "SIMULATION")))
(unless (find-package "SIMULATION::PUBSUBUAVMSG")
  (make-package "SIMULATION::PUBSUBUAVMSG"))

(in-package "ROS")
;;//! \htmlinclude PubSubUavMsg.msg.html


(defclass simulation::PubSubUavMsg
  :super ros::object
  :slots (_str ))

(defmethod simulation::PubSubUavMsg
  (:init
   (&key
    ((:str __str) "")
    )
   (send-super :init)
   (setq _str (string __str))
   self)
  (:str
   (&optional __str)
   (if __str (setq _str __str)) _str)
  (:serialization-length
   ()
   (+
    ;; string _str
    4 (length _str)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _str
       (write-long (length _str) s) (princ _str s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _str
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _str (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(setf (get simulation::PubSubUavMsg :md5sum-) "994972b6e03928b2476860ce6c4c8e17")
(setf (get simulation::PubSubUavMsg :datatype-) "simulation/PubSubUavMsg")
(setf (get simulation::PubSubUavMsg :definition-)
      "string str
")



(provide :simulation/PubSubUavMsg "994972b6e03928b2476860ce6c4c8e17")


