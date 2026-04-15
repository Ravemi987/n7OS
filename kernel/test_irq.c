#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>
#include <n7OS/keyboard.h>
#include <stdio.h>

extern void handler_IT();
extern void handler_IT50();
extern void handler_IT20();  // TIMER
extern void handler_IT21();  // CLAVIER

void init_irq() {
    init_irq_entry(50, (uint32_t)handler_IT50);
    init_irq_entry(0x20, (uint32_t)handler_IT20);
    init_irq_entry(0x21, (uint32_t)handler_IT21);
}

void handler_en_C() {
}

void handler_it50() {
    printf("Interruption recue !\n");
}

/* Interruption pour le timer (IT 0x20 ou 32)*/
void handler_it20() {
    outb(inb(0x21)|(1 << 0), 0x21);           // Désactivation de l'IT
    outb(0x20, 0x20);                         // ACK au PIC Master

    increment_timer();                        // Incrémentation du compteur système
    display_time();

    outb(inb(0x21) & ~(1 << 0), 0x21);        // Ré-activation de l'IT
}

/* Interruption pour le clavier (IT 0x21) */
void handler_it21() {
    outb(inb(0x21)|(1 << 1), 0x21);           // Désactivation de l'IT
    outb(0x20, 0x20);                         // ACK au PIC Master

    uint32_t char_code = inb(KEYB_ENCODER);
    read_code(char_code);

    outb(inb(0x21) & ~(1 << 1), 0x21);      // Ré-activation de l'IT
}
