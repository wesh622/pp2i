#ifndef SDL_BOUCLE_H
#define SDL_BOUCLE_H

#include "plateau.h"
#include "pioche.h"
#include "argc.h"
#include "sdl_init.h"      
#include "sdl_plateau.h"

static void tout_afficher(ContexteSDL* ctx, Plateau* plateau, Pioche* pioche,config* conf, Tuiles* tuile, int tour,int total_joueurs, Joueur* j, VueSDL* vue);
static int tuile_placable_quelque_part(Plateau* p, Tuiles* t);
static void calculer_zones_interdites(config* conf, int total_joueurs,int x, int y, int interdit[5]);
void boucle_sdl_principale(Plateau* plateau, Pioche* pioche,config* conf, int total_joueurs);



#endif
