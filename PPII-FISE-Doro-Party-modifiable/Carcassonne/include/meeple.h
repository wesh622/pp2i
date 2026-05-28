#ifndef MEEPLE_H
#define MEEPLE_H
#include "tuiles.h"

struct _Joueur;
struct _Plateau;

typedef struct _Meeple {
    int id; //de 1 a 7 pr chaque joueur
    int idjoueur; //entre 1 et 5
    int etat; //0 pose 1 dispo
    int zone; //type zone (VILLE ROUTE etc)
    int emplacement; //1=a 2=b 3=c 4=d 5=centre
    int posX; //coord X (-1 si pa pose)
    int posY; //coord Y (-1 si pa pose)
} Meeple;

void init_meeple(Meeple* m, int id, int idjoueur);
int placer_meeple(Meeple* m, int posX, int posY, int zone, int emplacement);
int peut_placer_meeple(struct _Plateau* plateau, int posX, int posY,int emplacement);
void retirer_meeple(Meeple* m);
int meeple_disponible(Meeple* m);
void afficher_meeple(Meeple* m);
int au_moins_un_meeple_disponible(struct _Joueur* j);
Meeple* premier_meeple_disponible(struct _Joueur* j);
void choix_meeple_IA(struct _Plateau* plateau, Tuiles tuile,struct _Joueur* j, int x, int y);

#endif
