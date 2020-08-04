
(cl:in-package :asdf)

(defsystem "wifistatus-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "CommInfo" :depends-on ("_package_CommInfo"))
    (:file "_package_CommInfo" :depends-on ("_package"))
    (:file "Position" :depends-on ("_package_Position"))
    (:file "_package_Position" :depends-on ("_package"))
  ))