#include <stdio.h>
#include <inttypes.h>
#include <n7OS/keyboard.h>
#include <n7OS/processus.h>


void processus2() {
    while (1) {
        char c = kgetch();

        if (c != 0) {
            printf("%c", c);
        }
    }
}
