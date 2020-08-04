;; Auto-generated. Do not edit!


(when (boundp 'parrot_test::PubSubUavMsg)
  (if (not (find-package "PARROT_TEST"))
    (make-package "PARROT_TEST"))
  (shadow 'PubSubUavMsg (find-package "PARROT_TEST")))
(unless (find-package "PARROT_TEST::PUBSUBUAVMSG")
  (make-package "PARROT_TEST::PUBSUBUAVMSG"))

(in-package "ROS")
;;//! \htmlinclude PubSubUavMsg.msg.html


(defclass parrot_test::PubSubUavMsg
  :super ros::object
  :slots (_str ))

(defmethod parrot_test::PubSubUavMsg
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

(setf (get parrot_test::PubSubUavMsg :md5sum-) "994972b6e03928b2476860ce6c4c8e17")
(setf (get parrot_test::PubSubUavMsg :datatype-) "parrot_test/PubSubUavMsg")
(setf (get parrot_test::PubSubUavMsg :definition-)
      "string str
")



(provide :parrot_test/PubSubUavMsg "994972b6e03928b2476860ce6c4c8e17")


