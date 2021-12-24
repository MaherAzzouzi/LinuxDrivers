obj-m:=module_p.o

all:
	make -C ~/pwn/kernel/buildroot/output/build/linux-5.15 M=`pwd` modules
clean:
	$(MAKE) -C $(KERNELTREE) M=$(PWD) clean
