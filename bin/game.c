#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <n7OS/processus.h>
#include <n7OS/time.h>
#include <unistd.h>

static unsigned int next_seed = 1337;

int my_rand() {
    next_seed = next_seed * 1103515245 + 12345;
    return (unsigned int)(next_seed / 65536) % 32768;
}

void print_equation(int round, int a, char op1, int b, char op2, int c, int res, int hidden_pos, int pattern) {
    printf("[Round %d/10]  ", round);
    
    if (pattern == 0) printf("(");
    
    if (hidden_pos == 0) printf("?"); else printf("%d", a);
    printf(" %c ", op1);
    
    if (pattern == 1) printf("(");
    
    if (hidden_pos == 1) printf("?"); else printf("%d", b);
    
    if (pattern == 0) printf(")");
    printf(" %c ", op2);
    
    if (hidden_pos == 2) printf("?"); else printf("%d", c);
    
    if (pattern == 1) printf(")");
    
    printf(" = ");
    if (hidden_pos == 3) printf("?\n"); else printf("%d\n", res);
}

void play_game() {
    char input[16];
    int a, b, c, intermediate, final_res;
    int op1, op2; 
    int hidden_pos;
    int expected_answer, user_answer;
    
    unsigned long start_time, end_time, elapsed_seconds;
    int score = 0;
    int round = 1;
    int pattern;

    next_seed = (unsigned int)get_timer();

    printf("\n==================================================\n");
    printf("        CHALLENGE DE CALCUL MENTAL                \n");
    printf("==================================================\n");
    printf(" - Equations a completer.                          \n");
    printf(" - Le '?' peut etre n'importe ou !                \n");
    printf(" - Plus vous repondez vite, plus vous gagnez de points.\n");
    printf(" - Une mauvaise reponse vous penalise !           \n");
    printf(" - Limite de 10 questions.                        \n");
    printf(" Tapez 'q' pour quitter la partie.                \n");
    printf("==================================================\n\n");

    while (round <= 10) {
        op1 = my_rand() % 3;
        op2 = my_rand() % 3;

        a = (my_rand() % 12) + 2;
        b = (my_rand() % 10) + 2;
        c = (my_rand() % 10) + 2;

        if (op1 == 2 && op2 != 2) {
            pattern = 0;
            intermediate = a * b;
            if (op2 == 1 && intermediate < c) op2 = 0; 
            final_res = (op2 == 0) ? (intermediate + c) : (intermediate - c);
        } 
        else if (op1 != 2 && op2 == 2) {
            pattern = 1;
            intermediate = b * c;
            if (op1 == 1 && a < intermediate) {
                a = intermediate + (my_rand() % 10) + 1; 
            }
            final_res = (op1 == 0) ? (a + intermediate) : (a - intermediate);
        } 
        else {
            pattern = 2;
            if (op1 == 0) {
                intermediate = a + b;
            } else if (op1 == 1) {
                if (a < b) { int tmp = a; a = b; b = tmp; } 
                intermediate = a - b;
            } else {
                intermediate = a * b;
            }

            if (op2 == 0) {
                final_res = intermediate + c;
            } else if (op2 == 1) {
                if (intermediate < c) op2 = 0; 
                final_res = (op2 == 0) ? (intermediate + c) : (intermediate - c);
            } else {
                final_res = intermediate * c;
            }
        }

        hidden_pos = my_rand() % 4;
        if (hidden_pos == 0) expected_answer = a;
        else if (hidden_pos == 1) expected_answer = b;
        else if (hidden_pos == 2) expected_answer = c;
        else expected_answer = final_res;

        char c_op1 = (op1 == 0) ? '+' : (op1 == 1) ? '-' : '*';
        char c_op2 = (op2 == 0) ? '+' : (op2 == 1) ? '-' : '*';

        print_equation(round, a, c_op1, b, c_op2, c, final_res, hidden_pos, pattern);

        start_time = get_timer();

        printf("Votre reponse : ");
        memset(input, 0, 16);
        scanf("%s", input);

        end_time = get_timer();
        elapsed_seconds = (int)((end_time - start_time) / 1000); 

        if (input[0] == '\0' || input[0] == '\n' || input[0] == '\r') {
            continue;
        }

        if (input[0] == 'q' || input[0] == 'Q') {
            break;
        }

        user_answer = atoi(input);

        if (user_answer == expected_answer) {
            int points_gained = 1;
            if (elapsed_seconds <= 3) {
                points_gained = 5;
                printf("   >>> EXCELLENT ! En seulement %d secondes ! (+5 pts)\n\n", (int)elapsed_seconds);
            } else if (elapsed_seconds <= 6) {
                points_gained = 3;
                printf("   >>> BIEN JOUE ! Repondu en %d secondes. (+3 pts)\n\n", (int)elapsed_seconds);
            } else {
                printf("   >>> CORRECT ! Mais un peu lent (%d secondes). (+1 pt)\n\n", (int)elapsed_seconds);
            }
            score += points_gained;
        } else {
            printf("   >>> FAUX ! La bonne reponse etait : %d (-2 pts)\n\n", expected_answer);
            score -= 2;
            if (score < 0) score = 0;
        }

        round++;
    }

    printf("\n==================================================\n");
    printf("=> FIN DE LA PARTIE !\n");
    printf("=> Score final cumule : %d Points.\n", score);
    printf("==================================================\n\n");

    printf("Retour au terminal...\n");
    exit();
}
