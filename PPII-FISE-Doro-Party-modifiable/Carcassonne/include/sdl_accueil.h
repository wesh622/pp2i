#ifndef SDL_ACCUEIL_H
#define SDL_ACCUEIL_H

#include "sdl_init.h"
#include "argc.h"

// écran d'accueil — bloquant, attend Espace pour démarrer
void sdl_afficher_accueil(ContexteSDL* ctx, config* conf, int total_joueurs);

#endif
