obj-m=module.o
EXTRA_CFLAGS := -I$(KERNELTREE)/fs/proc/internal.h

all:
	$(MAKE) -C $(KERNELTREE) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELTREE) M=$(PWD) clean