#ifndef SDL_ACCUEIL_H
#define SDL_ACCUEIL_H

#include "sdl_init.h"
#include "argc.h"

// menu principal avec Jouer/Regles/Quitter — retourne 1=jouer, 0=quitter
int sdl_menu_principal(ContexteSDL* ctx, config* conf, int total_joueurs);

#endif
