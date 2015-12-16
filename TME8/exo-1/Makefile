.PHONY: all
#.SECONDARY:

ifneq ($(KERNELRELEASE),)

  obj-m += syscall.o

else

  #KERNELDIR ?= /lib/modules/$(shell uname -r)/build
  KERNELDIR ?= /tmp/linux-4.2.3
  PWD := $(shell pwd)

all : NRKILL HELLO HELLOARG HELLORET
	#make -C $(KERNELDIR) M=$(PWD) modules

proper:
	rm -f *~

clean:proper
	rm -f NRKILL HELLO HELLOARG HELLORET
	#make -C $(KERNELDIR) M=$(PWD) clean

endif

NRKILL: NRKill.c
	gcc -Wall $^ -o $@

HELLO: hello.c
	gcc -Wall $^ -o $@

HELLOARG: helloArg.c
	gcc -Wall $^ -o $@

HELLORET: helloRet.c
	gcc -Wall $^ -o $@
