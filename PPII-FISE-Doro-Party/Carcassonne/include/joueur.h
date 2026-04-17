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

#endif
