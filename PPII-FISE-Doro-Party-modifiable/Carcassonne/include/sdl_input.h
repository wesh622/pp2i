#ifndef SDL_INPUT_H
#define SDL_INPUT_H

#include <SDL2/SDL.h>
#include "sdl_plateau.h"

#define ACTION_RIEN      0
#define ACTION_POSER     1
#define ACTION_ROTATION  2
#define ACTION_QUITTER  -1

int sdl_attendre_action(VueSDL* vue, int* i_out, int* j_out, int* rot_out);

#endif
