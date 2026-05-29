#include <stdio.h>
#include "score.h"
#include "joueur.h"
#include "tuiles.h"
#include "plateau.h"

//initialisation du score
void init_scores(Joueur joueurs[], int nb_joueurs) {
    for (int i = 0; i < nb_joueurs; i++) {
        joueurs[i].score = 0;
    }
}

//bah ajouter au score
void ajouter_score(Joueur joueurs[], int index_joueur, int points) {
    if (index_joueur < 0 || index_joueur >= NB_JOUEURS_MAX) return;
    if (!joueurs[index_joueur].actif) return;
    joueurs[index_joueur].score += points;
}

//bah afficher quoi
void afficher_scores(Joueur joueurs[], int nb_joueurs) {
    printf("\n=== SCORES ===\n");
    for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].actif) {
            printf("  %s : %d pts\n", joueurs[i].nom, joueurs[i].score);
        }
    }
    printf("==============\n\n");
}

// donne le gagnant en comparant sur les score des joueur
int joueur_gagnant(Joueur joueurs[], int nb_joueurs) {
    int max = -1;
    int index = 0;
    for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].actif && joueurs[i].score > max) {
            max = joueurs[i].score;
            index = i;
        }
    }
    return index;
}

int compter_points_ville(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]) {
    if (x < 0 || x >= TAILLE_MAX || y < 0 || y >= TAILLE_MAX || p->occupes[x][y] == 0 || visite[x][y]) {
        return 0;
    }

    visite[x][y] = 1;
    Tuiles t = p->grille[x][y];

    // 1 pt de base + 1 si blason (le x2 pour ville complete est fait a l'appel)
    int pts = 1;
    if (t.a == VILLE_BOUCLIER || t.b == VILLE_BOUCLIER || t.c == VILLE_BOUCLIER || t.d == VILLE_BOUCLIER) {
        pts += 1;
    }

    // On explore les 4 directions
    if (t.a == VILLE || t.a == VILLE_BOUCLIER) {
        pts += compter_points_ville(p, x, y + 1, visite);
    }

    if (t.b == VILLE || t.b == VILLE_BOUCLIER) {
        pts += compter_points_ville(p, x + 1, y, visite);
    }

    if (t.c == VILLE || t.c == VILLE_BOUCLIER) {
        pts += compter_points_ville(p, x, y - 1, visite);
    }

    if (t.d == VILLE || t.d == VILLE_BOUCLIER) {
        pts += compter_points_ville(p, x - 1, y, visite);
    }

    return pts;
}

int compter_points_route(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]) {
    if (x < 0 || x >= TAILLE_MAX || y < 0 || y >= TAILLE_MAX || p->occupes[x][y] == 0) {
        return 0;
    }
    if (visite[x][y] == 1) {
        return 0;
    }  // Déjà compté

    visite[x][y] = 1;
    Tuiles t = p->grille[x][y];
    int pts = 1; // 1 point par tuile route

    if (t.a == ROUTE_PRAIRIE) {
        pts += compter_points_route(p, x, y + 1, visite);
    }
    if (t.b == ROUTE_PRAIRIE) {
        pts += compter_points_route(p, x + 1, y, visite);
    }
    if (t.c == ROUTE_PRAIRIE) {
        pts += compter_points_route(p, x, y - 1, visite);
    }
    if (t.d == ROUTE_PRAIRIE) {
        pts += compter_points_route(p, x - 1, y, visite);
    }

    return pts;
}

void attribuer_points_et_recuperer_meeples(int visite[TAILLE_MAX][TAILLE_MAX], int points, Joueur joueurs[], int nb_joueurs) {
    int meeples_par_joueur[NB_JOUEURS_MAX] = { 0 };

    // compter les meeple de chaque joueurs sur la structure
    for (int i = 0; i < nb_joueurs; i++) {
        if (!joueurs[i].actif) {
            continue;
        }
        for (int m = 0; m < 7; m++) {
            if (joueurs[i].stock[m].etat == 0) { // Si posé
                int mx = joueurs[i].stock[m].posX;
                int my = joueurs[i].stock[m].posY;
                if (mx >= 0 && my >= 0 && visite[mx][my]) {
                    meeples_par_joueur[i]++;
                }
            }
        }
    }

    // trouver le max
    int max_meeples = 0;
    for (int i = 0; i < nb_joueurs; i++) {
        if (meeples_par_joueur[i] > max_meeples) {
            max_meeples = meeples_par_joueur[i];
        }
    }

    // donner les point aux vainqueur et recuperer les meeple
    if (max_meeples > 0) {
        for (int i = 0; i < nb_joueurs; i++) {
            if (!joueurs[i].actif) {
                continue;
            }

            if (meeples_par_joueur[i] == max_meeples) {
                joueurs[i].score += points;
            }

            // Récupération des meeples pour TOUS les joueurs présents dans la zone
            for (int m = 0; m < 7; m++) {
                if (joueurs[i].stock[m].etat == 0) {
                    int mx = joueurs[i].stock[m].posX;
                    int my = joueurs[i].stock[m].posY;
                    if (mx >= 0 && my >= 0 && visite[mx][my]) {
                        joueurs[i].stock[m].etat = 1;
                        joueurs[i].stock[m].posX = -1;
                        joueurs[i].stock[m].posY = -1;
                    }
                }
            }
        }
    }
}

