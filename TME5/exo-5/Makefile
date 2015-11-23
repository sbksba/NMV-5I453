.PHONY: all 
#.SECONDARY:

ifneq ($(KERNELRELEASE),)

  obj-m += helloioctl.o

else

  #KERNELDIR ?= /lib/modules/$(shell uname -r)/build
  KERNELDIR ?= /tmp/linux-4.2.3
  PWD := $(shell pwd)

all :
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc -Wall test.c -o test

proper:
	rm -f *~

clean:proper
	make -C $(KERNELDIR) M=$(PWD) clean
	rm -f test
endif
