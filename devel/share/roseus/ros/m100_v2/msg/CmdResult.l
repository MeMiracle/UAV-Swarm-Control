;; Auto-generated. Do not edit!


(when (boundp 'm100_v2::CmdResult)
  (if (not (find-package "M100_V2"))
    (make-package "M100_V2"))
  (shadow 'CmdResult (find-package "M100_V2")))
(unless (find-package "M100_V2::CMDRESULT")
  (make-package "M100_V2::CMDRESULT"))

(in-package "ROS")
;;//! \htmlinclude CmdResult.msg.html


(defclass m100_v2::CmdResult
  :super ros::object
  :slots (_component _command _status ))

(defmethod m100_v2::CmdResult
  (:init
   (&key
    ((:component __component) "")
    ((:command __command) "")
    ((:status __status) nil)
    )
   (send-super :init)
   (setq _component (string __component))
   (setq _command (string __command))
   (setq _status __status)
   self)
  (:component
   (&optional __component)
   (if __component (setq _component __component)) _component)
  (:command
   (&optional __command)
   (if __command (setq _command __command)) _command)
  (:status
   (&optional __status)
   (if __status (setq _status __status)) _status)
  (:serialization-length
   ()
   (+
    ;; string _component
    4 (length _component)
    ;; string _command
    4 (length _command)
    ;; bool _status
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _component
       (write-long (length _component) s) (princ _component s)
     ;; string _command
       (write-long (length _command) s) (princ _command s)
     ;; bool _status
       (if _status (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _component
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _component (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _command
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _command (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; bool _status
     (setq _status (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(setf (get m100_v2::CmdResult :md5sum-) "50fc9051b66559e2902987e4c3f3c68f")
(setf (get m100_v2::CmdResult :datatype-) "m100_v2/CmdResult")
(setf (get m100_v2::CmdResult :definition-)
      "string component
string command
bool status
")



(provide :m100_v2/CmdResult "50fc9051b66559e2902987e4c3f3c68f")


