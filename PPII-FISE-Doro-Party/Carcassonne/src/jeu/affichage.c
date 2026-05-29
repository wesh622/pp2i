#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "affichage.h"
#include "pioche.h"
#include "config.h"

/* =========================================================
 *  UTILITAIRES INTERNES
 * ========================================================= */

/* Retourne la couleur ANSI correspondant à un type de face */
static const char* couleur_face(int face) {
    switch (face) {
        case ROUTE_PRAIRIE: return "\033[33m";   /* jaune  – route */
        case PRAIRIE:       return "\033[32m";   /* vert   – prairie */
        case VILLE:         return "\033[34m";   /* bleu   – ville */
        case VILLE_BOUCLIER:return "\033[36m";   /* cyan   – ville+bouclier */
        case ABBAYE:        return "\033[35m";   /* violet – abbaye */
        case CARREFOUR:     return "\033[31m";   /* rouge  – carrefour */
        default:            return "\033[0m";
    }
}

#define RESET "\033[0m"

/* Retourne le symbole (3 cars) d'une face, centré */
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

/* Retourne le caractère unique (pour l'affichage compact du plateau) */
static char lettre_face(int face) {
    switch (face) {
        case ROUTE_PRAIRIE: return 'r';
        case PRAIRIE:       return '.';
        case VILLE:         return 'V';
        case VILLE_BOUCLIER:return 'W';
        case ABBAYE:        return 'A';
        case CARREFOUR:     return 'X';
        default:            return '?';
    }
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

/* =========================================================
 *  AFFICHAGE D'UNE TUILE (vue détaillée 5 lignes)
 *
 *  Exemple :
 *   ┌────────────┐
 *   │    [RTE]   │
 *   │ [PRR][VIL] │   ← Ouest / Centre / Est
 *   │    [VIL]   │
 *   └── id:07 ───┘
 * =========================================================
 *  AFFICHAGE D'UNE TUILE COMPACT (1 cellule dans le plateau)
 *  Format : N/E/S/O dans 4 coins + centre
 *
 *  ╔═══╗
 *  ║.V.║   Nord=Prairie  Est=Ville  Sud=Prairie
 *  ║rVr║   Ouest=Route   Centre=Ville
 *  ║.V.║
 *  ╚═══╝
 * ========================================================= */

/* Affichage compact d'une tuile seule (3 lignes, utile pour la tuile piochée) */
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
    printf("    %s%c%s %s%c%s %s%c%s\n", cO,o,RESET, cC,c,RESET, cE,e,RESET);
    printf("      %s%c%s\n", cS, s, RESET);
    printf("    id: %d\n", t.id);
}

/* =========================================================
 *  AFFICHAGE DU PLATEAU (CLI)
 *  Chaque case occupée est représentée par ses 4 faces + centre
 *  Les cases vides sont grises
 *  Les cases où on PEUT poser sont vertes
 * ========================================================= */
/* Version améliorée avec couleurs et format compact */
void afficher_plateau_cli_ameliore(Plateau *p) {
    int centre = TAILLE_MAX / 2;
    int rayon  = 5;

    printf("\n\033[1m=== PLATEAU ===\033[0m\n");

    /* En-tête colonnes */
    printf("     ");
    for (int j = centre - rayon; j <= centre + rayon; j++) {
        printf(" %3d ", j);
    }
    printf("\n");

    for (int i = centre - rayon; i <= centre + rayon; i++) {
        /* Ligne de la rangée */
        printf("%3d  ", i);
        for (int j = centre - rayon; j <= centre + rayon; j++) {
            if (p->occupes[i][j]) {
                Tuiles t = p->grille[i][j];
                /* Cellule compacte : Nord/Est/Sud/Ouest/Centre en 1 char coloré */
                printf(" %s%c%s%s%c%s%s%c%s ",
                    couleur_face(t.a), lettre_face(t.a), RESET,
                    couleur_face(t.center), lettre_face(t.center), RESET,
                    couleur_face(t.c), lettre_face(t.c), RESET);
            } else {
                printf("\033[90m  ·  \033[0m");  /* gris – vide */
            }
        }
        printf("\n");
    }
    printf("\n");

    /* Légende */
    printf("Légende : "
           "%s.%s=Prairie  "
           "%sr%s=Route  "
           "%sV%s=Ville  "
           "%sW%s=VilleBouclier  "
           "%sA%s=Abbaye  "
           "%sX%s=Carrefour\n\n",
        couleur_face(PRAIRIE),       RESET,
        couleur_face(ROUTE_PRAIRIE), RESET,
        couleur_face(VILLE),         RESET,
        couleur_face(VILLE_BOUCLIER),RESET,
        couleur_face(ABBAYE),        RESET,
        couleur_face(CARREFOUR),     RESET);
}

/* =========================================================
 *  AFFICHAGE DU TOUR AVEC COULEUR
 * ========================================================= */
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

/* =========================================================
 *  AFFICHAGE DE LA CONFIGURATION
 * ========================================================= */
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

/* =========================================================
 *  AFFICHAGE DU NOMBRE DE TUILES RESTANTES
 * ========================================================= */
void afficher_pioche(Pioche *pioche) {
    int restantes = pioche->nbresrestantes;
    int total = 71;
    int ratio  = (restantes * 20) / total;  /* barre sur 20 chars */

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

/* =========================================================
 *  MENU D'OPTIONS EN COURS DE JEU
 *  Retourne le choix du joueur
 * ========================================================= */
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

/* =========================================================
 *  AFFICHAGE DE LA TUILE PIOCHÉE (avec détails)
 * ========================================================= */
void afficher_tuile_piochee(Tuiles t) {
    printf("\n\033[1m┌─ TUILE PIOCHÉE ───────────────────────┐\033[0m\n");
    printf("\033[1m│\033[0m\n");

    /* Rendu ASCII complet */
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

/* =========================================================
 *  AFFICHAGE DU RÉSULTAT FINAL
 * ========================================================= */
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
            printf("\033[1;32m║   %-14s  %4d pts ║\033[0m\n",
                   joueurs[i].nom, joueurs[i].score);
        else
            printf("║    %-14s  %4d pts ║\n",
                   joueurs[i].nom, joueurs[i].score);
    }

    printf("\033[1;33m╚══════════════════════════════════╝\033[0m\n");
    printf("\033[1;32m  Félicitations à %s !\033[0m\n\n", joueurs[gagnant].nom);
}
