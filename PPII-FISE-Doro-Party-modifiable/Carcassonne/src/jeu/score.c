#include <stdio.h>
#include "score.h"
#include "joueur.h"
#include "tuiles.h"
#include "plateau.h"

//debut Moran

void init_scores(Joueur joueurs[], int nb_joueurs) {
    for (int i = 0; i < nb_joueurs; i++) {
        joueurs[i].score = 0;
    }
}

// ajouter des points au bon joueur
void ajouter_score(Joueur joueurs[], int idjoueur, int points) {
    if (idjoueur < 0 || idjoueur >= NB_JOUEURS_MAX) return;
    if (!joueurs[idjoueur].actif) return;
    joueurs[idjoueur].score += points;
}

void afficher_scores(Joueur joueurs[], int nb_joueurs) {
    printf("\n=== SCORES ===\n");
    for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].actif) {
            printf("  %s : %d pts\n", joueurs[i].nom, joueurs[i].score);
        }
    }
    printf("==============\n\n");
}

// compare les scores et retourne l'index du gagnant
int joueur_gagnant(Joueur joueurs[], int nb_joueurs) {
    int max_score = -1;
    int index = 0;
    for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].actif && joueurs[i].score > max_score) {
            max_score = joueurs[i].score;
            index = i;
        }
    }
    return index;
}

// compte recursivement les points d'une ville a partir de (x,y)
int compter_points_ville(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]) {
    if (x < 0 || x >= TAILLE_MAX || y < 0 || y >= TAILLE_MAX || p->occupes[x][y] == 0 || visite[x][y]) {
        return 0;
    }

    visite[x][y] = 1;
    Tuiles t = p->grille[x][y];

    int pts = 1;
    // bouclier vaut un point de plus
    if (t.a == VILLE_BOUCLIER || t.b == VILLE_BOUCLIER || t.c == VILLE_BOUCLIER || t.d == VILLE_BOUCLIER) {
        pts += 1;
    }

    if (t.a == VILLE || t.a == VILLE_BOUCLIER) {
        pts += compter_points_ville(p, x, y+1, visite);
    }
    if (t.b == VILLE || t.b == VILLE_BOUCLIER) {
        pts += compter_points_ville(p, x+1, y, visite);
    }
    if (t.c == VILLE || t.c == VILLE_BOUCLIER) {
        pts += compter_points_ville(p, x, y-1, visite);
    }
    if (t.d == VILLE || t.d == VILLE_BOUCLIER) {
        pts += compter_points_ville(p, x-1, y, visite);
    }

    return pts;
}

// compte recursivement les points d'une route a partir de (x,y)
int compter_points_route(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]) {
    if (x < 0 || x >= TAILLE_MAX || y < 0 || y >= TAILLE_MAX || p->occupes[x][y] == 0) {
        return 0;
    }
    if (visite[x][y] == 1) return 0; // deja compté

    visite[x][y] = 1;
    Tuiles t = p->grille[x][y];
    int pts = 1; // 1 point par tuile route

    if (t.a == ROUTE_PRAIRIE) {
        pts += compter_points_route(p, x, y+1, visite);
    }
    if (t.b == ROUTE_PRAIRIE) {
        pts += compter_points_route(p, x+1, y, visite);
    }
    if (t.c == ROUTE_PRAIRIE) {
        pts += compter_points_route(p, x, y-1, visite);
    }
    if (t.d == ROUTE_PRAIRIE) {
        pts += compter_points_route(p, x-1, y, visite);
    }

    return pts;
}

