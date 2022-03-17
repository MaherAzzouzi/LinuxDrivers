obj-m:=module0.o

all:
	$(MAKE) -C $(KERNELPATH) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELPATH) M=$(PWD) clean
