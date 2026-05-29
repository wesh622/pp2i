#include "sdl_rendu.h"
#include "tuiles.h"
#include <stdio.h>

SDL_Color couleur_sdl_face(int face) {
    switch (face) {
        case ROUTE_PRAIRIE:  return (SDL_Color){210, 180,  80, 255};
        case PRAIRIE:        return (SDL_Color){ 80, 160,  60, 255};
        case VILLE:          return (SDL_Color){ 60, 100, 200, 255};
        case VILLE_BOUCLIER: return (SDL_Color){ 40, 200, 200, 255};
        case ABBAYE:         return (SDL_Color){160,  60, 200, 255};
        case CARREFOUR:      return (SDL_Color){200,  60,  60, 255};
        default:             return (SDL_Color){150, 150, 150, 255};
    }
}

const char* symbole_face(int face) {
    switch (face) {
        case ROUTE_PRAIRIE:  return "ROUTE";
        case PRAIRIE:        return "PRAIRIE";
        case VILLE:          return "VILLE";
        case VILLE_BOUCLIER: return "VILLE+B";
        case ABBAYE:         return "ABBAYE";
        case CARREFOUR:      return "CARREFOUR";
        default:             return "?";
    }
}

/* Déduit le nombre de rotations 90° CW appliquées à la tuile */
static int deduire_rotation(Tuiles t) {
    if (t.id < 1 || t.id > 72) return 0;
    Tuiles orig = tuiles_jeu[t.id - 1];
    if (t.a == orig.a && t.b == orig.b) return 0;
    if (t.a == orig.d && t.b == orig.a) return 1;
    if (t.a == orig.c && t.b == orig.d) return 2;
    if (t.a == orig.b && t.b == orig.c) return 3;
    return 0;
}

/* Rendu de repli (tuile colorée par zones) */
static void dessiner_tuile_fallback(SDL_Renderer* r, TTF_Font* police,
                                    Tuiles t, int px, int py,
                                    int tc, int surbrillance) {
    int tiers = tc / 3;

    SDL_Color cc = couleur_sdl_face(t.center);
    SDL_SetRenderDrawColor(r, cc.r, cc.g, cc.b, cc.a);
    SDL_Rect fond = {px, py, tc, tc};
    SDL_RenderFillRect(r, &fond);

    struct { int face; SDL_Rect rect; } bandes[4] = {
        { t.a, {px,          py,          tc,    tiers} },
        { t.c, {px,          py+tc-tiers, tc,    tiers} },
        { t.d, {px,          py,          tiers, tc   } },
        { t.b, {px+tc-tiers, py,          tiers, tc   } },
    };
    for (int k = 0; k < 4; k++) {
        SDL_Color c = couleur_sdl_face(bandes[k].face);
        SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
        SDL_RenderFillRect(r, &bandes[k].rect);
    }

    SDL_SetRenderDrawColor(r,
        surbrillance ? 255 : 40,
        surbrillance ? 255 : 40,
        surbrillance ?   0 : 40,
        255);
    SDL_RenderDrawRect(r, &fond);

    if (police) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", t.id);
        sdl_texte(r, police, buf, (SDL_Color){255,255,255,255}, px + tc/2, py + tc/2);
    }
}

void sdl_dessiner_tuile(ContexteSDL* ctx,
                        Tuiles t, int px, int py, int surbrillance) {
    SDL_Renderer* r = ctx->renderer;
    int tc = TAILLE_CASE;

    if (t.id >= 1 && t.id <= 72 && ctx->textures_tuiles[t.id]) {
        double angle = deduire_rotation(t) * 90.0;
        SDL_Rect dst = {px, py, tc, tc};
        SDL_RenderCopyEx(r, ctx->textures_tuiles[t.id], NULL, &dst,
                         angle, NULL, SDL_FLIP_NONE);
        SDL_SetRenderDrawColor(r,
            surbrillance ? 255 : 40,
            surbrillance ? 255 : 40,
            surbrillance ?   0 : 40,
            255);
        SDL_RenderDrawRect(r, &dst);
    } else {
        dessiner_tuile_fallback(r, ctx->police, t, px, py, tc, surbrillance);
    }
}

