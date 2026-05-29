#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "affichage.h"
#include "pioche.h"
#include "config.h"


// Couleurs des joueurs (fonds colorés pour les meeples) 
static const char* couleur_fond_joueur(int idjoueur) {
    switch (idjoueur) {
        case 1: return "\033[41m";   /* rouge     – joueur 1 */
        case 2: return "\033[42m";   /* vert      – joueur 2 */
        case 3: return "\033[44m";   /* bleu      – joueur 3 */
        case 4: return "\033[43m";   /* jaune     – joueur 4 */
        case 5: return "\033[45m";   /* jaune     – joueur 4 */
        default: return "\033[0m";
    }
}

// Retourne la couleur ANSI correspondant à un type de face 
static const char* couleur_face(int face) {
    switch (face) {
        case ROUTE_PRAIRIE: return "\033[33m";   // jaune  – route 
        case PRAIRIE:       return "\033[32m";   // vert   – prairie 
        case VILLE:         return "\033[34m";   // bleu   – ville 
        case VILLE_BOUCLIER:return "\033[36m";   // cyan   – ville+bouclier 
        case ABBAYE:        return "\033[35m";   // violet – abbaye 
        case CARREFOUR:     return "\033[31m";   // rouge  – carrefour 
        default:            return "\033[0m";
    }
}

#define RESET " \033[0m"

// Retourne le symbole (3 cars) d'une face, centré 
static const char* sym_face(int face) {
    switch (face) {
        case ROUTE_PRAIRIE: return "RTE";
        case PRAIRIE:       return "PRR";
        case VILLE:         return "VIL";
        case VILLE_BOUCLIER:return "VB!";
        case ABBAYE:        return "ABB";
        case CARREFOUR:     return "CRF";
        default:            return " ? ";
    }
}

static char lettre_face(int face) {
    switch (face) {
        case ROUTE_PRAIRIE: return 'r';
        case PRAIRIE:       return 'p';
        case VILLE:         return 'V';
        case VILLE_BOUCLIER:return 'W';
        case ABBAYE:        return 'A';
        case CARREFOUR:     return 'X';
        default:            return '?';
    }
}

// Retourne la couleur de fond d'un meeple si présent à la position et emplacement donnés, sinon retourne NULL. Les joueurs sont passés en paramètre pour parcourir leurs meeples 
static const char* find_meeple_color(Joueur joueurs[], int nb_joueurs, int posX, int posY, int emplacement) {
    for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].actif == 0) continue;
        
        for (int m = 0; m < 7; m++) {
            Meeple mep = joueurs[i].stock[m];
            if (mep.etat == 0 && mep.posX == posX && mep.posY == posY && mep.emplacement == emplacement) {
                return couleur_fond_joueur(mep.idjoueur);
            }
        }
    }
    return NULL;
}


void afficher_plateau_cli(Plateau *p) {
    printf("\n=== PLATEAU ===\n");
    for (int i = 0; i < TAILLE_MAX; i++) {
        for (int j = 0; j < TAILLE_MAX; j++) {
            if (p->occupes[i][j]) {
                printf("[%2d]", p->grille[i][j].id);
            } else {
                printf(" .. ");
            }
        }
        printf("\n");
    }
    printf("===============\n\n");
}

//Affichage compact d'une tuile seule (3 lignes, utile pour la tuile piochée) 
void afficher_tuile_compacte(Tuiles t) {
    char n = lettre_face(t.a);
    char e = lettre_face(t.b);
    char s = lettre_face(t.c);
    char o = lettre_face(t.d);
    char c = lettre_face(t.center);

    const char* cN = couleur_face(t.a);
    const char* cE = couleur_face(t.b);
    const char* cS = couleur_face(t.c);
    const char* cO = couleur_face(t.d);
    const char* cC = couleur_face(t.center);

    printf("      %s%c%s\n", cN, n, RESET);
    printf("    %s%c%s%s%c%s%s%c%s\n", cO,o,RESET, cC,c,RESET, cE,e,RESET);
    printf("      %s%c%s\n", cS, s, RESET);
    printf("    id: %d\n", t.id);
}

