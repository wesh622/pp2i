#include <stdio.h>
#include <stdlib.h>
#include "joueur.h"
#include "meeple.h"
#include <string.h>

void definirJoueur(Joueur *j, int id, char *name) {
    j->idjoueur = id;

    strncpy(j->nom, name, 19);
    j->nom[19] = '\0';
    j->est_IA = 0;

    j->score = 0;

    for (int i = 0; i < 7; i++) {
        init_meeple(&j->stock[i], i + 1, id);
    }
   
}

//debut Omar

void definirIA(Joueur *j, int id) {
    j->idjoueur = id;
    char nom_ia[20];
    sprintf(nom_ia, "IA_%d", id);
    strncpy(j->nom, nom_ia, 19);
    j->nom[19] = '\0';
    j->est_IA = 1;
    
    j->score = 0;
    j->actif = 1;

    for (int i = 0; i < 7; i++) {
        init_meeple(&j->stock[i], i + 1, id);
    }
}

//fin Omar

void enleverJoueur(Joueur *j) { 
    j->actif = 0;
    j->idjoueur = -1;
    strcpy(j->nom, "");
}

void ajouterpoints(Joueur *j, int points) {
    j->score += points;
}

int peutposermeeple(Joueur *j) {
    for (int i = 0; i < 7; i++) {
        if (j->stock[i].etat == 1) {
            return i; // Le joueur peut poser le meeple d'indice i en stock
        }
    }
    return -1; // Le joueur n'a plus de meeple en stock
}

void posermeeple(Joueur *j, int index) {
    j->stock[index].etat = 0;
}

void rendremeeple(Joueur *j, int idMeeple) {
    for (int i = 0; i < 7; i++) {
        if (j->stock[i].id == idMeeple) {
            j->stock[i].etat = 1;
        }
    }
}
