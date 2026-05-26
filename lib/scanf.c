#include <stdio.h>
#include <string.h>
#include <n7OS/keyboard.h>
#include <n7OS/console.h>
#include <ctype.h>

#define STRSIZE 255

char getchar(void) {
    char c;
    
    // kgetch() est déjà bloquante (elle attend une touche) donc on la récupère directement.
    c = kgetch();
    
    // Normalisation de la touche Entrée
    if (c == '\r') {
        c = '\n';
    }
    
    // On affiche
    console_putchar(c);
    
    return c;
}

char *gets(char *s) {
    char c;
    int count = 0;
    
    while (count < STRSIZE - 1) {
        c = getchar();   
        
        if (c == '\n' || c == '\r') {
            break; // Fin de la ligne, on sort de la boucle
        } 
        else if (c == '\b') {
            // Si on efface, on recule uniquement si on a écrit quelque chose
            if (count > 0) {
                count--;
                s[count] = '\0'; // On efface directement dans 's'
            }
        } 
        else {
            // Pour tous les autres caractères normaux, on les stocke dans 's'
            s[count++] = c;
        }
    }

    s[count] = '\0';

    return s;
}

int scanf(const char *format, ...) {
    va_list scan;
    char input[STRSIZE];
    int count = 0;

    va_start(scan, format);

    for (; *format; format++) {
        if (*format == '%') {
            format++; // On passe le '%'

            char maxchars[5] = {0};
            int i = 0, nmax = 0;
            
            while (isdigit(*format)) {
                maxchars[i++] = *format;
                format++;
            }
            maxchars[i] = '\0';
            if (i > 0) nmax = atoi(maxchars);

            // On demande à l'utilisateur de taper sa ligne
            gets(input);
            count += strlen(input);

            switch (*format) {
                case 's': {
                    char *s_ptr = va_arg(scan, char *);
                    int len = strlen(input);
                    
                    if (nmax > 0 && len > nmax) {
                        len = nmax;
                    }
                    
                    strncpy(s_ptr, input, len);
                    s_ptr[len] = '\0'; // On doit fermer la chaîne
                    break;
                }
                case 'd': {
                    int *i_ptr = va_arg(scan, int *);
                    
                    if (nmax != 0 && strlen(input) > nmax) {
                        input[nmax] = '\0';
                    }
                    
                    *i_ptr = atoi(input);
                    break;
                }
            }
        }
    }
    va_end(scan);
    return count;
}
