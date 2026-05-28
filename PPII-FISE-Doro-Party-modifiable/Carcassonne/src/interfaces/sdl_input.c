#include "sdl_input.h"

int sdl_attendre_action(VueSDL* vue, int* i_out, int* j_out, int* rot_out) {
    *rot_out = 0;
    *i_out   = -1;
    *j_out   = -1;

    SDL_Event evt;
    if (!SDL_WaitEvent(&evt)) return ACTION_RIEN;

    switch (evt.type) {
        case SDL_QUIT:
            return ACTION_QUITTER;

        case SDL_KEYDOWN:
            switch (evt.key.keysym.sym) {
                case SDLK_ESCAPE: return ACTION_QUITTER;
                case SDLK_r: *rot_out = 1; return ACTION_ROTATION;
                case SDLK_l: *rot_out = 2; return ACTION_ROTATION;
                case SDLK_UP:
                    if (vue->centre_i > RAYON_VUE) vue->centre_i--;
                    break;
                case SDLK_DOWN:
                    if (vue->centre_i < TAILLE_MAX - RAYON_VUE - 1) vue->centre_i++;
                    break;
                case SDLK_LEFT:
                    if (vue->centre_j > RAYON_VUE) vue->centre_j--;
                    break;
                case SDLK_RIGHT:
                    if (vue->centre_j < TAILLE_MAX - RAYON_VUE - 1) vue->centre_j++;
                    break;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (evt.button.button == SDL_BUTTON_LEFT) {
                pixel_vers_grille(evt.button.x, evt.button.y, *vue, i_out, j_out);
                if (*i_out >= 0) return ACTION_POSER;
            }
            break;
    }
    return ACTION_RIEN;
}
