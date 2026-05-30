#include "sdl_init.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// partie Moran

// contexte SDL : fenetre, renderer, polices, textures...
static const char* POLICES[] = {
    "ressources/font.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    "/usr/share/fonts/TTF/DejaVuSans.ttf",
    "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
    NULL
};

// charge une police à partir d'une liste de chemins possibles
static TTF_Font* charger_police(int taille) {
    for (int i = 0; POLICES[i]; i++) {
        TTF_Font* f = TTF_OpenFont(POLICES[i], taille);
        if (f) return f;
    }
    fprintf(stderr, "Police non trouvée, texte SDL désactivé\n");
    return NULL;
}

// initialisation et fermeture de SDL
ContexteSDL* init_sdl(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init : %s\n", SDL_GetError());
        return NULL;
    }
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        return NULL;
    }

    ContexteSDL* ctx = malloc(sizeof(*ctx));
    assert(ctx != NULL);

    ctx->fenetre = SDL_CreateWindow(
        "Carcassonne",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        LARGEUR_FENETRE, HAUTEUR_FENETRE,
        SDL_WINDOW_SHOWN
    );
    if (!ctx->fenetre) {
        fprintf(stderr, "Fenetre : %s\n", SDL_GetError());
        free(ctx); TTF_Quit(); SDL_Quit(); return NULL;
    }

    ctx->renderer = SDL_CreateRenderer(ctx->fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (!ctx->renderer) {
        fprintf(stderr, "Renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(ctx->fenetre);
        free(ctx); TTF_Quit(); SDL_Quit(); return NULL;
    }

    ctx->police        = charger_police(12);
    ctx->police_grande = charger_police(18);

    IMG_Init(IMG_INIT_PNG);
    memset(ctx->textures_tuiles, 0, sizeof(ctx->textures_tuiles));
    ctx->texture_meeple = NULL;

    for (int id = 1; id <= 72; id++) {
        char chemin[64];
        snprintf(chemin, sizeof(chemin), "assets/tile_%d.png", id);
        SDL_Surface* surf = IMG_Load(chemin);
        if (surf) {
            ctx->textures_tuiles[id] = SDL_CreateTextureFromSurface(ctx->renderer, surf);
            SDL_FreeSurface(surf);
        }
    }

    SDL_Surface* surf_m = IMG_Load("assets/meeple_int.png");
    if (surf_m) {
        ctx->texture_meeple = SDL_CreateTextureFromSurface(ctx->renderer, surf_m);
        SDL_SetTextureBlendMode(ctx->texture_meeple, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(surf_m);
    }

    return ctx;
}

// libère les ressources SDL
void close_sdl(ContexteSDL* ctx) {
    if (!ctx) return;
    for (int id = 1; id <= 72; id++)
        if (ctx->textures_tuiles[id]) SDL_DestroyTexture(ctx->textures_tuiles[id]);
    if (ctx->texture_meeple) SDL_DestroyTexture(ctx->texture_meeple);
    if (ctx->police)        TTF_CloseFont(ctx->police);
    if (ctx->police_grande) TTF_CloseFont(ctx->police_grande);
    if (ctx->renderer)      SDL_DestroyRenderer(ctx->renderer);
    if (ctx->fenetre)       SDL_DestroyWindow(ctx->fenetre);
    free(ctx);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

