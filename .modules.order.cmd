cmd_/home/ubuntu/Driver_Project/modules.order := {   echo /home/ubuntu/Driver_Project/mymodule.ko; :; } | awk '!x[$$0]++' - > /home/ubuntu/Driver_Project/modules.order