//  AFFICHAGE DU PLATEAU (CLI)
void afficher_plateau_cli_ameliore(Plateau *p, Joueur joueurs[], int nb_joueurs) {
    int centre = TAILLE_MAX / 2;
    int rayon  = 5;

    printf("\n\033[1m=== PLATEAU ===\033[0m\n");

    /* En-tête colonnes */
    printf("    ");
    for (int j = centre - rayon; j <= centre + rayon; j++) {
        printf(" %3d   ", j);
    }
    printf("\n");


    for (int i = centre - rayon; i <= centre + rayon; i++) {
        // Affichage sur 3 sous-lignes : nord, centre, sud
        // Ligne 1 : Nord
        printf("    ");
        for (int j = centre - rayon; j <= centre + rayon; j++) {
            if (p->occupes[i][j]) {
                Tuiles t = p->grille[i][j];
                const char* meeple_color = find_meeple_color(joueurs, nb_joueurs, i, j, 1);
                if (meeple_color) {
                    printf("  %s%s%c\033[0m%s  ", meeple_color, couleur_face(t.a), lettre_face(t.a), RESET);
                } else {
                    printf("  %s%c%s  ", couleur_face(t.a), lettre_face(t.a), RESET);
                }
                printf(" ");
            } else {
                printf("       ");
            }
        }
        printf("\n");
        // Ligne 2 : Ouest Centre Est
        printf("%3d ", i);
        for (int j = centre - rayon; j <= centre + rayon; j++) {
            if (p->occupes[i][j]) {
                Tuiles t = p->grille[i][j];
                
                const char* meeple_color_o = find_meeple_color(joueurs, nb_joueurs, i, j, 4);
                const char* meeple_color_c = find_meeple_color(joueurs, nb_joueurs, i, j, 5);
                const char* meeple_color_e = find_meeple_color(joueurs, nb_joueurs, i, j, 2);
                
                if (meeple_color_o) {
                    printf("%s%s%c\033[0m%s", meeple_color_o, couleur_face(t.d), lettre_face(t.d), RESET);
                } else {
                    printf("%s%c%s", couleur_face(t.d), lettre_face(t.d), RESET);
                }
                
                if (meeple_color_c) {
                    printf("%s%s%c\033[0m%s", meeple_color_c, couleur_face(t.center), lettre_face(t.center), RESET);
                } else {
                    printf("%s%c%s", couleur_face(t.center), lettre_face(t.center), RESET);
                }
                
                if (meeple_color_e) {
                    printf("%s%s%c\033[0m%s", meeple_color_e, couleur_face(t.b), lettre_face(t.b), RESET);
                } else {
                    printf("%s%c%s", couleur_face(t.b), lettre_face(t.b), RESET);
                }
                printf(" ");
            } else {
                printf("\033[90m  ·  \033[0m");
                printf("  ");
            }
        }
        printf("\n    ");
        // Ligne 3 : Sud
        for (int j = centre - rayon; j <= centre + rayon; j++) {
            if (p->occupes[i][j]) {
                Tuiles t = p->grille[i][j];
                const char* meeple_color = find_meeple_color(joueurs, nb_joueurs, i, j, 3);
                if (meeple_color) {
                    printf("  %s%s%c\033[0m%s  ", meeple_color, couleur_face(t.c), lettre_face(t.c), RESET);
                } else {
                    printf("  %s%c%s  ", couleur_face(t.c), lettre_face(t.c), RESET);
                }
                printf(" ");
            } else {
                printf("       ");
            }
        }
        printf("\n");
    }
    printf("\n");

    /* Légende */
    printf("Légende : "
           "%sp%s=Prairie  "
           "%sr%s=Route  "
           "%sV%s=Ville  "
           "%sW%s=VilleBouclier  "
           "%sA%s=Abbaye  "
           "%sX%s=Carrefour  \n"
           "\033[41m \033[0m=Joueur1(Rouge)  "
           "\033[42m \033[0m=Joueur2(Vert)  "
           "\033[44m \033[0m=Joueur3(Bleu)  "
           "\033[43m \033[0m=Joueur4(Jaune)  "
           "\033[45m \033[0m=Joueur5(Violet))\n\n",
        couleur_face(PRAIRIE),       RESET,
        couleur_face(ROUTE_PRAIRIE), RESET,
        couleur_face(VILLE),         RESET,
        couleur_face(VILLE_BOUCLIER),RESET,
        couleur_face(ABBAYE),        RESET,
        couleur_face(CARREFOUR),     RESET);
}

