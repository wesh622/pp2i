#ifndef SDL_BOUCLE_H
#define SDL_BOUCLE_H

#include "plateau.h"
#include "pioche.h"
#include "argc.h"

void boucle_sdl_principale(Plateau* plateau, Pioche* pioche,
                            config* conf, int total_joueurs);

#endif
