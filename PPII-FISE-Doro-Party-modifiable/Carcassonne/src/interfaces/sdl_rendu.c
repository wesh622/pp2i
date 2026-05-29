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

void sdl_dessiner_tuile(SDL_Renderer* r, TTF_Font* police,
                        Tuiles t, int px, int py, int surbrillance) {
    int tc    = TAILLE_CASE;
    int tiers = tc / 3;

    // fond centre
    SDL_Color cc = couleur_sdl_face(t.center);
    SDL_SetRenderDrawColor(r, cc.r, cc.g, cc.b, cc.a);
    SDL_Rect fond = {px, py, tc, tc};
    SDL_RenderFillRect(r, &fond);

    // 4 bandes directionnelles
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

    // bordure
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
        case 0: return (SDL_Color){220,  50,  50, 255};  // rouge
        case 1: return (SDL_Color){ 60, 120, 220, 255};  // bleu
        case 2: return (SDL_Color){ 50, 200,  80, 255};  // vert
        case 3: return (SDL_Color){230, 180,  20, 255};  // jaune
        case 4: return (SDL_Color){180,  50, 200, 255};  // violet
        default: return (SDL_Color){200, 200, 200, 255};
    }
}

void sdl_dessiner_tuile_zoom(SDL_Renderer* r, TTF_Font* police,
                             Tuiles t, int zpx, int zpy, int zoom) {
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

    if (police) {
        sdl_texte(r, police, "NORD",   (SDL_Color){255,255,255,200}, zpx+zoom/2,     zpy+tiers/2);
        sdl_texte(r, police, "EST",    (SDL_Color){255,255,255,200}, zpx+zoom-tiers/2, zpy+zoom/2);
        sdl_texte(r, police, "SUD",    (SDL_Color){255,255,255,200}, zpx+zoom/2,     zpy+zoom-tiers/2);
        sdl_texte(r, police, "OUEST",  (SDL_Color){255,255,255,200}, zpx+tiers/2,    zpy+zoom/2);
        sdl_texte(r, police, "CENTRE", (SDL_Color){255,255,255,200}, zpx+zoom/2,     zpy+zoom/2);
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
