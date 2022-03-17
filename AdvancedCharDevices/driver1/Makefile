obj-m:=module1.o

all:
	$(MAKE) -C $(KERNELPATH) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELPATH) M=$(PWD) clean