// regarde qui a la majorité de meeples sur la structure et attribue les points
// en cas d'egalité tous les joueurs a egalite recoivent les points
void attribuer_points_et_recuperer_meeples(int visite[TAILLE_MAX][TAILLE_MAX], int points, Joueur joueurs[], int nb_joueurs) {
    int meeples_par_joueur[NB_JOUEURS_MAX] = {0};

    for (int i = 0; i < nb_joueurs; i++) {
        if (!joueurs[i].actif) continue;
        for (int m = 0; m < 7; m++) {
            if (joueurs[i].stock[m].etat == 0) { // meeple posé
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
        if (meeples_par_joueur[i] > max_meeples)
            max_meeples = meeples_par_joueur[i];
    }

    if (max_meeples == 0) return; // personne sur la stucture

    for (int i = 0; i < nb_joueurs; i++) {
        if (!joueurs[i].actif) continue;

        if (meeples_par_joueur[i] == max_meeples) {
            joueurs[i].score += points;
        }

        // on recupere les meeples de TOUS les joueurs present dans la zone
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

// verifie apres chaque pose de tuile si une structure est completée et score
void verifier_et_scorer_structures(Plateau* p, int x, int y, Joueur joueurs[], int nb_joueurs) {
    Tuiles t = p->grille[x][y];

    // 1. abbaye : la tuile posee peut completer une abbaye adjacente
    for (int i = x-1; i <= x+1; i++) {
        for (int j = y-1; j <= y+1; j++) {
            if (i >= 0 && i < TAILLE_MAX && j >= 0 && j < TAILLE_MAX) {
                if (p->occupes[i][j] && p->grille[i][j].center == ABBAYE) {
                    if (verifier_abbaye_complete(p, i, j) == 1) {
                        int visite[TAILLE_MAX][TAILLE_MAX] = {0};
                        visite[i][j] = 1;
                        attribuer_points_et_recuperer_meeples(visite, 9, joueurs, nb_joueurs);
                        printf("L'abbaye en (%d,%d) est terminee (9 pts) !\n", i, j);
                    }
                }
            }
        }
    }

    // 2. ville
    if (t.a == VILLE || t.b == VILLE || t.c == VILLE || t.d == VILLE ||
        t.a == VILLE_BOUCLIER || t.b == VILLE_BOUCLIER || t.c == VILLE_BOUCLIER || t.d == VILLE_BOUCLIER) {
        if (verifie_ville_complete(p, x, y) == 1) {
            int visite[TAILLE_MAX][TAILLE_MAX] = {0};
            int pts = compter_points_ville(p, x, y, visite);
            attribuer_points_et_recuperer_meeples(visite, pts, joueurs, nb_joueurs);
            printf("Une ville vient d'etre completee (%d pts) !\n", pts);
        }
    }

    // 3. route
    if (t.a == ROUTE_PRAIRIE || t.b == ROUTE_PRAIRIE || t.c == ROUTE_PRAIRIE || t.d == ROUTE_PRAIRIE) {
        if (verifie_route_complete(p, x, y) == 1) {
            int visite[TAILLE_MAX][TAILLE_MAX] = {0};
            int pts = compter_points_route(p, x, y, visite);
            attribuer_points_et_recuperer_meeples(visite, pts, joueurs, nb_joueurs);
            printf("Une route vient d'etre completee (%d pts) !\n", pts);
        }
    }
}

// score les stuctures non completes a la fin de la partie
// regles fin de partie : 1pt par tuile pour les villes et routes incompletes
void score_final(Plateau* p, Joueur joueurs[], int nb_joueurs) {
    printf("\n=== FIN DE PARTIE : calcul des structures incompletes ===\n");

    int deja_compte[TAILLE_MAX][TAILLE_MAX] = {0};

    for (int x = 0; x < TAILLE_MAX; x++) {
        for (int y = 0; y < TAILLE_MAX; y++) {
            if (!p->occupes[x][y] || deja_compte[x][y]) continue;

            Tuiles t = p->grille[x][y];

            // abbaye non terminee : 1pt par tuile presente autour
            if (t.center == ABBAYE) {
                int cases_pleines = 0;
                for (int i = x-1; i <= x+1; i++) {
                    for (int j = y-1; j <= y+1; j++) {
                        if (i >= 0 && i < TAILLE_MAX && j >= 0 && j < TAILLE_MAX) {
                            if (p->occupes[i][j]) cases_pleines++;
                        }
                    }
                }
                if (cases_pleines < 9) { // abbaye incomplete seulement
                    int visite[TAILLE_MAX][TAILLE_MAX] = {0};
                    visite[x][y] = 1;
                    attribuer_points_et_recuperer_meeples(visite, cases_pleines, joueurs, nb_joueurs);
                    deja_compte[x][y] = 1;
                }
            }

            // ville incomplete
            if ((t.a == VILLE || t.a == VILLE_BOUCLIER ||
                 t.b == VILLE || t.b == VILLE_BOUCLIER ||
                 t.c == VILLE || t.c == VILLE_BOUCLIER ||
                 t.d == VILLE || t.d == VILLE_BOUCLIER) &&
                !verifie_ville_complete(p, x, y)) {
                int visite[TAILLE_MAX][TAILLE_MAX] = {0};
                int pts = compter_points_ville(p, x, y, visite);
                // marque tout le secteur comme compte
                for (int i = 0; i < TAILLE_MAX; i++)
                    for (int j = 0; j < TAILLE_MAX; j++)
                        if (visite[i][j]) deja_compte[i][j] = 1;
                attribuer_points_et_recuperer_meeples(visite, pts, joueurs, nb_joueurs);
            }

            // route incomplete
            if ((t.a == ROUTE_PRAIRIE || t.b == ROUTE_PRAIRIE ||
                 t.c == ROUTE_PRAIRIE || t.d == ROUTE_PRAIRIE) &&
                !verifie_route_complete(p, x, y)) {
                int visite[TAILLE_MAX][TAILLE_MAX] = {0};
                int pts = compter_points_route(p, x, y, visite);
                for (int i = 0; i < TAILLE_MAX; i++)
                    for (int j = 0; j < TAILLE_MAX; j++)
                        if (visite[i][j]) deja_compte[i][j] = 1;
                attribuer_points_et_recuperer_meeples(visite, pts, joueurs, nb_joueurs);
            }
        }
    }

    afficher_scores(joueurs, nb_joueurs);
}

//fin Moran
