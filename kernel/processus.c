#include <n7OS/processus.h>
#include <stdio.h>
#include <malloc.h>


extern void ctx_sw(uint32_t *regs_old, uint32_t *regs_new);


process_t process_table[NB_PROC];
Queue *file_processus_prets;
process_t *processus_actif = NULL;


pid_t getPid() {
    return processus_actif != NULL ? processus_actif->pid : (pid_t)-1;
}


pid_t allouer_pid() {
    for (pid_t i = 0; i < NB_PROC; i++) {
        if (process_table[i].state == LIBRE) {
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
    process_t *p = &process_table[getPid()];

    if (p->state == ELU) {
        p->state = LIBRE;
        free(p->stack_base);
        schedule();
    }
}


void arreter_processus() {
    process_t *p = &process_table[getPid()];

    if (p->state == ELU) {
        p->state = BLOQUE;
        schedule();
    }
}


void debloquer_processus(pid_t pid) {
    process_t *p = &process_table[pid];

    if (p->state == BLOQUE) {
        push(file_processus_prets, p);
        p->state = PRET;
        schedule();
    }
}


void bloquer_processus(pid_t pid) {
    process_t *p = &process_table[pid];

    
    if (p->state == ELU) {
        p->state = BLOQUE;
        schedule();
    } else if (p->state == PRET) {
        remove(file_processus_prets, p);
        p->state = BLOQUE;
    }
}


void init_kernel_process() {
    file_processus_prets = createQueue();

    process_t *p0 = &process_table[0];
    p0->pid = 0;
    p0->state = ELU;
    processus_actif = p0;
}


pid_t creer_processus(void *func) {
    pid_t pid;

    if ((pid = allouer_pid()) == (pid_t)-1) return (pid_t)-1;
    
    // Allocation de la pile et calcul du sommet de pile
    void *stack = malloc(STACK_SIZE);
    if (stack == NULL) return -1;
    uint32_t *stack_top = (uint32_t *)((uint8_t *)stack + STACK_SIZE);

    // Astuce: si un process n'a pas de boucle, on passe l'aadresse pour qu'il se termine sans crash
    stack_top--;
    *stack_top = (uint32_t)terminer_processus;
    
    // On empile l'adresse de la fonction;
    stack_top--;
    *stack_top = (uint32_t)func;
    
    // On initialise le descripteur du process
    process_table[pid].stack_base = stack;
    process_table[pid].pid = pid;
    process_table[pid].state = PRET;
    
    init_regs(pid, stack_top);
    push(file_processus_prets, &process_table[pid]);
    
    return pid;
}


void schedule() {
    if (isEmpty(file_processus_prets)) return;

    process_t *old_process = processus_actif;
    process_t *new_process = (process_t *)getHead(file_processus_prets);
    pop(file_processus_prets);

    if (old_process->state == ELU) {
        old_process->state = PRET;
        push(file_processus_prets, old_process);
    }

    new_process->state = ELU;
    processus_actif = new_process;

    ctx_sw(old_process->regs, new_process->regs);
}


void idle() {
    while (1) {
        hlt();
    }
}
