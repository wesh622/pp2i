#ifndef PLATEAU_H
#define PLATEAU_H
#define TAILLE_MAX 72
#include "tuiles.h"

typedef struct _Plateau {
    Tuiles grille[TAILLE_MAX][TAILLE_MAX]; //Initialisation du plateau, en l'occurence ici c'est un tabelau 50x50
    int occupes[TAILLE_MAX][TAILLE_MAX]; // 0 si l'emplacement est vide et 1 sinon
} Plateau;

Plateau* init_plateau();
int peut_poser_tuile(Plateau* p, int x, int y);
void poser_tuile(Plateau* p, Tuiles tuile, int x, int y);
void afficher_plateau(Plateau* p);
void free_plateau(Plateau* p);

#endif

