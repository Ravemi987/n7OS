#include <stdio.h>
#include <string.h>
#include <n7OS/processus.h>
#include <unistd.h>

#define SIZE 9
#define ABS(x) ((x) < 0 ? -(x) : (x))

int grid[SIZE][SIZE];
int current_player;

void init_nauru() {
    current_player = 1; // 1 = Blanc (o), 2 = Noir (x)
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (r < 3) grid[r][c] = 2;       // Pions noirs en haut
            else if (r > 5) grid[r][c] = 1;  // Pions blancs en bas
            else grid[r][c] = 0;             // Espace vide au milieu
        }
    }
}

void print_nauru_board() {
    int w1 = 0, w2 = 0;
    // Les lettres en haut (Colonnes)
    printf("\n    A B C D E F G H I\n");
    printf("  +-------------------+\n");
    for (int r = 0; r < SIZE; r++) {
        // Les chiffres sur le côté (Lignes)
        printf("%d | ", r + 1); 
        for (int c = 0; c < SIZE; c++) {
            if (grid[r][c] == 1) { printf("o "); w1++; }
            else if (grid[r][c] == 2) { printf("x "); w2++; }
            else printf(". ");
        }
        printf("|\n");
    }
    printf("  +-------------------+\n");
    printf("  Blanc (o): %d  |  Noir (x): %d\n\n", w1, w2);
}

// Vérifie si un coup est valide (Retourne 1: Elimination, 2: Retournement, 3: Libre, 0: Invalide)
int check_move(int sr, int sc, int er, int ec, int player, int mode) {
    if (sr < 0 || sr >= SIZE || sc < 0 || sc >= SIZE || er < 0 || er >= SIZE || ec < 0 || ec >= SIZE) return 0;
    if (sr == er && sc == ec) return 0;
    
    int dr = er - sr;
    int dc = ec - sc;
    // Doit être parfaitement vertical, horizontal ou diagonal
    if (dr != 0 && dc != 0 && ABS(dr) != ABS(dc)) return 0; 
    
    int step_r = (dr > 0) - (dr < 0);
    int step_c = (dc > 0) - (dc < 0);
    int dist = (dr != 0) ? ABS(dr) : ABS(dc);
    int opp = (player == 1) ? 2 : 1;
    
    if (mode == 1) { // 1. Élimination (Atterrir sur l'adversaire)
        if (dist < 2) return 0; // Pas de prise au corps à corps
        if (grid[er][ec] != opp) return 0;
        for (int i = 1; i < dist; i++) {
            if (grid[sr + i * step_r][sc + i * step_c] != 0) return 0;
        }
        return 1;
    } 
    else if (mode == 2) { // Retournement (Atterrir sur vide, adversaire juste derrière)
        if (dist < 2) return 0;
        if (grid[er][ec] != 0) return 0;
        if (grid[er - step_r][ec - step_c] != opp) return 0; // Adversaire avant l'arrivée
        for (int i = 1; i < dist - 1; i++) {
            if (grid[sr + i * step_r][sc + i * step_c] != 0) return 0; // Chemin vide
        }
        return 2;
    } 
    else if (mode == 3) { // Libre (Chemin vide)
        if (grid[er][ec] != 0) return 0;
        for (int i = 1; i < dist; i++) {
            if (grid[sr + i * step_r][sc + i * step_c] != 0) return 0;
        }
        return 3;
    }
    return 0;
}

// Vérifie la règle globale : Déplacement libre interdit si une prise est possible !
int has_any_capture(int player) {
    for (int sr = 0; sr < SIZE; sr++) {
        for (int sc = 0; sc < SIZE; sc++) {
            if (grid[sr][sc] != player) continue;
            // Scan de toutes les destinations possibles sur le plateau
            for (int er = 0; er < SIZE; er++) {
                for (int ec = 0; ec < SIZE; ec++) {
                    if (check_move(sr, sc, er, ec, player, 1) > 0) return 1;
                    if (check_move(sr, sc, er, ec, player, 2) > 0) return 1;
                }
            }
        }
    }
    return 0;
}

