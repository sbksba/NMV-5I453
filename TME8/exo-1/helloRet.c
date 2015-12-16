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
    mama = syscall(324, "world", buf, strlen("world"));
  else
    mama = syscall(324, argv[1], buf, strlen(argv[1]));
  printf("[SYS_HELLO] call -> %ld\n", mama);
  printf("[SYS_HELLO] %s\n", buf);
  
  return 0;
}
