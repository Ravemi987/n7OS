#include <n7OS/mem.h>
#include <stdio.h>

#define NB_PAGES ((uint32_t)LAST_MEMORY_INDEX/(uint32_t)PAGE_SIZE)
#define BITMAP_SIZE ((uint32_t)NB_PAGES / (uint32_t) 32)


uint32_t free_page_bitmap_table[BITMAP_SIZE];

/**
 * @brief Marque la page allouée
 * 
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 * 
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {
    // On divise addr par PAGE_SIZE pour avoir un numéro physique de page entre 0 et NB_PAGES
    // (index / 32) map l'adresse dans une case et (index % 32) donne l'indice du bit dans cette case
    // on met le bit à 1
    uint32_t index = addr / PAGE_SIZE;
    free_page_bitmap_table[index / 32] |= (1 << (index % 32));
}

/**
 * @brief Désalloue la page
 * 
 * Libère la page allouée.
 * 
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
    // On divise addr par PAGE_SIZE pour avoir un numéro physique de page entre 0 et NB_PAGES
    // (index / 32) map l'adresse dans une case et (index % 32) donne l'indice du bit dans cette case
    // on clear le bit: on fait un 'not' puis un 'and' pour ne pas clear tous les bits
    uint32_t index = addr / PAGE_SIZE;
    free_page_bitmap_table[index / 32] &= ~(1 << (index % 32));   
}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 * 
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage() {
    uint32_t adresse= 0xFFFFFFFF;

    // On itère sur chaque case du bitmap (NB_PAGES / 32)
    for (uint32_t i = 0; i < BITMAP_SIZE; ++i) {

        // Si au moins une page est libre dans le bloc (pas que des bits à 1)
        if (free_page_bitmap_table[i] != 0xFFFFFFFF) {

            // On parcours cette case du bitmap pour trouver la page libre
            for (uint32_t j = 0; j < 32; ++j) {
                // L'adresse réelle c'est son adresse dans le tableau * la size
                adresse = (i * 32 + j) * PAGE_SIZE;

                // Si on a trouvé le bit à 0
                if (!(free_page_bitmap_table[i] & (1 << j))) {
                    // On alloue cette page
                    setPage(adresse);
                    return adresse;
                }
            }
        }
    }
    return adresse;
}

/**
 * @brief Initialise le gestionnaire de mémoire physique
 * 
 */
void init_mem() {
    for (uint32_t i = 0; i < BITMAP_SIZE; ++i) {
        free_page_bitmap_table[i] = 0;
    }
}

/**
 * @brief Affiche l'état de la mémoire physique
 * 
 */
void print_mem() {
    uint32_t free_count = 0;

    for (uint32_t i = 0; i < NB_PAGES; ++i) {
        uint32_t index = i / 32;
        if (!(free_page_bitmap_table[index] & (1 << (i % 32)))) {
            free_count ++;
        }
    } 
    
    uint32_t occupied_count = NB_PAGES - free_count;

    printf("Nombre de pages occupees : %d\n", occupied_count);
    printf("Nombre de pages libres : %d\n", free_count);
}
