#ifndef _PROCESSUS_H
#define _PROCESSUS_H

#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/queue.h>
#include <n7OS/time.h>

#define NB_PROC 10
#define STACK_SIZE 1024

typedef uint32_t pid_t;

typedef enum {
    LIBRE,  // Case vide dans process_table
    ELU,    // En cours d'exécution
    PRET,   // Dans la file d'attente
    BLOQUE  // En attente du timer
} process_state_t;

typedef struct {
    pid_t pid;
    process_state_t state;
    void *stack_base;
    const char *name;
    uint32_t regs[5];
    uint32_t wake_time;
} process_t;

// Appels systèmes dans syscall_defs.h

pid_t creer_processus(const char *name, void *function);

void terminer_processus();

process_t* get_processus(pid_t pid);

void reveiller_processus();

pid_t get_pid();

void schedule();

void init_process();

void idle();

#endif
