#ifndef SDL_RENDU_H
#define SDL_RENDU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "tuiles.h"
#include "sdl_init.h"

SDL_Color   couleur_sdl_face(int face);
const char* symbole_face(int face);

void sdl_dessiner_tuile(SDL_Renderer* r, TTF_Font* police,
                        Tuiles t, int px, int py, int surbrillance);
void sdl_dessiner_case_vide(SDL_Renderer* r, int px, int py, int valide);
void sdl_texte(SDL_Renderer* r, TTF_Font* police,
               const char* texte, SDL_Color col, int px, int py);

#endif
