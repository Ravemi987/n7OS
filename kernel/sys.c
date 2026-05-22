#include <n7OS/sys.h>
#include <n7OS/syscall_defs.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <unistd.h>
#include <n7OS/cpu.h>


extern void handler_syscall();
extern void init_syscall();

void init_syscall() {
	// ajout de la fonction de traitement de l'appel systeme
	add_syscall(NR_shutdown, (fnptr)sys_shutdown);
	add_syscall(NR_write, (fnptr)sys_write);
	add_syscall(NR_fork, (fnptr)sys_fork);
	add_syscall(NR_exit, (fnptr)sys_exit);
	add_syscall(NR_getpid, (fnptr)sys_getpid);
	add_syscall(NR_sleep, (fnptr)sys_sleep);

	// initialisation de l'IT soft qui gère les appels systeme
	init_irq_entry(0x80, (uint32_t) handler_syscall);
}

int sys_shutdown(int n) {
	if (n == 1) {
		outw(0x2000, 0x604);
		return -1;
	} else {
		return n;
	}
}

int sys_write(const char *s, int len) {
  	console_putbytes(s, len);
  	return len;
}

int sys_exit() {
  	terminer_processus();
  	return 0;
}

pid_t sys_getpid() {
    return get_pid();
}

pid_t sys_fork(const char *name, void *function) {
	pid_t pid = creer_processus(name, function);
  	return pid;
}

int sys_sleep(int seconds) {
	process_t *p = get_processus(get_pid());

	p->wake_time = get_timer() + seconds * 1000;
	p->state = BLOQUE;

	schedule();

	return 0;
}
