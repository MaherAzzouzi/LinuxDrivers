obj-m:=module.o

all:
	$(MAKE) -C $(KERNELTREE) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELTREE) M=$(PWD) clean