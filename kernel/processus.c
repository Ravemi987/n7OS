#include <n7OS/processus.h>
#include <stdio.h>
#include <malloc.h>

extern void ctx_sw(uint32_t *regs_old, uint32_t *regs_new);


process_t process_table[NB_PROC];
Queue *file_processus_prets;
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
    process_t *p = &process_table[get_pid()];

    if (p->state == ELU) {
        p->state = LIBRE;
        schedule();
    }
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
        push(file_processus_prets, p);
        p->state = PRET;
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


pid_t creer_processus(const char *name, void *function) {
    pid_t pid;
    
    if ((pid = allouer_pid()) == (pid_t)-1) return (pid_t)-1;
    
    // Allocation de la pile et calcul du sommet de pile
    void *stack = malloc(STACK_SIZE);
    if (stack == NULL) return -1;
    uint32_t *stack_top = (uint32_t *)((uint8_t *)stack + STACK_SIZE);
    
    // Astuce: si un process n'a pas de boucle, on passe l'addresse pour qu'il se termine sans crash
    stack_top--;
    *stack_top = (uint32_t)terminer_processus;
    
    // On empile l'adresse de la fonction;
    stack_top--;
    *stack_top = (uint32_t)function;
    
    // On initialise le descripteur du process
    process_table[pid].name = name;
    process_table[pid].stack_base = stack;
    process_table[pid].pid = pid;
    process_table[pid].state = PRET;
    
    init_regs(pid, stack_top);
    push(file_processus_prets, &process_table[pid]);
    
    return pid;
}



void init_process() {
    // Initialisation de la file des prêts
    file_processus_prets = createQueue();

    // Initialisation de toute la table des processus à LIBRE
    for (int i = 0; i < NB_PROC; i++) {
        process_table[i].state = LIBRE;
    }

    // On utilise un contexte temporaire pour ne pas ecraser le registre du kernel après premier switch
    static process_t boot_context;
    boot_context.pid = 999;
    boot_context.state = LIBRE;
    
    processus_actif = &boot_context;

    // On créé le processus idle
    creer_processus("idle", idle);
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


void reveiller_processus() {
    uint32_t current_time = get_timer();

    for (int i = 0; i < NB_PROC; i++) {
        if (process_table[i].state == BLOQUE && process_table[i].wake_time <= current_time) {
            process_table[i].state = PRET;
            push(file_processus_prets, &process_table[i]);
        }
    }
}


void idle() {
    while (1) {
        hlt();
    }
}
