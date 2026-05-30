#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "score.h"
#include "meeple.h"

void init_scores(Joueur joueurs[], int nb_joueurs) {
    for (int i = 0; i < nb_joueurs; i++) {
        joueurs[i].score = 0;
    }
}

void ajouter_score(Joueur joueurs[], int index_joueur, int points) {
    if (index_joueur < 0 || index_joueur >= NB_JOUEURS_MAX) return;
    if (!joueurs[index_joueur].actif) return;
    joueurs[index_joueur].score += points;
}

void afficher_scores(Joueur joueurs[], int nb_joueurs) {
    printf("\n=== SCORES ===\n");
    for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].actif)
            printf("  %s : %d pts\n", joueurs[i].nom, joueurs[i].score);
    }
    printf("==============\n\n");
}

int joueur_gagnant(Joueur joueurs[], int nb_joueurs) {
    int idx = -1;
    int max = -1;
    for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].actif && joueurs[i].score > max) {
            max = joueurs[i].score;
            idx = i;
        }
    }
    return idx;
}

// compte tuiles + bouclier d'une ville (1pt/tuile 1pt/bouclier)
int compter_points_ville(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]) {
    if (x < 0 || x >= TAILLE_MAX || y < 0 || y >= TAILLE_MAX) return 0;
    if (!p->occupes[x][y] || visite[x][y]) return 0;
    Tuiles t = p->grille[x][y];
    if (t.a != VILLE && t.a != VILLE_BOUCLIER &&
        t.b != VILLE && t.b != VILLE_BOUCLIER &&
        t.c != VILLE && t.c != VILLE_BOUCLIER &&
        t.d != VILLE && t.d != VILLE_BOUCLIER) return 0;
    visite[x][y] = 1;
    int pts = 1;
    if (t.a == VILLE_BOUCLIER || t.b == VILLE_BOUCLIER ||
        t.c == VILLE_BOUCLIER || t.d == VILLE_BOUCLIER) pts++;
    if (t.a == VILLE || t.a == VILLE_BOUCLIER)
        pts += compter_points_ville(p, x-1, y, visite);
    if (t.b == VILLE || t.b == VILLE_BOUCLIER)
        pts += compter_points_ville(p, x, y+1, visite);
    if (t.c == VILLE || t.c == VILLE_BOUCLIER)
        pts += compter_points_ville(p, x+1, y, visite);
    if (t.d == VILLE || t.d == VILLE_BOUCLIER)
        pts += compter_points_ville(p, x, y-1, visite);
    return pts;
}

int compter_points_ville_incomplete(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]) {
    return compter_points_ville(p, x, y, visite);
}

// compte les tuiles d'une route (1pt/tuile)
int compter_points_route(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]) {
    if (x < 0 || x >= TAILLE_MAX || y < 0 || y >= TAILLE_MAX) return 0;
    if (!p->occupes[x][y] || visite[x][y]) return 0;
    Tuiles t = p->grille[x][y];
    if (t.a != ROUTE_PRAIRIE && t.b != ROUTE_PRAIRIE &&
        t.c != ROUTE_PRAIRIE && t.d != ROUTE_PRAIRIE) return 0;
    visite[x][y] = 1;
    if (t.center == CARREFOUR) return 1; //carrefour = fin de route
    int pts = 1;
    if (t.a == ROUTE_PRAIRIE) pts += compter_points_route(p, x-1, y, visite);
    if (t.b == ROUTE_PRAIRIE) pts += compter_points_route(p, x, y+1, visite);
    if (t.c == ROUTE_PRAIRIE) pts += compter_points_route(p, x+1, y, visite);
    if (t.d == ROUTE_PRAIRIE) pts += compter_points_route(p, x, y-1, visite);
    return pts;
}

void attribuer_points_et_recuperer_meeples(int visite[TAILLE_MAX][TAILLE_MAX], int points, Joueur joueurs[], int nb_joueurs, int type_zone_cible) {
    (void)type_zone_cible;
    int nb_meeples[NB_JOUEURS_MAX] = {0};
    for (int i = 0; i < nb_joueurs; i++) {
        for (int k = 0; k < 7; k++) {
            Meeple* m = &joueurs[i].stock[k];
            if (m->etat == 0 && m->posX >= 0 && m->posY >= 0 && visite[m->posX][m->posY])
                nb_meeples[i]++;
        }
    }
    int max_meeples = 0;
    for (int i = 0; i < nb_joueurs; i++)
        if (nb_meeples[i] > max_meeples) max_meeples = nb_meeples[i];
    if (max_meeples == 0) return;
    for (int i = 0; i < nb_joueurs; i++) {
        if (nb_meeples[i] == max_meeples)
            joueurs[i].score += points;
        //recupere tout les meeples de la zone peut importe le nb
        for (int k = 0; k < 7; k++) {
            Meeple* m = &joueurs[i].stock[k];
            if (m->etat == 0 && m->posX >= 0 && m->posY >= 0 && visite[m->posX][m->posY])
                retirer_meeple(m);
        }
    }
}