void afficher_plateau_cli_ameliore_pour_placer_tuile(Plateau *p, Tuiles *tuile_param, Joueur joueurs[], int nb_joueurs) {
    int centre = TAILLE_MAX / 2;
    int rayon  = 5;
    printf("\n\033[1m=== PLATEAU ===\033[0m\n");

    
    printf("    ");
    for (int j = centre - rayon; j <= centre + rayon; j++) {
        printf(" %3d   ", j);
    }
    printf("\n");


    for (int i = centre - rayon; i <= centre + rayon; i++) {
        printf("    ");
        for (int j = centre - rayon; j <= centre + rayon; j++) {
            if (p->occupes[i][j]) {
                Tuiles t = p->grille[i][j];
                const char* meeple_color = find_meeple_color(joueurs, nb_joueurs, i, j, 1);
                if (meeple_color) {
                    printf("  ");
                    printf("%s%c\033[0m  ", meeple_color, lettre_face(t.a));
                    printf(" ");
                } else {
                    printf("  %s%c%s  ", couleur_face(t.a), lettre_face(t.a), RESET);
                }
                printf(" ");
            } else {
                printf("       ");
            }
        }
        printf("\n");
        printf("%3d ", i);
        for (int j = centre - rayon; j <= centre + rayon; j++) {
            if (p->occupes[i][j]) {
                Tuiles t = p->grille[i][j];
                
                const char* meeple_color_o = find_meeple_color(joueurs, nb_joueurs, i, j, 4);
                const char* meeple_color_c = find_meeple_color(joueurs, nb_joueurs, i, j, 5);
                const char* meeple_color_e = find_meeple_color(joueurs, nb_joueurs, i, j, 2);
                
                if (meeple_color_o) {
                    printf("%s%c\033[0m", meeple_color_o, lettre_face(t.d));
                    printf(" ");
                } else {
                    printf("%s%c%s", couleur_face(t.d), lettre_face(t.d), RESET);
                }
                
                if (meeple_color_c) {
                    printf("%s%c\033[0m", meeple_color_c, lettre_face(t.center));
                    printf(" ");
                } else {
                    printf("%s%c%s", couleur_face(t.center), lettre_face(t.center), RESET);
                }
                
                if (meeple_color_e) {
                    printf("%s%c\033[0m", meeple_color_e, lettre_face(t.b));
                    printf(" ");
                } else {
                    printf("%s%c%s", couleur_face(t.b), lettre_face(t.b), RESET);
                }
                printf(" ");
            } else {
                if (peut_poser_tuile_silent(p, tuile_param, i, j) == 1) {
                    printf("\033[32m  ·  \033[0m");
                    printf("  ");

                } else {
                    printf("\033[31m  ·  \033[0m");
                    printf("  ");
                }
            }
        }
        printf("\n    ");

        for (int j = centre - rayon; j <= centre + rayon; j++) {
            if (p->occupes[i][j]) {
                Tuiles t = p->grille[i][j];
                const char* meeple_color = find_meeple_color(joueurs, nb_joueurs, i, j, 3);
                if (meeple_color) {
                    printf("  ");
                    printf("%s%c\033[0m  ", meeple_color, lettre_face(t.c));
                    printf(" ");
                } else {
                    printf("  %s%c%s  ", couleur_face(t.c), lettre_face(t.c), RESET);
                }
                printf(" ");
            } else {
                printf("       ");
            }
        }
        printf("\n");
    }
    printf("\n");

    // Légende
    printf("Légende : "
            "\033[32m·\033[0m=Peut placer  "
           "\033[31m·\033[0m=Impossible  "
           "%sp%s=Prairie  "
           "%sr%s=Route  "
           "%sV%s=Ville  "
           "%sW%s=VilleBouclier  "
           "%sA%s=Abbaye  "
           "%sX%s=Carrefour  \n"
           "\033[41m \033[0m=Joueur1(Rouge)  "
           "\033[42m \033[0m=Joueur2(Vert)  "
           "\033[44m \033[0m=Joueur3(Bleu)  "
           "\033[43m \033[0m=Joueur4(Jaune)  "
           "\033[45m \033[0m=Joueur5(Violet))\n\n",
        couleur_face(PRAIRIE),       RESET,
        couleur_face(ROUTE_PRAIRIE), RESET,
        couleur_face(VILLE),         RESET,
        couleur_face(VILLE_BOUCLIER),RESET,
        couleur_face(ABBAYE),        RESET,
        couleur_face(CARREFOUR),     RESET);
}