void verifier_et_scorer_structures(Plateau* p, int x, int y, Joueur joueurs[], int nb_joueurs) {
    Tuiles t = p->grille[x][y];

    // 1. Vérification Abbaye : poser une tuile peut terminer une abbaye ADJACENTE
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && i < TAILLE_MAX && j >= 0 && j < TAILLE_MAX) {
                if (p->occupes[i][j] && p->grille[i][j].center == ABBAYE) {
                    if (verifier_abbaye_complete(p, i, j) == 1) {
                        int visite[TAILLE_MAX][TAILLE_MAX] = {0};
                        visite[i][j] = 1; // L'abbaye est sur une seule case
                        attribuer_points_et_recuperer_meeples(visite, 9, joueurs, nb_joueurs);
                        printf("L'abbaye en (%d,%d) est terminee (9 pts) !\n", i, j);
                    }
                }
            }
        }
    }

    // 2. Vérification Ville
    if (t.a == VILLE || t.b == VILLE || t.c == VILLE || t.d == VILLE ||
        t.a == VILLE_BOUCLIER || t.b == VILLE_BOUCLIER || t.c == VILLE_BOUCLIER || t.d == VILLE_BOUCLIER) {
        if (verifie_ville_complete(p, x, y) == 1) {
            int visite[TAILLE_MAX][TAILLE_MAX] = {0};
            int pts = compter_points_ville(p, x, y, visite);
            pts *= 2; // regle : ville complete = 2pts/tuile + 2pts/blason
            attribuer_points_et_recuperer_meeples(visite, pts, joueurs, nb_joueurs);
            printf("Une ville vient d'etre completee (%d pts) !\n", pts);
        }
    }

    // 3. Vérification Route
    if (t.a == ROUTE_PRAIRIE || t.b == ROUTE_PRAIRIE || t.c == ROUTE_PRAIRIE || t.d == ROUTE_PRAIRIE) {
        if (verifie_route_complete(p, x, y) == 1) {
            int visite[TAILLE_MAX][TAILLE_MAX] = {0};
            int pts = compter_points_route(p, x, y, visite);
            attribuer_points_et_recuperer_meeples(visite, pts, joueurs, nb_joueurs);
            printf("Une route vient d'etre finie (%d pts) !\n", pts);
        }
    }
}

// calcul du score final en fin de partie (structures incompletes)
void score_final(Plateau* p, Joueur joueurs[], int nb_joueurs) {
    int visite[TAILLE_MAX][TAILLE_MAX] = {0};

    printf("\n=== SCORE FINAL (structures incompletes) ===\n");

    for (int x = 0; x < TAILLE_MAX; x++) {
        for (int y = 0; y < TAILLE_MAX; y++) {
            if (!p->occupes[x][y] || visite[x][y]) continue;

            Tuiles t = p->grille[x][y];
            visite[x][y] = 1;

            // abbaye incomplete : 1 pt par tuile adjacente + elle meme
            if (t.center == ABBAYE) {
                int meeple_present = 0;
                for (int i = 0; i < nb_joueurs && !meeple_present; i++) {
                    for (int m = 0; m < 7; m++) {
                        if (joueurs[i].stock[m].etat == 0 &&
                            joueurs[i].stock[m].posX == x &&
                            joueurs[i].stock[m].posY == y) {
                            meeple_present = 1;
                            break;
                        }
                    }
                }
                if (meeple_present) {
                    int pts = 1;
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            if (dx == 0 && dy == 0) continue;
                            int nx = x + dx, ny = y + dy;
                            if (nx >= 0 && nx < TAILLE_MAX && ny >= 0 && ny < TAILLE_MAX && p->occupes[nx][ny])
                                pts++;
                        }
                    }
                    int visite_abbaye[TAILLE_MAX][TAILLE_MAX] = {0};
                    visite_abbaye[x][y] = 1;
                    attribuer_points_et_recuperer_meeples(visite_abbaye, pts, joueurs, nb_joueurs);
                    printf("Abbaye incomplete en (%d,%d) : %d pts\n", x, y, pts);
                }
            }

            // ville incomplete : 1 pt par tuile + 1 pt par blason (compter_points_ville retourne deja ca)
            if (t.a == VILLE || t.b == VILLE || t.c == VILLE || t.d == VILLE ||
                t.a == VILLE_BOUCLIER || t.b == VILLE_BOUCLIER || t.c == VILLE_BOUCLIER || t.d == VILLE_BOUCLIER) {
                int visite_ville[TAILLE_MAX][TAILLE_MAX] = {0};
                int pts = compter_points_ville(p, x, y, visite_ville);
                for (int i = 0; i < TAILLE_MAX; i++)
                    for (int j = 0; j < TAILLE_MAX; j++)
                        if (visite_ville[i][j]) visite[i][j] = 1;
                attribuer_points_et_recuperer_meeples(visite_ville, pts, joueurs, nb_joueurs);
                if (pts > 0) printf("Ville incomplete : %d pts\n", pts);
            }

            // route incomplete : 1 pt par tuile
            if (t.a == ROUTE_PRAIRIE || t.b == ROUTE_PRAIRIE || t.c == ROUTE_PRAIRIE || t.d == ROUTE_PRAIRIE) {
                int visite_route[TAILLE_MAX][TAILLE_MAX] = {0};
                int pts = compter_points_route(p, x, y, visite_route);
                for (int i = 0; i < TAILLE_MAX; i++)
                    for (int j = 0; j < TAILLE_MAX; j++)
                        if (visite_route[i][j]) visite[i][j] = 1;
                attribuer_points_et_recuperer_meeples(visite_route, pts, joueurs, nb_joueurs);
                if (pts > 0) printf("Route incomplete : %d pts\n", pts);
            }
        }
    }

    afficher_scores(joueurs, nb_joueurs);
    int gagnant = joueur_gagnant(joueurs, nb_joueurs);
    printf("=== GAGNANT : %s avec %d pts ===\n", joueurs[gagnant].nom, joueurs[gagnant].score);
}
