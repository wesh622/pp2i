#ifndef SDL_RENDU_H
#define SDL_RENDU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "tuiles.h"
#include "sdl_init.h"

SDL_Color   couleur_sdl_face(int face);
const char* symbole_face(int face);
SDL_Color   couleur_joueur(int idx);  /* idx 0-indexé (joueur 1 = idx 0) */

void sdl_dessiner_tuile(ContexteSDL* ctx,
                        Tuiles t, int px, int py, int surbrillance);
void sdl_dessiner_tuile_zoom(ContexteSDL* ctx,
                             Tuiles t, int zpx, int zpy, int zoom);
void sdl_dessiner_case_vide(SDL_Renderer* r, int px, int py, int valide);
void sdl_texte(SDL_Renderer* r, TTF_Font* police,
               const char* texte, SDL_Color col, int px, int py);
void sdl_texte_g(SDL_Renderer* r, TTF_Font* police,
                 const char* texte, SDL_Color col, int x, int y);

#endif
