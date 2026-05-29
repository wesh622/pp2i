#ifndef SCORE_H
#define SCORE_H

#include "joueur.h"
#include "plateau.h"

#define NB_JOUEURS_MAX 4

// initialise tous les score a 0
void init_scores(Joueur joueurs[], int nb_joueurs);

// ajoute des points a un joueur specifique
void ajouter_score(Joueur joueurs[], int idjoueur, int points);

// affiche les scores de tous les joueurs actifs
void afficher_scores(Joueur joueurs[], int nb_joueurs);

// retourne l'index du meuilleur joueur
int joueur_gagnant(Joueur joueurs[], int nb_joueurs);

int compter_points_ville(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]);

int compter_points_route(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]);

void attribuer_points_et_recuperer_meeples(int visite[TAILLE_MAX][TAILLE_MAX], int points, Joueur joueurs[], int nb_joueurs);

void verifier_et_scorer_structures(Plateau* p, int x, int y, Joueur joueurs[], int nb_joueurs);

// score les structures incompletes en fin de partie (1pt par tuile)
void score_final(Plateau* p, Joueur joueurs[], int nb_joueurs);

#endif