void verifier_et_scorer_structures(Plateau* p, int x, int y, Joueur joueurs[], int nb_joueurs) {
    int visite[TAILLE_MAX][TAILLE_MAX];
    // abbayes : verifie la case posee et ses 8 voisines
    for (int i = x-1; i <= x+1; i++) {
        for (int j = y-1; j <= y+1; j++) {
            if (i < 0 || i >= TAILLE_MAX || j < 0 || j >= TAILLE_MAX) continue;
            if (!p->occupes[i][j] || p->grille[i][j].center != ABBAYE) continue;
            if (verifier_abbaye_complete(p, i, j)) {
                memset(visite, 0, sizeof(visite));
                for (int ii = i-1; ii <= i+1; ii++)
                    for (int jj = j-1; jj <= j+1; jj++)
                        if (ii >= 0 && ii < TAILLE_MAX && jj >= 0 && jj < TAILLE_MAX)
                            visite[ii][jj] = 1;
                attribuer_points_et_recuperer_meeples(visite, 9, joueurs, nb_joueurs, ABBAYE);
            }
        }
    }
    Tuiles t = p->grille[x][y];
    // villes
    if (t.a == VILLE || t.a == VILLE_BOUCLIER ||
        t.b == VILLE || t.b == VILLE_BOUCLIER ||
        t.c == VILLE || t.c == VILLE_BOUCLIER ||
        t.d == VILLE || t.d == VILLE_BOUCLIER) {
        if (verifie_ville_complete(p, x, y)) {
            memset(visite, 0, sizeof(visite));
            int pts = 2 * compter_points_ville(p, x, y, visite);
            attribuer_points_et_recuperer_meeples(visite, pts, joueurs, nb_joueurs, VILLE);
        }
    }
    // routes standard (tuile normale)
    if (t.center != CARREFOUR && (t.a == ROUTE_PRAIRIE || t.b == ROUTE_PRAIRIE ||
        t.c == ROUTE_PRAIRIE || t.d == ROUTE_PRAIRIE)) {
        if (verifie_route_complete(p, x, y)) {
            memset(visite, 0, sizeof(visite));
            int pts = compter_points_route(p, x, y, visite);
            attribuer_points_et_recuperer_meeples(visite, pts, joueurs, nb_joueurs, ROUTE_PRAIRIE);
        }
    }
    // carrefour : verifie chaque branche de route separement
    // route_recursive retourne 1 sur une case deja visitee -> pre-marquer le carrefour
    // le fait agir comme terminal sans modifié plateau.c
    if (t.center == CARREFOUR) {
        int dir_x[4] = {-1, 0, 1, 0};
        int dir_y[4] = {0, 1, 0, -1};
        int faces[4] = {t.a, t.b, t.c, t.d};
        for (int d = 0; d < 4; d++) {
            if (faces[d] != ROUTE_PRAIRIE) continue;
            int nx = x + dir_x[d];
            int ny = y + dir_y[d];
            if (nx < 0 || nx >= TAILLE_MAX || ny < 0 || ny >= TAILLE_MAX) continue;
            if (!p->occupes[nx][ny]) continue;
            memset(visite, 0, sizeof(visite));
            visite[x][y] = 1; // carrefour = terminal
            if (route_recursive(p, nx, ny, visite)) {
                memset(visite, 0, sizeof(visite));
                visite[x][y] = 1;
                int pts = 1 + compter_points_route(p, nx, ny, visite);
                attribuer_points_et_recuperer_meeples(visite, pts, joueurs, nb_joueurs, ROUTE_PRAIRIE);
            }
        }
    }
}

