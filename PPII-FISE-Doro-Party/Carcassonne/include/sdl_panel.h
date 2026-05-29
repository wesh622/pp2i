#ifndef SDL_PANEL_H
#define SDL_PANEL_H

#include "sdl_init.h"
#include "joueur.h"
#include "pioche.h"
#include "tuiles.h"

void sdl_afficher_panel(ContexteSDL* ctx, Joueur* j, Pioche* pioche,
                        Tuiles* tuile_courante, int tour,
                        int total_joueurs, Joueur* tous_joueurs);

#endif
