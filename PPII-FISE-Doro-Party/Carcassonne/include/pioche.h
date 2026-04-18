#ifndef PIOCHE_H
#define PIOCHE_H
#include "tuiles.h"

typedef struct _Pioche {
    Tuiles pile[72]; // Initialisation de la pile dans laquelle on va stocker les tuiles
    int indicecourant; // Indice de la tuile pioché qui va donc de 0 (quand aucune tuile n'est pioché) à 72
    int nbresrestantes; // Indique le nombre de tuiles restantes à piocher
} Pioche;

Pioche* init_pioche(int seed);
Tuiles* piocher(Pioche* p);
int pioche_vide(Pioche* p);
void free_pioche(Pioche* p);

#endif

