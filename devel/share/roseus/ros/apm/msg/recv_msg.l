;; Auto-generated. Do not edit!


(when (boundp 'apm::recv_msg)
  (if (not (find-package "APM"))
    (make-package "APM"))
  (shadow 'recv_msg (find-package "APM")))
(unless (find-package "APM::RECV_MSG")
  (make-package "APM::RECV_MSG"))

(in-package "ROS")
;;//! \htmlinclude recv_msg.msg.html


(defclass apm::recv_msg
  :super ros::object
  :slots (_recv_msg ))

(defmethod apm::recv_msg
  (:init
   (&key
    ((:recv_msg __recv_msg) (make-array 263 :initial-element 0 :element-type :char))
    )
   (send-super :init)
   (setq _recv_msg __recv_msg)
   self)
  (:recv_msg
   (&optional __recv_msg)
   (if __recv_msg (setq _recv_msg __recv_msg)) _recv_msg)
  (:serialization-length
   ()
   (+
    ;; uint8[263] _recv_msg
    (* 1    263)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8[263] _recv_msg
     (princ _recv_msg s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8[263] _recv_msg
   (setq _recv_msg (make-array 263 :element-type :char))
   (replace _recv_msg buf :start2 ptr-) (incf ptr- 263)
   ;;
   self)
  )

(setf (get apm::recv_msg :md5sum-) "dc444b49bfa57eb79eb16e3052e264e0")
(setf (get apm::recv_msg :datatype-) "apm/recv_msg")
(setf (get apm::recv_msg :definition-)
      "uint8[263] recv_msg

")



(provide :apm/recv_msg "dc444b49bfa57eb79eb16e3052e264e0")


