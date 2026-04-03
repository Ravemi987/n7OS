#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>
#include <stdio.h>

extern void handler_IT();
extern void handler_IT50();
extern void handler_IT20();

void init_irq() {
    init_irq_entry(50, (uint32_t)handler_IT50);
    init_irq_entry(0x20, (uint32_t)handler_IT20);
}

void handler_en_C() {
}

void handler_it50() {
    printf("Interruption recue !\n");
}

void handler_it20() {
    outb(inb(PIC_CONFIG)|1, PIC_CONFIG);      // Désactivation de l'IT
    outb(PIC_CMD, PIC_CMD);                   // ACK aus PIC Master

    increment_timer();                        // Incrémentation du compteur système
    display_time();

    outb(inb(PIC_CONFIG) & 0xfe, PIC_CONFIG); // Ré-activation de l'IT
}