// affichage couleur
void afficher_tour_ameliore(int numero_tour, Joueur *j) {
    printf("\n\033[1;33m╔══════════════════════════════╗\033[0m\n");
    printf("\033[1;33m║      TOUR %-3d                 ║\033[0m\n", numero_tour);
    printf("\033[1;33m╠══════════════════════════════╣\033[0m\n");
    printf("\033[1;33m║\033[0m Joueur : \033[1m%-20s\033[0m\033[1;33m║\033[0m\n", j->nom);
    printf("\033[1;33m║\033[0m Score  : \033[1;32m%-4d pts\033[0m               \033[1;33m║\033[0m\n", j->score);

    /* Meeples disponibles */
    int dispo = 0;
    for (int i = 0; i < 7; i++) {
        if (j->stock[i].etat == 1) dispo++;
    }
    printf("\033[1;33m║\033[0m Meeples: ");
    for (int i = 0; i < 7; i++) {
        if (j->stock[i].etat == 1)
            printf("\033[35m●\033[0m");
        else
            printf("\033[90m○\033[0m");
    }
    printf("  (%d/7)  \033[1;33m║\033[0m\n", dispo);
    printf("\033[1;33m╚══════════════════════════════╝\033[0m\n\n");
}

// affichage 2
void afficher_config(config *conf, int nb_joueurs_total) {
    printf("\n\033[1m╔══════════════════════════════════╗\033[0m\n");
    printf("\033[1m║         CONFIGURATION            ║\033[0m\n");
    printf("\033[1m╠══════════════════════════════════╣\033[0m\n");
    printf("\033[1m║\033[0m Mode       : %-20s\033[1m║\033[0m\n",
           conf->mode == 1 ? "SDL (graphique)" : "CLI (texte)");
    printf("\033[1m║\033[0m Joueurs    : %-20d\033[1m║\033[0m\n", conf->nbr_joueur);
    printf("\033[1m║\033[0m IAs        : %-20d\033[1m║\033[0m\n", conf->ai);
    printf("\033[1m║\033[0m Seed       : %-20d\033[1m║\033[0m\n", conf->seed);
    if (conf->max_turn < 0)
        printf("\033[1m║\033[0m Tours max  : %-20s\033[1m║\033[0m\n", "illimité");
    else
        printf("\033[1m║\033[0m Tours max  : %-20d\033[1m║\033[0m\n", conf->max_turn);
    printf("\033[1m╠══════════════════════════════════╣\033[0m\n");
    printf("\033[1m║\033[0m Participants :\033[1m                   ║\033[0m\n");
    for (int i = 0; i < nb_joueurs_total; i++) {
        if (!conf->tab[i].actif) continue;
        int est_ia = (i >= conf->nbr_joueur);
        printf("\033[1m║\033[0m   [%d] %-14s  %s\033[1m       ║\033[0m\n",
               i + 1, conf->tab[i].nom,
               est_ia ? "\033[36mIA\033[0m" : "\033[32m👤\033[0m");
    }
    printf("\033[1m╚══════════════════════════════════╝\033[0m\n\n");
}

