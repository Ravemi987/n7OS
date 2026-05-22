#include <stdio.h>
#include <n7OS/cpu.h>

extern void terminal();

void terminal() {
	printf("\nLancement du terminal...\n");
	
  	while(1) {
    	hlt();
  	}
}
