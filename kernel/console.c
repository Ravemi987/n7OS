#include <n7OS/console.h>
#include <n7OS/cpu.h>


uint16_t *scr_tab;

void init_console() {
    scr_tab = (uint16_t *) SCREEN_ADDR;
    console_clear();
    console_put_cursor(0);
}

int is_control_char(const char c) {
    return c == '\b' || c == '\t' || c == '\n' || c == '\f' || c == '\r';
}

void console_clear() {
    for (uint16_t i = 0; i < VGA_SIZE; i++) {
        scr_tab[i] = ' ';
    }
}

void console_display_char(uint16_t pos, const char c) {
    scr_tab[pos] = CHAR_COLOR<<8|c;
}

void console_put_cursor(uint16_t pos) {
    outb(CMD_LOW, PORT_CMD);
    outb(pos&CMD_LOW, PORT_DATA);
    outb(CMD_HIGH, PORT_CMD);
    outb(pos&CMD_HIGH, PORT_DATA);
}

uint16_t console_get_cursor() {
    uint16_t pos;
    outb(CMD_LOW, PORT_CMD);
    pos = inb(PORT_DATA);
    outb(CMD_HIGH, PORT_CMD);
    pos = inb(PORT_DATA) | pos;

    return pos;
}

void console_putchar(const char c) {
    uint16_t pos = console_get_cursor();

    if (c > 31 && c < 127) {
        console_display_char(0, c);
    }
    console_put_cursor(pos);
}

void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}
