#include <stdio.h>
#include <n7OS/cpu.h>

extern void terminal();

void terminal() {
	printf("\nLancement du terminal...\n");
	char buf[256];

  	while(1) {

		printf("root$ ");
		scanf("%s", buf);
		printf("Vous avez entre : %s\n", buf);
  	}
}
