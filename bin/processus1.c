#include <inttypes.h>
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

extern void terminal();


void processus1() {
    while(1) {
        printf("test\n");
    }
    // printf("Tests du kernel :\n");
    // printf("\n");
    
    // printf("Test memoire :\n");
    // print_mem();

    // // --- Test paging ---

    // printf("Test page fault :\n");
    // alloc_page_entry(0xA000000, 1, 1);
    // uint32_t *ptr = (uint32_t *)0xA000000;
    // int page_fault = *ptr;
    // page_fault = 0;
    // printf ("%d\n", page_fault);

    // // --- Test clavier ---
    // // char buf[256];
    // // printf("Test clavier - Entrez une chaîne de caractères : ");
    // // scanf("%s", buf);
    // // printf("Vous avez entre : %s\n", buf);


    // // --- Test interruption ---
    // printf("Test interruption :\n");
    // //__asm__("int $50");


    // // --- Test appels systèmes ---
    // // printf("Processus [%d] : sleep pendant 3 secondes\n", get_pid());
    // // sleep(3);
    // // printf("Processus [%d] : réveillé après 3 secondes\n", get_pid());

    // // printf("\nAppuyez sur Entrée pour lancer le terminal...\n");
    // // char c;
    // // while ((c = getchar()) != '\n' && c != '\r') {
    // //     c = getchar();
    // // }

    // // On lance le terminal
    // //fork("terminal", terminal);

    // //exit();
}
