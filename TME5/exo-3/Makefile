.PHONY: all 
#.SECONDARY:

ifneq ($(KERNELRELEASE),)

  obj-m += hellosysfs.o

else

  #KERNELDIR ?= /lib/modules/$(shell uname -r)/build
  KERNELDIR ?= /tmp/linux-4.2.3
  PWD := $(shell pwd)

all :
	make -C $(KERNELDIR) M=$(PWD) modules

proper:
	rm -f *~

clean:proper
	make -C $(KERNELDIR) M=$(PWD) clean

endif
