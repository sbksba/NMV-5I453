.PHONY: all directories proper clean cleanall
#.SECONDARY:
CC=gcc
BIN=bin
INC=include
OBJ=obj
SRC=src

ifneq ($(KERNELRELEASE),)

  obj-m += ${SRC}/keyser.o

else

  #KERNELDIR ?= /lib/modules/$(shell uname -r)/build
  KERNELDIR ?= /tmp/linux-4.2.3
  PWD := $(shell pwd)

all : directories TEST
	make -C $(KERNELDIR) M=$(PWD) modules

directories: ${OBJ} ${BIN}

${OBJ}:
	mkdir -p ${OBJ}
${BIN}:
	mkdir -p ${BIN}

# =====
# OBJET
# =====
${OBJ}/test.o: ${SRC}/test.c
	${CC} -c -o $@ $< $(CFLAGS)

# ==========
# EXECUTABLE
# ==========
TEST: ${OBJ}/test.o 
	${CC} -o ${BIN}/$@ $^


proper:
	rm -f *~ ${SRC}/*~

clean:proper
	make -C $(KERNELDIR) M=$(PWD) clean
	rm -f ${BIN}/*

cleanall:clean
	rm -r ${BIN} ${OBJ}

endif
