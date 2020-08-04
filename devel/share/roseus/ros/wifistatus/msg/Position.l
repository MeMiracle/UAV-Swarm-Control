;; Auto-generated. Do not edit!


(when (boundp 'wifistatus::Position)
  (if (not (find-package "WIFISTATUS"))
    (make-package "WIFISTATUS"))
  (shadow 'Position (find-package "WIFISTATUS")))
(unless (find-package "WIFISTATUS::POSITION")
  (make-package "WIFISTATUS::POSITION"))

(in-package "ROS")
;;//! \htmlinclude Position.msg.html


(defclass wifistatus::Position
  :super ros::object
  :slots (_kind _x _y _z ))

(defmethod wifistatus::Position
  (:init
   (&key
    ((:kind __kind) 0)
    ((:x __x) 0.0)
    ((:y __y) 0.0)
    ((:z __z) 0.0)
    )
   (send-super :init)
   (setq _kind (round __kind))
   (setq _x (float __x))
   (setq _y (float __y))
   (setq _z (float __z))
   self)
  (:kind
   (&optional __kind)
   (if __kind (setq _kind __kind)) _kind)
  (:x
   (&optional __x)
   (if __x (setq _x __x)) _x)
  (:y
   (&optional __y)
   (if __y (setq _y __y)) _y)
  (:z
   (&optional __z)
   (if __z (setq _z __z)) _z)
  (:serialization-length
   ()
   (+
    ;; int32 _kind
    4
    ;; float32 _x
    4
    ;; float32 _y
    4
    ;; float32 _z
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _kind
       (write-long _kind s)
     ;; float32 _x
       (sys::poke _x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _y
       (sys::poke _y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _z
       (sys::poke _z (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _kind
     (setq _kind (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32 _x
     (setq _x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _y
     (setq _y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _z
     (setq _z (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get wifistatus::Position :md5sum-) "381238523f8add39b00b8a991b078417")
(setf (get wifistatus::Position :datatype-) "wifistatus/Position")
(setf (get wifistatus::Position :definition-)
      "int32 kind
float32 x
float32 y
float32 z
")



(provide :wifistatus/Position "381238523f8add39b00b8a991b078417")


