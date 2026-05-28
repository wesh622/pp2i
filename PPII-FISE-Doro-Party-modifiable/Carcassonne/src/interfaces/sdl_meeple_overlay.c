#include "sdl_meeple_overlay.h"
#include "sdl_rendu.h"
#include <SDL2/SDL.h>

static SDL_Rect zone_relative(int emplacement) {
    int tc    = TAILLE_CASE;
    int tiers = tc / 3;
    switch (emplacement) {
        case 1: return (SDL_Rect){tiers,      0,          tiers, tiers};  // Nord
        case 2: return (SDL_Rect){tc - tiers, tiers,      tiers, tiers};  // Est
        case 3: return (SDL_Rect){tiers,      tc - tiers, tiers, tiers};  // Sud
        case 4: return (SDL_Rect){0,          tiers,      tiers, tiers};  // Ouest
        case 5: return (SDL_Rect){tiers,      tiers,      tiers, tiers};  // Centre
        default: return (SDL_Rect){0,0,0,0};
    }
}

int sdl_choisir_emplacement_meeple(ContexteSDL* ctx, Tuiles t, int px, int py) {
    (void)t;
    SDL_Renderer* r = ctx->renderer;
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);

    for (int e = 1; e <= 5; e++) {
        SDL_Rect z = zone_relative(e);
        z.x += px; z.y += py;
        SDL_SetRenderDrawColor(r, 255, 255, 0, 100);
        SDL_RenderFillRect(r, &z);
        SDL_SetRenderDrawColor(r, 255, 200, 0, 255);
        SDL_RenderDrawRect(r, &z);
    }
    sdl_texte(r, ctx->police, "Clic=meeple  Espace=passer",
              (SDL_Color){255,255,255,255}, px + TAILLE_CASE/2, py - 12);
    SDL_RenderPresent(r);

    SDL_Event evt;
    while (SDL_WaitEvent(&evt)) {
        if (evt.type == SDL_QUIT) return 0;
        if (evt.type == SDL_KEYDOWN) {
            if (evt.key.keysym.sym == SDLK_SPACE  ||
                evt.key.keysym.sym == SDLK_RETURN ||
                evt.key.keysym.sym == SDLK_ESCAPE) return 0;
        }
        if (evt.type == SDL_MOUSEBUTTONDOWN &&
            evt.button.button == SDL_BUTTON_LEFT) {
            int mx = evt.button.x - px;
            int my = evt.button.y - py;
            for (int e = 1; e <= 5; e++) {
                SDL_Rect z = zone_relative(e);
                if (mx >= z.x && mx < z.x + z.w &&
                    my >= z.y && my < z.y + z.h) return e;
            }
        }
    }
    return 0;
}
