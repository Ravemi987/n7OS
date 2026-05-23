#include <n7OS/processus.h>
#include <stdio.h>
#include <malloc.h>
#include <n7OS/cpu.h>
#include <n7OS/syscall_defs.h>

extern void ctx_sw(uint32_t *regs_old, uint32_t *regs_new);

process_t process_table[NB_PROC];
process_t *processus_actif = NULL;


pid_t get_pid() {
    return processus_actif != NULL ? processus_actif->pid : (pid_t)-1;
}

process_t* get_processus(pid_t pid) {
    if (pid >= NB_PROC) return NULL;
    return &process_table[pid];
}

pid_t allouer_pid() {
    for (pid_t i = 0; i < NB_PROC; i++) {
        if (process_table[i].state == LIBRE && process_table[i].stack_base == NULL) {
            return i;
        }
    }
    return -1;
}

void init_regs(pid_t pid, uint32_t *stack_top) {
    for (int i = 0; i < 5; i++) process_table[pid].regs[i] = 0;
    process_table[pid].regs[1] = (uint32_t)stack_top;
}

void terminer_processus() {
    process_t *p = &process_table[get_pid()];
    if (p->state == ELU) {
        p->state = LIBRE;
        schedule();
    }
    while(1);
}

void arreter_processus() {
    process_t *p = &process_table[get_pid()];
    if (p->state == ELU) {
        p->state = BLOQUE;
        schedule();
    }
}

void debloquer_processus(pid_t pid) {
    process_t *p = &process_table[pid];
    if (p->state == BLOQUE) {
        p->state = PRET;
    }
}

void bloquer_processus(pid_t pid) {
    process_t *p = &process_table[pid];
    if (p->state == ELU) {
        p->state = BLOQUE;
        schedule();
    } else if (p->state == PRET) {
        p->state = BLOQUE;
    }
}

// Wrapper C pour simuler le retour d'interruption
void process_wrapper(void (*func)()) {
    sti();
    func();
    terminer_processus();
    while(1);
}


pid_t creer_processus(const char *name, void *function) {
    pid_t pid;
    if ((pid = allouer_pid()) == (pid_t)-1) return (pid_t)-1;
    
    void *stack = malloc(STACK_SIZE);
    if (stack == NULL) return -1;
    
    uint32_t *stack_top = (uint32_t *)((uint8_t *)stack + STACK_SIZE);
    stack_top = (uint32_t *)((uint32_t)stack_top & 0xFFFFFFF0);
    stack_top -= 4; 
    
    stack_top[0] = (uint32_t)function;           
    stack_top[-1] = (uint32_t)terminer_processus; 
    stack_top[-2] = (uint32_t)process_wrapper;    
    
    process_table[pid].name = name;
    process_table[pid].stack_base = stack;
    process_table[pid].pid = pid;
    process_table[pid].state = PRET;
    
    init_regs(pid, &stack_top[-2]);
    
    return pid;
}


void init_process() {
    for (int i = 0; i < NB_PROC; i++) {
        process_table[i].state = LIBRE;
    }

    static process_t boot_context;
    boot_context.pid = 999;
    boot_context.state = LIBRE; 
    
    processus_actif = &boot_context;

    creer_processus("idle", idle);
}


void schedule() {
    //  Déterminer où l'on est dans le tableau
    int current_pid = 0;
    if (processus_actif != NULL && processus_actif->pid < NB_PROC) {
        current_pid = processus_actif->pid;
    }

    // Parcourir le tableau (Round-Robin) pour trouver le prochain processus PRET
    int next_pid = (current_pid + 1) % NB_PROC;
    int found = 0;

    for (int i = 0; i < NB_PROC; i++) {
        if (process_table[next_pid].state == PRET) {
            found = 1;
            break;
        }
        next_pid = (next_pid + 1) % NB_PROC;
    }

    // Si aucun n'est prêt, on reste sur l'actuel
    if (!found) return;

    process_t *old_process = processus_actif;
    process_t *new_process = &process_table[next_pid];

    // Mettre à jour les états
    if (old_process != NULL && old_process->state == ELU) {
        old_process->state = PRET;
    }

    new_process->state = ELU;
    processus_actif = new_process;

    // Basculer d eprocess
    if (old_process != NULL) {
        ctx_sw(old_process->regs, new_process->regs);
    }
}


void reveiller_processus() {
    uint32_t current_time = get_timer();

    for (int i = 0; i < NB_PROC; i++) {
        if (process_table[i].state == BLOQUE && process_table[i].wake_time <= current_time) {
            process_table[i].state = PRET;
        }
    }
}


void idle() {
    while (1) {
        // Garbage Collector
        for (int i = 0; i < NB_PROC; i++) {
            if (process_table[i].state == LIBRE && process_table[i].stack_base != NULL) {
                process_table[i].stack_base = NULL; 
            }
        }
        hlt();
    }
}
