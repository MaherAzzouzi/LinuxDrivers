cmd_/home/maher/kernel/lkm/LinuxDrivers/driver5/Module.symvers := sed 's/\.ko$$/\.o/' /home/maher/kernel/lkm/LinuxDrivers/driver5/modules.order | scripts/mod/modpost    -o /home/maher/kernel/lkm/LinuxDrivers/driver5/Module.symvers -e -i Module.symvers   -T -
