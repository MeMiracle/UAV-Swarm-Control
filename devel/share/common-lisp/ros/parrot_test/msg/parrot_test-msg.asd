
(cl:in-package :asdf)

(defsystem "parrot_test-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "PubSubUavMsg" :depends-on ("_package_PubSubUavMsg"))
    (:file "_package_PubSubUavMsg" :depends-on ("_package"))
  ))