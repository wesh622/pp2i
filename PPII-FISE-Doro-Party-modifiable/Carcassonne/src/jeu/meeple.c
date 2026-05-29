#include "meeple.h"
#include "plateau.h"
#include "joueur.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Debut: Mohamed

// Initialisation d'un meeple
void init_meeple(Meeple* m, int id, int idjoueur){
    assert(m != NULL);
    assert(id >= 1 && id <= 7);
    assert(idjoueur >= 1 && idjoueur <= 5);
    m->id = id;
    m->idjoueur = idjoueur;
    m->etat = 1; 
    m->zone = 0; 
    m->emplacement = 0;
    m->posX = -1; 
    m->posY = -1;
}

// Placement d'un meeple sur le plateau
int placer_meeple(Meeple* m, int posX, int posY, int zone, int emplacement){
    if(m->etat != 1){
        return 0;
    }
    else{
        m->etat = 0; 
        m->zone = zone; 
        m->emplacement = emplacement;
        m->posX = posX; 
        m->posY = posY;
        return 1;
    }
}

// Vérifie si un meeple peut etre place a la position 
int peut_placer_meeple(Plateau* plateau, int posX, int posY, int emplacement){
    if(!(posX >= 0 && posX < TAILLE_MAX && posY >= 0 && posY < TAILLE_MAX)){
        return 0;
    }
    else if(plateau->occupes[posX][posY] == 0){
        return 0;
    }
    else if(!(emplacement >= 1 && emplacement <= 5)){
        return 0;
    }
    else{
        return 1;
    }
}

// retirer un meeple du plateau
void retirer_meeple(Meeple* m){
    m->etat = 1; 
    m->zone = 0; 
    m->emplacement = 0;
    m->posX = -1; 
    m->posY = -1;    
}

// Vérifie si un meeple est disponible pour être placé
int meeple_disponible(Meeple* m){
    if(m->etat == 1){
        return 1;
    }
    return 0;
}    

// Vérifie si au moins un meeple du joueur est disponible
int au_moins_un_meeple_disponible(Joueur* j){
    for (int i=0; i<7 ; i++) {
        if (meeple_disponible(&(j->stock[i])) == 1) {
            return 1;
        }
    }
    return 0;
}

// Retourne un pointeur vers le premier meeple disponible du joueur.
Meeple* premier_meeple_disponible(Joueur* j){
    for (int i=0; i<7 ; i++) {
        if (meeple_disponible(&(j->stock[i])) == 1) {
            return &(j->stock[i]);
        }
    }
    return &(j->stock[0]);
}

// Affichage d'un meeple (pour debug)
void afficher_meeple(Meeple* m){
    if(m == NULL){
        return;
    }
    printf("Meeple:  -id=%d -joueur=%d -etat=%d -zone=%d -position=(%d,%d) -emplacement=%d\n",
           m->id,
           m->idjoueur,
           m->etat,
           m->zone,
           m->posX,
           m->posY,
           m->emplacement);
}

// Choix de l'IA pour placer un meeple
void choix_meeple_IA(Plateau* plateau, Tuiles tuile, Joueur* j, int x, int y) {
        int n = rand();
        if (n % 2 == 0) { // L'IA veut poser un meeple
            int emplacement = (rand() % 5) + 1;
            int zone = 0;
            for (int i = 1; i<6; i++) {
                if (emplacement == 1) {
                    zone = tuile.a;
                }
                if (emplacement == 2) {
                    zone = tuile.b;
                }
                if (emplacement == 3) {
                    zone = tuile.c;
                }
                if (emplacement == 4) {
                    zone = tuile.d;
                }
                if (emplacement == 5) {
                    zone = tuile.center;
                }
                if (peut_placer_meeple(plateau, x, y, emplacement)) {
                    Meeple* m = premier_meeple_disponible(j);
                    placer_meeple(m, x, y, zone, emplacement);
                    return;
                }
            }
        }
    }



