#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "config.h"

config* configuration(int mode, int nb_joueur, int seed, int ai,int max_turn){
    config* c = malloc(sizeof(config));
    assert(c != NULL);
    c->mode = mode;
    c->seed = seed;
    c->ai = ai;
    c->max_turn = max_turn;
    c->nbr_joueur = nb_joueur;
    Joueur* tab = malloc(sizeof(Joueur)*(nb_joueur+ai));
    assert(tab != NULL);
    for(int i=0;i<nb_joueur;i++){
        printf("Entrer les infos du Joueurs %d",i);
        //tab[i] = set_joueur();  //fonction non defini mais qui servirait à definir les infos du joueur
    }
    for(int i=nb_joueur;i<(nb_joueur+ai);i++){
        //tab[i] = set_ia(); //fonction non defini mais qui servirait à definir aleatoirement les infos de l'ia en tant que joueur
    }
    return c;
}

void print_config(config* configuration){
    if(configuration != NULL){
        if(configuration->mode == 1){
            printf("Mode: sdl \n");
        }
        else{
            printf("Mode: cli \n");
        }
        printf("seed : %d \n",configuration->seed);
        printf("nombre de joueur ia: %d \n",configuration->ai);
        printf("nombre de tours maximum: %d \n",configuration->max_turn);
        printf("nombre de joueur reel: %d \n",configuration->nbr_joueur);
    }
}
