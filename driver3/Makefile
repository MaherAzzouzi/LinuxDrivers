obj-m:=module.o

all:
	make -C $(KERNELTREE) M=`pwd` modules
clean:
	make -C $(KERNELTREE) M=`pwd` clean