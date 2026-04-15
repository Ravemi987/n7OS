#include <n7OS/paging.h>
#include <stddef.h> // nécessaire pour NULL
#include <n7OS/kheap.h>
#include <n7OS/mem.h>


void alloc_repertory_table();

RepertoryTable rpt;

void enable_paging(uint32_t page_repertory_addr) {
    __asm__ __volatile__(
        "mov %0, %%cr3\n\t"    // Charge l'adresse du répertoire
        "mov %%cr0, %%eax\n\t" // Lit le registre de contrôle 0
        "or $0x80000000, %%eax\n\t" // Met le bit 31 à 1
        "mov %%eax, %%cr0"     // Réécrit dans cr0 pour activer
        : 
        : "r" (page_repertory_addr)
        : "eax", "memory"
    );
}

void initialise_paging() {
    init_mem();
    init_heap();
    alloc_repertory_table();

    for (uint32_t i = 0; i < get_stack_top(); i += PAGE_SIZE) {
        alloc_page_entry(i, 1, 0);
    }

    enable_paging((uint32_t)rpt);
}

void alloc_repertory_table() {
    // Alloue un repertoire de pages
    // sizeof(RTE) = uint32_t. C'est un tableau de 1024 int32
    rpt = (RepertoryTable) kmalloc_a(sizeof(RTE) * 1024);

    for (int i = 0; i < 1024; ++i) {
        rpt[i].value = 0;
        PageTable pgtable = (PageTable) kmalloc_a(sizeof(PTE) * 1024);

        for (int j = 0; j < 1024; ++j) {
            pgtable[j].value = 0;
        }
        rpt[i].repertory_entry.addr = ((uint32_t)pgtable >> 12);
        rpt[i].repertory_entry.p = 1; 
        rpt[i].repertory_entry.rw = 1;
        rpt[i].repertory_entry.user = 1;
    }
}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    uint16_t indexPDE = address >> 22;
    uint16_t indexPTE = (address >> 12) & 0x3FF;

    // La table est allouée
    PageTable pagtab_ptr = (PageTable) (rpt[indexPDE].repertory_entry.addr << 12);
    
    // On prend une page physique libre.
    // Pour l'allocation de la première page, on a bien de l'identity mapping
    uint32_t new_page_addr = findfreePage();

    pagtab_ptr[indexPTE].page_entry.addr = new_page_addr >> 12;
    pagtab_ptr[indexPTE].page_entry.p = 1;
    pagtab_ptr[indexPTE].page_entry.rw = is_writeable;
    pagtab_ptr[indexPTE].page_entry.user = !is_kernel;

    return pagtab_ptr;
}
