.PHONY: all
#.SECONDARY:

ifneq ($(KERNELRELEASE),)

  obj-m := helloWorld.o #helloWorldParam.o

else

  KERNELDIR ?= /tmp/linux-4.2.3 #/lib/modules/$(shell uname -r)/build
  PWD := $(shell pwd)

all :
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

endif
