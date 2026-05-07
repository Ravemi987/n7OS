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
    sti();                      // Interruptions
    init_syscall();             // Appels systèmes
    init_kernel_process();
    

    // print_mem();


    // --- Test clavier ---
    // char buf[256];
    // printf("Entrez une chaîne de caractères : ");
    // scanf("%s", buf);
    // printf("Vous avez entre : %s\n", buf);


    // --- Test interruption ---
    //__asm__("int $50");

    
    // --- Test paging ---

    //alloc_page_entry(0xA000000, 1, 1);
    // uint32_t *ptr = (uint32_t *)0xA000000;
    // int page_fault = *ptr;
    // page_fault = 0;
    //printf ("%d\n", page_fault);


    // --- Test appels systèmes ---
    //shutdown(1);

    creer_processus(idle);
    creer_processus(processus1);
    
    // on ne doit jamais sortir de kernel_start
    while (1) {
        hlt();
    }
}
