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

extern RepertoryTable rpt;
extern void init_irq();

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
    

    // print_mem();

    // --- Test console ---
    printf("coucou\ntout\nle\nmonde\n");

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
    
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
