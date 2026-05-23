#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>
#include <n7OS/mem.h>
#include <n7OS/time.h>
#include <unistd.h>
#include <stdio.h>
#include <n7OS/keyboard.h>
#include <n7OS/processus.h>

extern RepertoryTable rpt;
extern void init_irq();
extern void init_syscall();
extern void processus1();


void kernel_start(void)
{
    // Attention à ne pas utiliser ailleurs
    init_console();             // Console
    initialise_paging();        // Paging
    setup_base((uint32_t)rpt);
    init_irq();                 // Timer
    init_timer();
    init_keyboard();            // Clavier
    init_syscall();             // Appels systèmes
    
    init_process();
    creer_processus("processus1", processus1);
    
    sti();
    
    while(1) {
        hlt();
    }
}
