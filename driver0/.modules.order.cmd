cmd_/home/maher/lkm/driver0/modules.order := {   echo /home/maher/lkm/driver0/hello.ko; :; } | awk '!x[$$0]++' - > /home/maher/lkm/driver0/modules.order
