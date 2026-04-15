#include <n7OS/keyboard.h>
#include <stdio.h>


#define BUF_SIZE 1024

char buffer[BUF_SIZE];
int read_pt = 0;
int write_pt = 0;
int shift_active = 0;


void init_keyboard() {
    // Activation de l'IT 0x21 (clavier)
    outb(inb(0x21) & ~(1 << 1), 0x21);
}

int is_pressed(uint32_t char_code) {
    return (char_code >> 7) & 0x1 == 1 ? 0 : 1;
}

int buffer_is_empty() {
    return read_pt == write_pt;
}

void read_code(uint32_t code) {
    if (code == SHIFT_PRESSED) {
        shift_active = 1;
        return;
    } else if (code == SHIFT_RELEASED) {
        shift_active = 0;
        return;
    }

    if (is_pressed(code) == 1) {
        push_code(code);
    }
}

void push_code(uint32_t scancode) {
    if ((write_pt + 1) % BUF_SIZE == read_pt) return;

    char ascii;
    if (shift_active) {
        ascii = (char)scancode_map_shift[scancode];
    } else {
        ascii = (char)scancode_map[scancode];
    }

    if (ascii != 0) {
        buffer[write_pt] = ascii;
        printf("%c", buffer[write_pt]);
        write_pt = (write_pt + 1) % BUF_SIZE;
    }
}

char kgetch() {
    while(buffer_is_empty()) {}

    char c = buffer[read_pt];
    read_pt = (read_pt + 1) % BUF_SIZE;

    return c;
}
