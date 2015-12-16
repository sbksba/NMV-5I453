#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char **argv)
{
  long int mama;
  
  if (argc < 2)
    mama = syscall(324, "world");
  else
    mama = syscall(324, argv[1]);
  printf("[SYS_HELLO] call -> %ld\n", mama);

  return 0;
}