void calculer_score_incomplet_fin_partie(Plateau* p, Joueur joueurs[], int nb_joueurs) {
    int traite[TAILLE_MAX][TAILLE_MAX];
    int visite[TAILLE_MAX][TAILLE_MAX];
    memset(traite, 0, sizeof(traite));
    for (int x = 0; x < TAILLE_MAX; x++) {
        for (int y = 0; y < TAILLE_MAX; y++) {
            if (!p->occupes[x][y] || traite[x][y]) continue;
            Tuiles t = p->grille[x][y];
            // villes incompletes
            if ((t.a == VILLE || t.a == VILLE_BOUCLIER ||
                 t.b == VILLE || t.b == VILLE_BOUCLIER ||
                 t.c == VILLE || t.c == VILLE_BOUCLIER ||
                 t.d == VILLE || t.d == VILLE_BOUCLIER) &&
                !verifie_ville_complete(p, x, y)) {
                memset(visite, 0, sizeof(visite));
                int pts = compter_points_ville_incomplete(p, x, y, visite);
                attribuer_points_et_recuperer_meeples(visite, pts, joueurs, nb_joueurs, VILLE);
                for (int i = 0; i < TAILLE_MAX; i++)
                    for (int j = 0; j < TAILLE_MAX; j++)
                        if (visite[i][j]) traite[i][j] = 1;
            }
            // routes incompletes
            if ((t.a == ROUTE_PRAIRIE || t.b == ROUTE_PRAIRIE ||
                 t.c == ROUTE_PRAIRIE || t.d == ROUTE_PRAIRIE) &&
                !verifie_route_complete(p, x, y)) {
                memset(visite, 0, sizeof(visite));
                int pts = compter_points_route(p, x, y, visite);
                attribuer_points_et_recuperer_meeples(visite, pts, joueurs, nb_joueurs, ROUTE_PRAIRIE);
                for (int i = 0; i < TAILLE_MAX; i++)
                    for (int j = 0; j < TAILLE_MAX; j++)
                        if (visite[i][j]) traite[i][j] = 1;
            }
            // abbayes incompletes
            if (t.center == ABBAYE) {
                int cases_remplies = 0;
                memset(visite, 0, sizeof(visite));
                for (int i = x-1; i <= x+1; i++) {
                    for (int j = y-1; j <= y+1; j++) {
                        if (i < 0 || i >= TAILLE_MAX || j < 0 || j >= TAILLE_MAX) continue;
                        visite[i][j] = 1;
                        if (p->occupes[i][j]) cases_remplies++;
                    }
                }
                attribuer_points_et_recuperer_meeples(visite, cases_remplies, joueurs, nb_joueurs, ABBAYE);
                traite[x][y] = 1;
            }
        }
    }
}

void calculer_score_prairies_fin_partie(Plateau* p, Joueur joueurs[], int nb_joueurs) {
    // 3pts par ville complete adjacente au meeple de prairie
    int villes_comptees[TAILLE_MAX][TAILLE_MAX];
    int visite[TAILLE_MAX][TAILLE_MAX];
    memset(villes_comptees, 0, sizeof(villes_comptees));
    // precompute les tuiles apartenant a des villes completes
    for (int x = 0; x < TAILLE_MAX; x++) {
        for (int y = 0; y < TAILLE_MAX; y++) {
            if (!p->occupes[x][y] || villes_comptees[x][y]) continue;
            Tuiles t = p->grille[x][y];
            if ((t.a == VILLE || t.a == VILLE_BOUCLIER ||
                 t.b == VILLE || t.b == VILLE_BOUCLIER ||
                 t.c == VILLE || t.c == VILLE_BOUCLIER ||
                 t.d == VILLE || t.d == VILLE_BOUCLIER) &&
                verifie_ville_complete(p, x, y)) {
                memset(visite, 0, sizeof(visite));
                compter_points_ville(p, x, y, visite);
                for (int i = 0; i < TAILLE_MAX; i++)
                    for (int j = 0; j < TAILLE_MAX; j++)
                        if (visite[i][j]) villes_comptees[i][j] = 1;
            }
        }
    }
    for (int i = 0; i < nb_joueurs; i++) {
        if (!joueurs[i].actif) continue;
        for (int k = 0; k < 7; k++) {
            Meeple* m = &joueurs[i].stock[k];
            if (m->etat != 0 || m->zone != PRAIRIE) continue;
            int pts = 0;
            int vx = m->posX;
            int vy = m->posY;
            // villes completes deja vues autour de ce meeple
            int deja_vu[TAILLE_MAX][TAILLE_MAX];
            memset(deja_vu, 0, sizeof(deja_vu));
            int offsets[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
            for (int d = 0; d < 4; d++) {
                int nx = vx + offsets[d][0];
                int ny = vy + offsets[d][1];
                if (nx < 0 || nx >= TAILLE_MAX || ny < 0 || ny >= TAILLE_MAX) continue;
                if (!p->occupes[nx][ny] || !villes_comptees[nx][ny] || deja_vu[nx][ny]) continue;
                // marque toute la ville pour eviter double comptage
                memset(visite, 0, sizeof(visite));
                compter_points_ville(p, nx, ny, visite);
                for (int ii = 0; ii < TAILLE_MAX; ii++)
                    for (int jj = 0; jj < TAILLE_MAX; jj++)
                        if (visite[ii][jj]) deja_vu[ii][jj] = 1;
                pts += 3;
            }
            joueurs[i].score += pts;
            retirer_meeple(m);
        }
    }
}
