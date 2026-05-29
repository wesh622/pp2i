#include <stdio.h>
#include "score.h"
#include "joueur.h"
#include "tuiles.h"
#include "plateau.h"
#include "meeple.h"

#define MAX_VISITE 144


static int ville_deja_comptee(int villes[],int id) {
    for (int i = 0; i < 72; i++)
        if (villes[i] == id)
            return 1;
    return 0;
}

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

// compte les points d'une ville en utilisant une approche récursive
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
        pts += compter_points_ville(p, x-1, y, visite);
    } 

    if (t.b == VILLE || t.b == VILLE_BOUCLIER) {
        pts += compter_points_ville(p, x, y+1, visite);
    } 

    if (t.c == VILLE || t.c == VILLE_BOUCLIER) {
        pts += compter_points_ville(p, x+1, y, visite);
    }

    if (t.d == VILLE || t.d == VILLE_BOUCLIER) {
        pts += compter_points_ville(p, x, y-1, visite);
    } 

    return pts;
}

// compte les points d'une route en utilisant une approche récursive
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
        pts += compter_points_route(p, x-1, y, visite);
    } 
    if (t.b == ROUTE_PRAIRIE) {
        pts += compter_points_route(p, x, y+1, visite);
    } 
    if (t.c == ROUTE_PRAIRIE) {
        pts += compter_points_route(p, x+1, y, visite);
    } 
    if (t.d == ROUTE_PRAIRIE) {
        pts += compter_points_route(p, x, y-1, visite);
    } 

    return pts;
}

// Attribue les points à tous les joueurs  
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

// Vérifie les structures complètes après la pose d'une tuile et attribue les points
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


void calculer_score_prairies_fin_partie(Plateau* p,Joueur joueurs[],int nb_joueurs) {
    int visite[MAX_VISITE][MAX_VISITE] = {0};

    for (int x = 0; x < TAILLE_MAX; x++)
    {
        for (int y = 0; y < TAILLE_MAX; y++)
        {
            if (!p->occupes[x][y])
                continue;

            if (visite[x][y])
                continue;

            Tuiles t = p->grille[x][y];

            // On ne part que des prairies
            if (!(t.a == PRAIRIE || t.b == PRAIRIE ||
                  t.c == PRAIRIE || t.d == PRAIRIE))
                continue;

            int prairie_visite[MAX_VISITE][MAX_VISITE] = {0};
            int villes[72] = {0};
            int meeples[NB_JOUEURS_MAX] = {0};

            // DFS prairie
            prairie_recursive(p, x, y, prairie_visite);

            // marquer globalement
            for (int i = 0; i < TAILLE_MAX; i++)
                for (int j = 0; j < TAILLE_MAX; j++)
                    if (prairie_visite[i][j])
                        visite[i][j] = 1;

            // analyser meeples + villes
            for (int j = 0; j < nb_joueurs; j++)
            {
                for (int m = 0; m < 7; m++)
                {
                    Meeple *me =
                        &joueurs[j].stock[m];

                    if (me->etat == 1)
                        continue;

                    if (me->zone != PRAIRIE)
                        continue;

                    if (!prairie_visite[me->posX][me->posY])
                        continue;

                    meeples[j]++;
                }
            }

            // compter villes complètes adjacentes
            for (int i = 0; i < TAILLE_MAX; i++)
            {
                for (int j = 0; j < TAILLE_MAX; j++)
                {
                    if (!prairie_visite[i][j])
                        continue;

                    Tuiles tile =
                        p->grille[i][j];

                    if ((tile.a == VILLE || tile.a == VILLE_BOUCLIER) ||
                        (tile.b == VILLE || tile.b == VILLE_BOUCLIER) ||
                        (tile.c == VILLE || tile.c == VILLE_BOUCLIER) ||
                        (tile.d == VILLE || tile.d == VILLE_BOUCLIER))
                    {
                        int v[MAX_VISITE][MAX_VISITE] = {0};

                        if (ville_recursive(p, i, j, v))
                        {
                            if (!ville_deja_comptee(villes, tile.id))
                                villes[tile.id] = 1;
                        }
                    }
                }
            }

            int nb_villes = 0;

            for (int i = 0; i < 72; i++)
                if (villes[i])
                    nb_villes++;

            int points = nb_villes * 3;

            if (points == 0)
                continue;

            // majorité
            int max = 0;

            for (int j = 0; j < nb_joueurs; j++)
                if (meeples[j] > max)
                    max = meeples[j];

            if (max == 0)
                continue;

            for (int j = 0; j < nb_joueurs; j++)
            {
                if (meeples[j] == max)
                {
                    ajouterpoints(&joueurs[j], points);

                    printf(
                        "%s gagne %d points de prairie\n",
                        joueurs[j].nom,
                        points
                    );
                }
            }
        }
    }

    printf("Score prairies fin de partie termine.\n");
}

