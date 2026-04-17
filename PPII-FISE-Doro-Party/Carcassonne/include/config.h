#ifndef CONFIG_H
#define CONFIG_H
#include "joueur.h"

//Ce fichier consiste a definir la structure de la configuration du jeu rentrée en paramétre

typedef struct Config{
    int mode; //1 cli 2 sdl 3 rien
    int nbr_joueur;
    Joueur* tab; //tableau de joueurs
    int seed;
    int ai;
    int max_turn;
}config;

config* configuration(int mode, int nb_joueur, int seed, int ai,int max_turn);

#endif
