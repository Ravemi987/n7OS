#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>
#include <n7OS/mem.h>

extern RepertoryTable rpt;

void kernel_start(void)
{
    init_console();
    initialise_paging();
    setup_base((uint32_t)rpt);
    // print_mem();

    //printf("coucou\ntout\nle\nmonde");

    // lancement des interruptions
    sti();

    //alloc_page_entry(0xA000000, 1, 1);
    // uint32_t *ptr = (uint32_t *)0xA000000;
    // int page_fault = *ptr;
    // page_fault = 0;

    // printf ("%d\n", page_fault);

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
