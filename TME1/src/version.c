#include<stdlib.h>
#include<stdio.h>

#include"version.h"

int is_unstable(struct version *v)
{
  	return 1 & ((char *)v)[sizeof(short)];
}

int isUnstableBis(struct version *v)
{
  	return 1 & v->minor;
}

void display_version(struct version *v, int(*ptrfonction)(struct version *))
{
	printf("%2u-%lu %-20s", v->major, v->minor,
	       (*ptrfonction)(v) ? "(unstable)" : "(stable)");
}

int cmp_version(struct version *v, int major, int minor)
{
	return v->major == major && v->minor == minor;
}