//afficher tuile restantes dans la pioche
void afficher_pioche(Pioche *pioche) {
    int restantes = pioche->nbresrestantes;
    int total = 71;
    int ratio  = (restantes * 20) / total; 

    printf("\n\033[1m┌─ PIOCHE ──────────────────────┐\033[0m\n");
    printf("\033[1m│\033[0m  [");
    for (int i = 0; i < 20; i++) {
        if (i < ratio)
            printf("\033[32m█\033[0m");
        else
            printf("\033[90m░\033[0m");
    }
    printf("] %2d/71  \033[1m│\033[0m\n", restantes);
    printf("\033[1m└───────────────────────────────┘\033[0m\n\n");
}

// menu et options au fur et à mesure du jeu
int afficher_menu_options(void) {
    printf("\n\033[1m── Options ─────────────────────────────────\033[0m\n");
    printf("  \033[33m[1]\033[0m Poser la tuile\n");
    printf("  \033[33m[2]\033[0m Afficher les scores\n");
    printf("  \033[33m[3]\033[0m Voir les tuiles restantes dans la pioche\n");
    printf("  \033[33m[4]\033[0m Voir la configuration de la partie\n");
    printf("  \033[33m[5]\033[0m Tourner la tuile à droite  (⟳)\n");
    printf("  \033[33m[6]\033[0m Tourner la tuile à gauche  (⟲)\n");
    printf("  \033[33m[7]\033[0m Réafficher le plateau\n");
    printf("  \033[33m[0]\033[0m Quitter la partie\n");
    printf("\033[1m────────────────────────────────────────────\033[0m\n");
    printf("Votre choix : ");

    int choix = 0;
    scanf("%d", &choix);
    return choix;
}

// affichage de la tuile pioché 
void afficher_tuile_piochee(Tuiles t) {
    printf("\n\033[1m┌─ TUILE PIOCHÉE ───────────────────────┐\033[0m\n");
    printf("\033[1m│\033[0m\n");
    printf("\033[1m│\033[0m       Nord: %s%-3s%s\n",
           couleur_face(t.a), sym_face(t.a), RESET);
    printf("\033[1m│\033[0m  %-3s  [%s%-3s%s]  %-3s\n",
           sym_face(t.d),
           couleur_face(t.center), sym_face(t.center), RESET,
           sym_face(t.b));
    printf("\033[1m│\033[0m  Ouest Centre  Est\n");
    printf("\033[1m│\033[0m       Sud:  %s%-3s%s\n",
           couleur_face(t.c), sym_face(t.c), RESET);
    printf("\033[1m│\033[0m\n");
    printf("\033[1m│\033[0m  ID: %d\n", t.id);
    printf("\033[1m└───────────────────────────────────────┘\033[0m\n\n");
}

// affichage de fin de partie et scores finaux
void afficher_fin_de_partie(Joueur joueurs[], int nb_joueurs) {
    printf("\n\033[1;33m");
    printf("╔══════════════════════════════════╗\n");
    printf("║       FIN DE PARTIE              ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("\033[0m");

    int max_score = -1;
    int gagnant   = 0;
    for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].actif && joueurs[i].score > max_score) {
            max_score = joueurs[i].score;
            gagnant   = i;
        }
    }

    for (int i = 0; i < nb_joueurs; i++) {
        if (!joueurs[i].actif) continue;
        if (i == gagnant)
            printf("\033[1;32m║    %-14s  %4d pts ║\033[0m\n",
                   joueurs[i].nom, joueurs[i].score);
        else
            printf("║    %-14s  %4d pts ║\n",
                   joueurs[i].nom, joueurs[i].score);
    }

    printf("\033[1;33m╚══════════════════════════════════╝\033[0m\n");
    printf("\033[1;32m  Félicitations à %s !\033[0m\n\n", joueurs[gagnant].nom);
}
