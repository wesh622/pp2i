#ifndef MEEPLE_H
#define MEEPLE_H
#include "tuiles.h"

typedef struct _Meeple {
    int id; // Entier qui va de 1 à 7 pour chaque joueur
    int idjoueur; // Entier compris entre 1 et 4 qui permet de savoir à quel joueur appartient ce meeple
    int etat; // 0 si le meeple est posé sur le plateau et 1 si il est disponible
    int zone; // Entier compris entre 1 et 6, qui définit sur quel genre de zone est posé le meeple, donc on peut mettre HERBE par exemple vu que j'ai include le tuiles.h
    int emplacement; // Précision : face a, b, c, d ou centre
    int posX; // Coordonnée X sur le plateau
    int posY; // Coordonnée Y sur le plateau

} Meeple;

#endif
