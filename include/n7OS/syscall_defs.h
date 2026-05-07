#ifndef __SYSCALL_DEFS_H__
#define __SYSCALL_DEFS_H__

#include <n7OS/processus.h>

#define NB_SYSCALL 3

int sys_example();

int sys_shutdown(int n);

int sys_write(const char *s, int len);

typedef int (*fnptr)();

pid_t fork(const char *name, fnptr function);

int exit();

pid_t getpid();

int sleep(int seconds);

extern fnptr syscall_table[NB_SYSCALL];

void add_syscall(int num, fnptr function);

#endif
