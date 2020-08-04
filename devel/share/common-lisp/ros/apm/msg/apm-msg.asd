
(cl:in-package :asdf)

(defsystem "apm-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "recv_msg" :depends-on ("_package_recv_msg"))
    (:file "_package_recv_msg" :depends-on ("_package"))
    (:file "send_msg" :depends-on ("_package_send_msg"))
    (:file "_package_send_msg" :depends-on ("_package"))
  ))