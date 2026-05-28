#ifndef SDL_MEEPLE_OVERLAY_H
#define SDL_MEEPLE_OVERLAY_H

#include "sdl_init.h"
#include "tuiles.h"

// retourne l'emplacement choisi (1=N,2=E,3=S,4=O,5=Centre) ou 0 si on passe
int sdl_choisir_emplacement_meeple(ContexteSDL* ctx, Tuiles t, int px, int py);

#endif
