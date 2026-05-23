#ifndef __SYSCALL_DEFS_H__
#define __SYSCALL_DEFS_H__

#include <n7OS/processus.h>

#define NB_SYSCALL 7

typedef int (*fnptr)();

extern fnptr syscall_table[NB_SYSCALL];
void add_syscall(int num, fnptr function);

int sys_shutdown(int n);
int sys_write(const char *s, int len);
pid_t sys_fork(const char *name, void *function);
int sys_exit();
pid_t sys_getpid();
int sys_sleep(int seconds);
int sys_wait(int pid);



#endif
