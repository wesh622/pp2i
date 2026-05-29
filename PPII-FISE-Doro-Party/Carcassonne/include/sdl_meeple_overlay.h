#ifndef SDL_MEEPLE_OVERLAY_H
#define SDL_MEEPLE_OVERLAY_H

#include "sdl_init.h"
#include "tuiles.h"

// zoom centré sur la tuile — retourne emplacement (1=N,2=E,3=S,4=O,5=Centre) ou 0 si on passe
// interdit[5] : interdit[e-1]==1 => zone grisée et non cliquable
int sdl_choisir_emplacement_meeple(ContexteSDL* ctx, Tuiles t,
                                   SDL_Color col_joueur, int interdit[5]);

#endif
