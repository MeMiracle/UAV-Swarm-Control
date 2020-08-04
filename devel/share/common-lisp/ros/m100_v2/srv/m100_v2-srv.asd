
(cl:in-package :asdf)

(defsystem "m100_v2-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "EnableMotors" :depends-on ("_package_EnableMotors"))
    (:file "_package_EnableMotors" :depends-on ("_package"))
  ))