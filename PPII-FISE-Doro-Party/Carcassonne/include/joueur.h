#ifndef JOUEUR_H
#define JOUEUR_H
#include "meeple.h"

typedef struct _Joueur {
    char nom[20]; // 20 charactères maximum pour un nom
    int idjoueur; // Entier compris entre 1 et 4
    int actif; // 1 si il est dans la partie 0 s'il a quitté
    Meeple stock[7]; // Stock de meeple pour le joueur
    int score; // Score du joueur 
} Joueur;

//Debut Omar
void definirJoueur(Joueur *j, int id, char *name);
void definirIA(Joueur *j, int id);  
void enleverJoueur(Joueur *j);
void ajouterpoints(Joueur *j, int points);
int peutposermeeple(Joueur *j);
void posermeeple(Joueur *j, int index);
void rendremeeple(Joueur *j, int idMeeple);
//fin

#endif
