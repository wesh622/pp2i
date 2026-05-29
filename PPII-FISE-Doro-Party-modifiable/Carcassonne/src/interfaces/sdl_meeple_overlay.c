#include "sdl_meeple_overlay.h"
#include "sdl_rendu.h"
#include <SDL2/SDL.h>

#define ZOOM 240  // taille de la tuile zoomée (4x TAILLE_CASE)

// coordonnées relatives des zones cliquables dans la vue zoomée
static SDL_Rect zone_zoom(int emplacement) {
    int tiers = ZOOM / 3;
    switch (emplacement) {
        case 1: return (SDL_Rect){tiers,      0,           tiers, tiers};  // Nord
        case 2: return (SDL_Rect){ZOOM-tiers, tiers,       tiers, tiers};  // Est
        case 3: return (SDL_Rect){tiers,      ZOOM-tiers,  tiers, tiers};  // Sud
        case 4: return (SDL_Rect){0,          tiers,       tiers, tiers};  // Ouest
        case 5: return (SDL_Rect){tiers,      tiers,       tiers, tiers};  // Centre
        default: return (SDL_Rect){0,0,0,0};
    }
}

int sdl_choisir_emplacement_meeple(ContexteSDL* ctx, Tuiles t, SDL_Color col_joueur) {
    SDL_Renderer* r = ctx->renderer;

    // zone d'affichage du plateau (sans le panel)
    int largeur_jeu = TAILLE_CASE * (2*RAYON_VUE+1);

    // fond noir semi-transparent sur toute la fenêtre
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
    SDL_Rect ecran = {0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE};
    SDL_RenderFillRect(r, &ecran);

    // position de la tuile zoomée centrée dans la zone plateau
    int zpx = largeur_jeu/2 - ZOOM/2;
    int zpy = HAUTEUR_FENETRE/2 - ZOOM/2;

    // dessiner la tuile en grand avec labels de zones
    sdl_dessiner_tuile_zoom(r, ctx->police_grande, t, zpx, zpy, ZOOM);

    // surligner les 5 zones cliquables avec la couleur du joueur
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    for (int e = 1; e <= 5; e++) {
        SDL_Rect z = zone_zoom(e);
        z.x += zpx; z.y += zpy;
        SDL_SetRenderDrawColor(r, col_joueur.r, col_joueur.g, col_joueur.b, 80);
        SDL_RenderFillRect(r, &z);
        SDL_SetRenderDrawColor(r, col_joueur.r, col_joueur.g, col_joueur.b, 255);
        SDL_RenderDrawRect(r, &z);
    }

    // titre + aide
    sdl_texte(r, ctx->police_grande, "Ou placer votre meeple ?",
              (SDL_Color){255,255,255,255}, largeur_jeu/2, zpy - 25);
    sdl_texte(r, ctx->police, "Clic = poser   Espace = passer",
              (SDL_Color){180,180,180,255}, largeur_jeu/2, zpy + ZOOM + 20);

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
            int mx = evt.button.x - zpx;
            int my = evt.button.y - zpy;
            for (int e = 1; e <= 5; e++) {
                SDL_Rect z = zone_zoom(e);
                if (mx >= z.x && mx < z.x + z.w &&
                    my >= z.y && my < z.y + z.h) return e;
            }
        }
    }
    return 0;
}
