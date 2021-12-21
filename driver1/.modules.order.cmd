cmd_/home/maher/lkm/driver1/modules.order := {   echo /home/maher/lkm/driver1/module_p.ko; :; } | awk '!x[$$0]++' - > /home/maher/lkm/driver1/modules.order
