#ifndef SDL_FIN_H
#define SDL_FIN_H

#include "sdl_init.h"
#include "joueur.h"

// bloquant — attend Espace ou Echap
void sdl_afficher_fin_de_partie(ContexteSDL* ctx, Joueur joueurs[], int nb_joueurs);

#endif
