#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "config.h"
#include "joueur.h"

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
    c->tab = tab;
    char* test_env = getenv("CARCASSONNE_TEST");
    for(int i=0;i<nb_joueur;i++){
        char nom[20];
        if(test_env && strcmp(test_env, "1") == 0) {
            snprintf(nom, sizeof(nom), "Test%d", i+1);
        } else {
            printf("Entrer les infos du Joueur %d\n", i+1);
            printf("Nom du joueur %d: ", i+1);
            scanf("%19s", nom);
        }
        definirJoueur(&c->tab[i], i+1, nom);
        c->tab[i].actif = 1;
    }
    for(int i=nb_joueur;i<(nb_joueur+ai);i++){
        definirIA(&c->tab[i], i+1);
    }
    return c;
}

void print_config(config* configuration){
    if(configuration != NULL){
        if(configuration->mode == 2){
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

void free_config(config* c){
    if(c){
        if(c->tab) free(c->tab);
        free(c);
    }
}
