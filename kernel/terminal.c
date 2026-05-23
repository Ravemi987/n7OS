#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>
#include <n7OS/processus.h>
#include <n7OS/console.h>

extern void terminal();
extern process_t process_table[NB_PROC];
extern void play_nauru();
void mini_game();


static void commande_ps() {
	printf("%-6s %-15s %-10s\n", "PID", "NOM", "ETAT");
	printf("-----------------------------------\n");
	
	for (int i = 0; i < NB_PROC; i++) {
		// On n'affiche que les cases du tableau qui contiennent un vrai processus
		if (process_table[i].state != LIBRE) {
			char *state_str;
			switch (process_table[i].state) {
				case ELU:    state_str = "ELU"; break;
				case PRET:   state_str = "PRET"; break;
				case BLOQUE: state_str = "BLOQUE"; break;
				default:     state_str = "INCONNU"; break;
			}
			printf("%-6d %-15s %-10s\n", process_table[i].pid, process_table[i].name, state_str);
		}
	}
}


void terminal() {
    printf("\n==================================================\n");
    printf("         BIENVENUE SUR LE MINISHELL N7OS          \n");
    printf("==================================================\n\n");
    
    char cmd[256];

    while(1) {
        printf("root@n7OS$ ");
        
        scanf("%s", cmd);
        
        if (cmd[0] == '\0') {
            continue;
        }

        if (strcmp(cmd, "help") == 0) {
            printf("Commandes disponibles :\n");
            printf("  help     : Affiche ce menu d'aide\n");
            printf("  clear    : Efface l'affichage du terminal\n");
            printf("  pid      : Donne le PID du processus courant\n");
            printf("  ps       : Liste les processus du systeme\n");
            printf("  uptime   : Donne le temps systeme ecoule (ms)\n");
            printf("  sleep3   : Teste l'appel systeme sleep corrige (3s)\n");
            printf("  play     : Lance le mini-jeu en tache de fond\n");
            printf("  shutdown : Eteint l'emulateur QEMU\n");
        }
        else if (strcmp(cmd, "clear") == 0) {
            console_clear(HEADER_HEIGHT);
            console_reset_cursor();
        }
        else if (strcmp(cmd, "pid") == 0) {
            printf("Terminal actif sur le PID : %d\n", get_pid());
        }
        else if (strcmp(cmd, "ps") == 0) {
            commande_ps();
        }
        else if (strcmp(cmd, "uptime") == 0) {
            printf("Uptime : %d ms\n", get_timer());
        }
        else if (strcmp(cmd, "sleep3") == 0) {
            printf("Fermeture des yeux pendant 3 secondes...\n");
            sleep(3);
            printf("Reveil du terminal !\n");
        }
        else if (strcmp(cmd, "play") == 0) {
            printf("Creation du processus de jeu...\n");
			mini_game();
        }
        else if (strcmp(cmd, "shutdown") == 0) {
            printf("Arret de n7OS...\n");
            shutdown(1);
        }
        else {
            printf("n7OS: commande introuvable: '%s'. Tapez 'help'\n", cmd);
        }

        cmd[0] = '\0';
    }
}


void mini_game() {
	printf("Lancement du jeu\n");
	printf("Arret du jeu\n");
	// int child_pid = fork("nauru", play_nauru); 
	
	// if (child_pid > 0) {
	// 	wait(child_pid); 
	// }
}
