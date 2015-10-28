#define _GNU_SOURCE
#include<stdio.h>
#include<unistd.h>
#include<dlfcn.h>

typedef ssize_t (*orig_read_f_type)(int, void *, size_t);

ssize_t read(int fd, void *buf, size_t count)
{
	orig_read_f_type orig_read = (orig_read_f_type)dlsym(RTLD_NEXT, "read");
	ssize_t ret = orig_read(fd, buf, count);
	if (*(char*)buf == 'r') {
		*(char*)buf = 'i';
		printf("AHAHAHAHAH\n", *(char*)buf);
	}
	/* TODO : replace any 'r' by 'i' */
			
	return ret;
}
