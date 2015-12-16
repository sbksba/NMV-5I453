#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char **argv)
{
  long int mama;
  char buf[80];
  
  if (argc < 2)
    mama = syscall(325, "world", buf);
  else
    mama = syscall(325, argv[1], buf);
  printf("[SYS_HELLO] call -> %ld\n", mama);
  printf("[SYS_HELLO] %s", buf);
  
  return 0;
}
