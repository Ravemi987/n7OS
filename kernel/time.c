#include <n7OS/time.h>
#include <n7OS/console.h>
#include <../include/malloc.h>
#include <stdio.h>

uint32_t timer = 0; // en ms
uint32_t last_update = 0;

syst_time_t sys_time;

void increment_timer() {
    timer++;
}

uint32_t get_timer() {
    return timer;
}

void init_timer() {

    /* Channel 0, accès poids faible/poids fort, générateur
     * d'impulsion, fréquence définie en binaire */
    outb(0x34, 0x43);

    uint32_t freq = FREQ_OSC / HORLOGE;
    uint8_t low = freq & 0xFF;
    uint8_t high = (freq >> 8) & 0xFF;

    outb(low, CHANNEL0);
    outb(high, CHANNEL0);

    // Activation de l'IT
    outb(inb(PIC_CONFIG) & 0xfe, PIC_CONFIG);
}

void convert_timer() {
    uint32_t total_sec = timer / 1000;

    sys_time.sec = total_sec % 60;
    sys_time.min = (total_sec / 60) % 60;
    sys_time.hours = total_sec / 3600;
}

void put_time(char *time_buffer) {
    // heures
    time_buffer[0] = '0' + (sys_time.hours / 10);
    time_buffer[1] = '0' + (sys_time.hours % 10);

    time_buffer[2] = ':';

    // minutes
    time_buffer[3] = '0' + (sys_time.min / 10);
    time_buffer[4] = '0' + (sys_time.min % 10);

    time_buffer[5] = ':';

    // seconds
    time_buffer[6] = '0' + (sys_time.sec / 10);
    time_buffer[7] = '0' + (sys_time.sec % 10);

    time_buffer[8] = '\0';
}

void display_time() {
    uint32_t current_timer = get_timer();

    if (current_timer - last_update >= 1000) {
        convert_timer();
        
        char time_buffer[9];

        put_time(time_buffer);

        console_print_at(0, 72, time_buffer);
        
        last_update += 1000;
    }
}
