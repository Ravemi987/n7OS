#include <n7OS/syscall_defs.h>

fnptr syscall_table[NB_SYSCALL];

void add_syscall(int num, fnptr function) {
  if (num < NB_SYSCALL) {
    syscall_table[num]= function;
  }
}

