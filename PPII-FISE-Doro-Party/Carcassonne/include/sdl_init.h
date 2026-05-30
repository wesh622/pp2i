#ifndef SDL_INIT_H
#define SDL_INIT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define TAILLE_CASE     60
#define RAYON_VUE        6
#define LARGEUR_PANEL  300
#define LARGEUR_FENETRE (TAILLE_CASE * (2*RAYON_VUE+1) + LARGEUR_PANEL)
#define HAUTEUR_FENETRE (TAILLE_CASE * (2*RAYON_VUE+1))

typedef struct _ContexteSDL {
    SDL_Window*   fenetre;
    SDL_Renderer* renderer;
    TTF_Font*     police;
    TTF_Font*     police_grande;
    SDL_Texture*  textures_tuiles[73];  /* indices 1..72 */
    SDL_Texture*  texture_meeple;
} ContexteSDL;

ContexteSDL* init_sdl(void);
void         close_sdl(ContexteSDL* ctx);

#endif
