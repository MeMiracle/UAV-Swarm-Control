
(cl:in-package :asdf)

(defsystem "simulation_m100-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "EnableMotors" :depends-on ("_package_EnableMotors"))
    (:file "_package_EnableMotors" :depends-on ("_package"))
  ))