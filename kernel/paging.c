#include <n7OS/paging.h>
#include <stddef.h> // nécessaire pour NULL
#include <n7OS/kheap.h>
#include <n7OS/mem.h>


void alloc_repertory_table();
void alloc_page_table(uint32_t indexPDE);
void alloc_first_page();


RepertoryTable rpt;

/*

        "mov %0, %%cr3\n\t"    // Charge l'adresse du répertoire
        "mov %%cr0, %%eax\n\t" // Lit le registre de contrôle 0
        "or $0x80000000, %%eax\n\t" // Met le bit 31 à 1
        "mov %%eax, %%cr0"     // Réécrit dans cr0 pour activer
        : : "r" (page_repertory_addr)
        : "eax"
*/


void enable_paging(uint32_t page_repertory_addr) {
    uint32_t cr0;
    __asm__ __volatile__("mov %0, %%cr3" :: "r"(page_repertory_addr));
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("mov %0, %%cr0" :: "r"(cr0));
}

void initialise_paging() {
    init_mem();
    alloc_repertory_table();

    for (int i = 0; i < get_stack_top(); i += PAGE_SIZE) {
        alloc_page_entry(i, 1, 1);
    }

    enable_paging((uint32_t)rpt);
}

void alloc_repertory_table() {
    // Alloue un repertoire de pages
    // sizeof(RTE) = uint32_t. C'est un tableau de 1024 int32
    rpt = (RepertoryTable) kmalloc_a(PAGE_SIZE);

    for (int i = 0; i < 1024; ++i) {
        rpt[i].value = 0;
    }
}

void alloc_page_table(uint32_t indexPDE) {
    // Alloue une table de pages
    PageTable pgtable = (PageTable) kmalloc_a(PAGE_SIZE);

    for (int i = 0; i < 1024; ++i) {
        pgtable[i].value = 0;
    }

    // L'adresse physique est stockée sur 20 bits
    rpt[indexPDE].repertory_entry.addr = ((uint32_t)pgtable >> 12);
    rpt[indexPDE].repertory_entry.iskernel = 0;
    rpt[indexPDE].repertory_entry.p = 1; // Elle est allouée
    rpt[indexPDE].repertory_entry.rw = 1; // Droit en écriture
}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    // index de la ligne dans le répertoire de pages
    uint16_t indexPDE = address >> 22;
    // index de la ligne dans la table de pages associée
    uint16_t indexPTE = (address >> 12) & 0x3FF;

    PageTable pagtab_ptr;

    // Si la table n'existe pas
    if (rpt[indexPDE].repertory_entry.p == 0) {
        // On alloue une nouvelle table
        alloc_page_table(indexPDE);
    }

    // L'adresse réelle doit être sur 32 bits
    pagtab_ptr = (PageTable) (rpt[indexPDE].repertory_entry.addr << 12);
    //PTE *pte = &pagtab_ptr[indexPTE];
    
    // Page allouée en mémoire physique
    uint32_t new_page_addr = findfreePage();

    // Mapping entre l'adresse virtuelle (indexPTE) et l'adresse physique (new_page_addr)
    pagtab_ptr[indexPTE].page_entry.addr = new_page_addr >> 12;
    pagtab_ptr[indexPTE].page_entry.p = 1;
    pagtab_ptr[indexPTE].page_entry.rw = is_writeable;
    pagtab_ptr[indexPTE].page_entry.iskernel = is_kernel;

    return pagtab_ptr;
}
