#ifndef SCORE_H
#define SCORE_H

#include "joueur.h"
#include "plateau.h"

#define NB_JOUEURS_MAX 4

// Initialise les scores
void init_scores(Joueur joueurs[], int nb_joueurs);

// Ajoute des points
void ajouter_score(Joueur joueurs[], int index_joueur, int points);

// Affiche le tableau des scores 
void afficher_scores(Joueur joueurs[], int nb_joueurs);

// Retourne le meuilleur joueurs
int joueur_gagnant(Joueur joueurs[], int nb_joueurs);

int compter_points_ville(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]);

int compter_points_route(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]) ;

void attribuer_points_et_recuperer_meeples(int visite[TAILLE_MAX][TAILLE_MAX], int points, Joueur joueurs[], int nb_joueurs);

void verifier_et_scorer_structures(Plateau* p, int x, int y, Joueur joueurs[], int nb_joueurs);

#endif
