cmd_/home/maher/lkm/driver0/Module.symvers := sed 's/\.ko$$/\.o/' /home/maher/lkm/driver0/modules.order | scripts/mod/modpost    -o /home/maher/lkm/driver0/Module.symvers -e -i Module.symvers   -T -