void sdl_dessiner_tuile_zoom(ContexteSDL* ctx,
                             Tuiles t, int zpx, int zpy, int zoom) {
    SDL_Renderer* r = ctx->renderer;

    if (t.id >= 1 && t.id <= 72 && ctx->textures_tuiles[t.id]) {
        double angle = deduire_rotation(t) * 90.0;
        SDL_Rect dst = {zpx, zpy, zoom, zoom};
        SDL_RenderCopyEx(r, ctx->textures_tuiles[t.id], NULL, &dst,
                         angle, NULL, SDL_FLIP_NONE);
        SDL_SetRenderDrawColor(r, 200, 200, 200, 255);
        SDL_RenderDrawRect(r, &dst);
    } else {
        int tiers = zoom / 3;
        SDL_Color cc = couleur_sdl_face(t.center);
        SDL_SetRenderDrawColor(r, cc.r, cc.g, cc.b, cc.a);
        SDL_Rect fond = {zpx, zpy, zoom, zoom};
        SDL_RenderFillRect(r, &fond);

        struct { int face; SDL_Rect rect; } bandes[4] = {
            { t.a, {zpx,            zpy,              zoom,  tiers} },
            { t.c, {zpx,            zpy+zoom-tiers,   zoom,  tiers} },
            { t.d, {zpx,            zpy,              tiers, zoom } },
            { t.b, {zpx+zoom-tiers, zpy,              tiers, zoom } },
        };
        for (int k = 0; k < 4; k++) {
            SDL_Color c = couleur_sdl_face(bandes[k].face);
            SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
            SDL_RenderFillRect(r, &bandes[k].rect);
        }
        SDL_SetRenderDrawColor(r, 200, 200, 200, 255);
        SDL_RenderDrawRect(r, &fond);

        if (ctx->police_grande) {
            sdl_texte(r, ctx->police_grande, "NORD",
                      (SDL_Color){255,255,255,200}, zpx+zoom/2,      zpy+tiers/2);
            sdl_texte(r, ctx->police_grande, "EST",
                      (SDL_Color){255,255,255,200}, zpx+zoom-tiers/2, zpy+zoom/2);
            sdl_texte(r, ctx->police_grande, "SUD",
                      (SDL_Color){255,255,255,200}, zpx+zoom/2,      zpy+zoom-tiers/2);
            sdl_texte(r, ctx->police_grande, "OUEST",
                      (SDL_Color){255,255,255,200}, zpx+tiers/2,     zpy+zoom/2);
            sdl_texte(r, ctx->police_grande, "CENTRE",
                      (SDL_Color){255,255,255,200}, zpx+zoom/2,      zpy+zoom/2);
        }
    }
}

void sdl_dessiner_case_vide(SDL_Renderer* r, int px, int py, int valide) {
    SDL_Rect rect = {px, py, TAILLE_CASE, TAILLE_CASE};
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    if (valide) {
        SDL_SetRenderDrawColor(r, 40, 90, 40, 180);
        SDL_RenderFillRect(r, &rect);
        SDL_SetRenderDrawColor(r, 80, 200, 80, 255);
    } else {
        SDL_SetRenderDrawColor(r, 25, 25, 25, 255);
        SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
        SDL_RenderFillRect(r, &rect);
        SDL_SetRenderDrawColor(r, 50, 50, 50, 255);
    }
    SDL_RenderDrawRect(r, &rect);
}

SDL_Color couleur_joueur(int idx) {
    switch (idx % 5) {
        case 0: return (SDL_Color){220,  50,  50, 255};
        case 1: return (SDL_Color){ 60, 120, 220, 255};
        case 2: return (SDL_Color){ 50, 200,  80, 255};
        case 3: return (SDL_Color){230, 180,  20, 255};
        case 4: return (SDL_Color){180,  50, 200, 255};
        default: return (SDL_Color){200, 200, 200, 255};
    }
}

void sdl_texte(SDL_Renderer* r, TTF_Font* police,
               const char* texte, SDL_Color col, int px, int py) {
    if (!police || !texte) return;
    SDL_Surface* surf = TTF_RenderText_Solid(police, texte, col);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    if (tex) {
        SDL_Rect dst = {px - surf->w/2, py - surf->h/2, surf->w, surf->h};
        SDL_RenderCopy(r, tex, NULL, &dst);
        SDL_DestroyTexture(tex);
    }
    SDL_FreeSurface(surf);
}

void sdl_texte_g(SDL_Renderer* r, TTF_Font* police,
                 const char* texte, SDL_Color col, int x, int y) {
    if (!police || !texte) return;
    SDL_Surface* surf = TTF_RenderText_Solid(police, texte, col);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    if (tex) {
        SDL_Rect dst = {x, y, surf->w, surf->h};
        SDL_RenderCopy(r, tex, NULL, &dst);
        SDL_DestroyTexture(tex);
    }
    SDL_FreeSurface(surf);
}
