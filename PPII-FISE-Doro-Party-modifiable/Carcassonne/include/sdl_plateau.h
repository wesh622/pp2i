#ifndef SDL_PLATEAU_H
#define SDL_PLATEAU_H

#include "sdl_init.h"
#include "sdl_rendu.h"
#include "plateau.h"
#include "tuiles.h"

typedef struct _VueSDL {
    int centre_i;
    int centre_j;
} VueSDL;

VueSDL vue_init(void);

void grille_vers_pixel(int i, int j, VueSDL vue, int* px, int* py);
void pixel_vers_grille(int mx, int my, VueSDL vue, int* i, int* j);

void sdl_afficher_plateau(ContexteSDL* ctx, Plateau* p,
                          Tuiles* tuile_courante, VueSDL vue);

#endif
