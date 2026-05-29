#ifndef PLATEAU_H
#define PLATEAU_H
#define TAILLE_MAX 144
#include "tuiles.h"
#include "joueur.h"
#include "pioche.h"
#include "meeple.h"
#include "config.h"

typedef struct _Plateau {
    Tuiles grille[TAILLE_MAX][TAILLE_MAX]; //Initialisation du plateau, en l'occurence ici c'est un tabelau 50x50
    int occupes[TAILLE_MAX][TAILLE_MAX]; // 0 si l'emplacement est vide et 1 sinon
} Plateau;

Plateau* init_plateau(void);
int peut_poser_tuile(Plateau* p, Tuiles t, int x, int y);
int peut_poser_tuile_silent(Plateau* p, Tuiles* t, int x, int y);
void poser_tuile(Plateau* p, Tuiles tuile, int x, int y);
void afficher_plateau(Plateau* p);
void afficher_plateau_pour_placer_tuile(Plateau* p,Pioche* pioche);
void free_plateau(Plateau* p);
void afficher_tour(int numero_tour, Joueur *j);
void afficher_tuile(Tuiles t);
char face_vers_char(int face);
int verifier_abbaye_complete(Plateau* p, int x, int y);
int ville_recursive(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]);
int route_recursive(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]);
int prairie_recursive(Plateau* p,int x,int y,int visite[TAILLE_MAX][TAILLE_MAX]);
int verifie_ville_complete(Plateau* p, int x, int y);
int verifie_route_complete(Plateau* p, int x, int y);
void choix_case_IA(Plateau* p, Tuiles tuile, Joueur* joueur, config* conf, int total_joueurs);
Tuiles* piocher_tuile_jouable(Pioche* pioche, Plateau* plateau);

#endif

