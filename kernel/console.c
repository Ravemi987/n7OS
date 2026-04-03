#include <n7OS/console.h>
#include <n7OS/cpu.h>


/* ----- Prototypes ----- */

void console_put_cursor(uint16_t pos);
uint16_t console_get_cursor();
void console_clear(uint8_t start_row);
uint16_t get_index(uint16_t row, uint16_t column);
void console_print_header();

/* ----- Init ----- */

uint16_t *scr_tab;


void init_console() {
    scr_tab = (uint16_t *) SCREEN_ADDR;

    // On clear l'affichage et on positionne le curseur en haut à gauche 
    console_clear(0);
    console_print_header();
    console_put_cursor(get_index(HEADER_HEIGHT, 0));
}

void console_print_header() {
    console_print_at(0, 0, "Welcome to Sauron's n7OS !");
    console_print_at(0, 72, "00:00:00");

    for (uint16_t j = 0; j < VGA_WIDTH; j++) {
        console_display_char(get_index(HEADER_HEIGHT-1, j), '=');
    }
}

/* ----- Fonctions auxiliaires (wrapper) ----- */

void console_display_char(uint16_t pos, const char c) {
    scr_tab[pos] = (CHAR_COLOR << 8) | c;
}

void console_clear(uint8_t start_row) {
    for (uint16_t i = start_row; i < VGA_HEIGHT; i++) {
        for (uint16_t j = 0; j < VGA_WIDTH; j++) {
            console_display_char(get_index(i, j), ' ');
        }
    }
}

int is_control_char(const char c) {
    return c == '\b' || c == '\t' || c == '\n' || c == '\f' || c == '\r';
}

uint16_t get_index(uint16_t row, uint16_t column) {
    return VGA_WIDTH * row + column;
}

uint16_t get_row(uint16_t pos) {
    return pos / VGA_WIDTH;
}

uint16_t get_col(uint16_t pos) {
    return pos % VGA_WIDTH;
}

/* ----- Fonctions principales ----- */

void console_put_cursor(uint16_t pos) {
    outb(CMD_LOW, PORT_CMD);                    // On veut écrire dans le deuxième octet
    outb((uint8_t) (pos & 0xFF), PORT_DATA);    // On donne comme valeur les 8 bits de poids faibles
    outb(CMD_HIGH, PORT_CMD);                   // On veut écrire dans le premier octet
    outb((uint8_t) (pos >> 8), PORT_DATA);      // Valeur 'pos' décalée à droite de 8 bits (pour avoir les bits hauts)
}

uint16_t console_get_cursor() {
    uint16_t pos = 0;

    outb(CMD_HIGH, PORT_CMD);                       // On veut lire dans le premier octet
    pos = ((uint16_t) inb(PORT_DATA)) << 8;         // On décale la valeur de 8 bits à gauche (pour former l'octet haut)
    
    outb(CMD_LOW, PORT_CMD);                        // On veut lire dans le deuxième octet
    pos |= (uint16_t) inb(PORT_DATA);               // On récupère la donnée associée et on reconstruit la position

    return pos;
}

uint16_t console_put_control(const char c, uint16_t pos) {
    uint16_t new_pos = pos;
    uint16_t row = get_row(pos);
    uint16_t col = get_col(pos);
    
    switch (c) {
        case '\b':
            new_pos--;
            break;
        case '\t':
            new_pos += 8;
            break;
        case '\n':
            new_pos = get_index(row + 1, 0);
            break;
        case '\r':
            new_pos = get_index(row, 0);
            break;
        case '\f':
            console_clear(HEADER_HEIGHT);
            new_pos = get_index(HEADER_HEIGHT, 0);
        default:
            break;
    }

    return new_pos;
}

void console_putchar(const char c) {
    uint16_t pos = console_get_cursor();

    // -- traitement du cractère
    if (c > 31 && c < 127) {
        console_display_char(pos, c);
        pos++;
    } else if (is_control_char(c)) {
        pos = console_put_control(c, pos);
    }

    // --- position du curseur
    if (pos >= VGA_SIZE) {
        pos = get_index(HEADER_HEIGHT, 0);
    }

    console_put_cursor(pos);
}

void console_putbytes(const char *s, int len) {
    for (int i = 0; i<len; i++) {
        console_putchar(s[i]);
    }
}

void console_print_at(uint16_t row, uint16_t col, const char *s) {
    uint16_t pos = get_index(row, col);
    int i = 0;
    
    while (s[i] != '\0') {
        console_display_char(pos + i, s[i]);
        i++;
    }
}
