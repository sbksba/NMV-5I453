.PHONY : all clean check
.ONESHELL:

#export LD_LIBRARY_PATH=/users/Etu5/2900825/cours/NMV/TME3/EXO-01
#LD_PRELOAD=./libread.so ./cron_func

LDFLAG= -L . -lfunc
LIB= libfunc.so libread.so

CHECK_PATCH=../../../../biblio/eudyptula/torvalds/scripts/checkpatch.pl

all : $(LIB) cron_func

%.o : %.c
	gcc -Wall -c $<

cron_func : cron_func.o #nothing.o
	gcc -Wall -o $@ $^ $(LDFLAG)

check :
	for f in *.c *.h ; do
		$(CHECK_PATCH) -f $$f
	done

clean :
	rm -rf *.o cron_func *.so *~

libfunc.so: nothing.c
	gcc -fPIC -shared -o $@ $<

libread.so: my_read.c
	gcc -fPIC -shared -o $@ $< -ldl
