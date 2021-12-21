cmd_/home/maher/lkm/driver1/Module.symvers := sed 's/\.ko$$/\.o/' /home/maher/lkm/driver1/modules.order | scripts/mod/modpost    -o /home/maher/lkm/driver1/Module.symvers -e -i Module.symvers   -T -
