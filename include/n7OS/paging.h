/**
 * @file paging.h
 * @brief Gestion de la pagination dans le noyau
 */
#ifndef _PAGING_H
#define _PAGING_H

#include <inttypes.h>

/**
 * @brief Description d'une ligne de la table de page
 * 
 */
typedef struct {
    uint32_t p : 1;
    uint32_t rw : 1;
    uint32_t user : 1;
    uint32_t accessed : 1;
    uint32_t reserved : 4;
    uint32_t dirty : 1;
    uint32_t available : 3;
    uint32_t addr : 20;
} __attribute__((packed)) page_table_entry_t;

/**
 * @brief Une entrée dans la table de page peut être manipulée en utilisant
 *        la structure page_table_entry_t ou directement la valeur
 */
typedef union {
    page_table_entry_t page_entry;
    uint32_t value;
} PTE; // PTE = Page Table Entry 

/**
 * @brief Une table de page (PageTable) est un tableau de descripteurs de page
 * 
 */
typedef PTE * PageTable;


/**
 * @brief Description d'une ligne du répertoire de page
 */
typedef struct {
    uint32_t p : 1;
    uint32_t rw : 1;
    uint32_t user : 1;
    uint32_t reserved : 9;
    uint32_t addr : 20;
} __attribute__((packed)) page_repertory_entry_t;

/**
 * @brief Une entrée dans le répertoire de page peut être manipulée en utilisant
 *        la structure page_repertory_entry_t ou directement la valeur
 */
typedef union {
    page_repertory_entry_t repertory_entry;
    uint32_t value;
} RTE;


typedef RTE * RepertoryTable;

extern RepertoryTable rpt;

/**
 * @brief Cette fonction initialise le répertoire de page, alloue les pages de table du noyau
 *        et active la pagination
 * 
 */
void initialise_paging();

/**
 * @brief Cette fonction alloue une page de la mémoire physique à une adresse de la mémoire virtuelle
 * 
 * @param address       Adresse de la mémoire virtuelle à mapper
 * @param is_writeable  Si is_writeable == 1, la page est accessible en écriture
 * @param is_kernel     Si is_kernel == 1, la page ne peut être accédée que par le noyau
 * @return PageTable    La table de page modifiée
 */
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);
#endif
