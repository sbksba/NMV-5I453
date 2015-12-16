#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[])
{
  pid_t tid;

  printf("[NRKILL] before kill\n");
  tid = syscall(SYS_gettid);
  tid = syscall(SYS_tgkill, getpid(), tid, __NR_kill);
  printf("[NRKILL] after kill (%d)\n",(int)tid);
  
  return 0;
}