void play_nauru() {
    init_nauru();
    
    // INITIALISATION OBLIGATOIRE : {0} remplit le buffer de '\0'.
    // Cela empêche l'écran bleu si scanf laisse la chaîne vide.
    char cmd[50] = {0}; 
    char src[50] = {0}; 
    char dst[50] = {0};

    printf("\n*** DEMARRAGE DU JEU DE NAURU ***\n");

    while(1) {
        print_nauru_board();
        
        int w1 = 0, w2 = 0;
        for(int r = 0; r < SIZE; r++) for(int c = 0; c < SIZE; c++) {
            if(grid[r][c] == 1) w1++;
            if(grid[r][c] == 2) w2++;
        }
        if (w1 < w2 && w1 < 6) { printf(">>> LE JOUEUR 2 (x) GAGNE LA PARTIE ! <<<\n"); break; }
        if (w2 < w1 && w2 < 6) { printf(">>> LE JOUEUR 1 (o) GAGNE LA PARTIE ! <<<\n"); break; }

        printf("========================================\n");
        printf("[ TOUR DU JOUEUR %d : %s ]\n", current_player, current_player == 1 ? "BLANC (o)" : "NOIR (x)");
        printf("========================================\n");
        printf("Type de deplacement :\n");
        printf("1: Elimination | 2: Retournement | 3: Libre | 4: Abandonner\n> ");
        
        // On nettoie la chaîne avant lecture
        cmd[0] = '\0';
        scanf("%s", cmd);
        
        // Sécurité si l'utilisateur appuie juste sur Entrée
        if (cmd[0] == '\0' || cmd[0] == '\n' || cmd[0] == '\r') continue;
        
        int type = cmd[0] - '0';
        if (type == 4) {
            printf("Le Joueur %d abandonne. Victoire du Joueur %d !\n", current_player, 3 - current_player);
            break;
        }
        if (type < 1 || type > 3) continue;

        if (type == 3 && has_any_capture(current_player)) {
            printf("/!\\ Prises possibles sur le plateau ! Deplacement libre interdit.\n");
            continue;
        }

        printf("Case de DEPART (ex: A1) : ");
        src[0] = '\0';
        scanf("%s", src);

        printf("Case d'ARRIVEE (ex: C3) : ");
        dst[0] = '\0';
        scanf("%s", dst);

        if (strlen(src) < 2 || strlen(dst) < 2) {
            printf("Format invalide. Vous devez entrer une Lettre et un Chiffre (ex: A1).\n");
            continue;
        }

        int sr = (src[0] >= 'a') ? src[0] - 'a' : src[0] - 'A';
        int sc = src[1] - '1';                                  
        int er = (dst[0] >= 'a') ? dst[0] - 'a' : dst[0] - 'A';
        int ec = dst[1] - '1';

        if (sr < 0 || sr >= SIZE || sc < 0 || sc >= SIZE || er < 0 || er >= SIZE || ec < 0 || ec >= SIZE) { 
            printf("/!\\ Coordonnees hors du plateau.\n"); 
            continue; 
        }
        if (grid[sr][sc] != current_player) { printf("/!\\ Ce n'est pas votre pion !\n"); continue; }

        int res = check_move(sr, sc, er, ec, current_player, type);
        if (res == 0) { printf("/!\\ Mouvement invalide !\n"); continue; }

        grid[er][ec] = current_player;
        grid[sr][sc] = 0;
        
        if (res == 2) {
            int step_r = ((er - sr) > 0) - ((er - sr) < 0);
            int step_c = ((ec - sc) > 0) - ((ec - sc) < 0);
            grid[er - step_r][ec - step_c] = current_player; 
        }

        // --- ENCHAINEMENT ---
        if (res == 2) {
            while(1) {
                print_nauru_board();
                printf("Enchainer une autre prise par retournement ? (1: oui, 2: non) : ");
                cmd[0] = '\0';
                scanf("%s", cmd);
                if (cmd[0] != '1') break;
                
                // Affichage formaté Ligne(Lettre) Colonne(Chiffre)
                printf("Nouvelle case d'arrivee depuis %c%d : ", 'A' + er, ec + 1);
                dst[0] = '\0';
                scanf("%s", dst);
                
                if (strlen(dst) < 2) { printf("Format invalide.\n"); continue; }

                int n_er = (dst[0] >= 'a') ? dst[0] - 'a' : dst[0] - 'A';
                int n_ec = dst[1] - '1';
                
                if (check_move(er, ec, n_er, n_ec, current_player, 2) == 2) {
                    grid[n_er][n_ec] = current_player;
                    grid[er][ec] = 0;
                    int step_r = ((n_er - er) > 0) - ((n_er - er) < 0);
                    int step_c = ((n_ec - ec) > 0) - ((n_ec - ec) < 0);
                    grid[n_er - step_r][n_ec - step_c] = current_player; 
                    er = n_er; ec = n_ec; 
                } else {
                    printf("/!\\ Enchainement invalide !\n");
                }
            }
        }
        
        current_player = 3 - current_player;
    }

    printf("Fin du jeu de Nauru. Retour au terminal...\n");
    exit();
}
