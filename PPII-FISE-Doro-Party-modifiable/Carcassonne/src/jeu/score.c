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
     
    // Si c'est une VILLE_BOUCLIER, on ajoute encore 2 points 
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
    
    // Compter le nombre de meeples de chaque joueur sur la structure
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
    
    // Trouver la majorité absolue
    int max_meeples = 0;
    for (int i = 0; i < nb_joueurs; i++) {
        if (meeples_par_joueur[i] > max_meeples) {
            max_meeples = meeples_par_joueur[i];
        } 
    }
    
    // Si des meeples étaient présents, donner les points aux vainqueurs et tout nettoyer
    if (max_meeples > 0) {
        for (int i = 0; i < nb_joueurs; i++) {
            if (!joueurs[i].actif) {
                continue;
            } 
            
            // Attribution des points
            if (meeples_par_joueur[i] == max_meeples) {
                joueurs[i].score += points;
            }
            
            // Récupération des meeples pour TOUS les joueurs présents dans la zone
            for (int m = 0; m < 7; m++) {
                if (joueurs[i].stock[m].etat == 0) {
                    int mx = joueurs[i].stock[m].posX;
                    int my = joueurs[i].stock[m].posY;
                    if (mx >= 0 && my >= 0 && visite[mx][my]) {
                        joueurs[i].stock[m].etat = 1; // Le meeple redevient dispo
                        joueurs[i].stock[m].posX = -1; // On reset sa position
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
    if (t.a == VILLE || t.b == VILLE || t.c == VILLE || t.d == VILLE || t.a == VILLE_BOUCLIER || t.b == VILLE_BOUCLIER) {
        if (verifie_ville_complete(p, x, y) == 1) {
            int visite[TAILLE_MAX][TAILLE_MAX] = {0};
            int pts = compter_points_ville(p, x, y, visite);
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
