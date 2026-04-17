#ifndef TUILE_H
#define TUILE_H
#define ROUTE_PRAIRIE  1
#define PRAIRIE  2
#define VILLE  3
#define VILLE_BOUCLIER 4
#define ABBAYE 5
#define CARREFOUR 6


typedef struct _Tuiles {
    int id;  //numéro de la tuile
    int a;   //face nord avant rotation
    int b;   //face est avant rotation
    int c;   //face sud avant rotation
    int d;   //face ouest avant rotation
    int center;    //centre de la tuile
} Tuiles;

// id prend une valeur allant de 1 à 72
// a,b,c,d prennent des valeurs allant de 1 à 4
// center prend une valeur allant de 1 à 6
//   1 : cette face correspond à un chemin, donc les bordures de cette face sont de la prairie
//   2 : cette face correspond à une prairie
//   3 : cette face correspond à une ville
//   4 : cette face correspond à une ville avec un bouclier
//   5 : il y a une abbaye
//   6 : il y a un carrefour, fermeture de chemin

extern Tuiles tuiles_jeu[];  // Permet d'utiliser le tableau tuiles_jeu définit dans tuiles.c
// dans l'ensemble des fichiers du projet


#endif
