/**
 * @file time.h
 * @brief Gestion du temps dans le noyau
 */

#ifndef _TIME_H_
#define _TIME_H_

#include <inttypes.h>
#include <n7OS/cpu.h>

#define CHANNEL0 0x40
#define CHANNEL1 0x41
#define CHANNEL2 0x42
#define COMMAND 0x43
#define FREQ_OSC 0x1234BD

// Horloge 1KHz
#define HORLOGE 0x3E8


/**
 * @brief Description du registre de contrôle.
 * 
 */
typedef struct {
    uint32_t channel_select : 2;
    uint32_t access_mode : 2;
    uint32_t operating_mode : 3;
    uint32_t bcd : 1;
} __attribute__((packed)) control_register_t;

typedef struct {
    uint32_t hours;
    uint32_t min;
    uint32_t sec;
} syst_time_t;


void init_timer();

void increment_timer();

uint32_t get_timer();

void display_time();

#endif
