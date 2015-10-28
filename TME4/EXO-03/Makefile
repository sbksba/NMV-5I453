.PHONY: all 
#.SECONDARY:

ifneq ($(KERNELRELEASE),)

obj-m := hanging.o

else

  #KERNELDIR ?= /lib/modules/$(shell uname -r)/build
  KERNELDIR ?= /tmp/linux-4.2.3
  PWD := $(shell pwd)

all :
	$(info obj-m : $(obj-m))
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

endif
