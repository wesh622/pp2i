#include "sdl_fin.h"
#include "sdl_rendu.h"
#include <SDL2/SDL.h>
#include <stdio.h>

// partie Antoine
void sdl_afficher_fin_de_partie(ContexteSDL* ctx, Joueur joueurs[], int nb_joueurs) {
    SDL_Renderer* r = ctx->renderer;

    int max_score = -1, gagnant = 0;
    for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].actif && joueurs[i].score > max_score) {
            max_score = joueurs[i].score;
            gagnant   = i;
        }
    }

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 210);
    SDL_Rect ecran = {0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE};
    SDL_RenderFillRect(r, &ecran);

    int cy = HAUTEUR_FENETRE/2 - nb_joueurs*12 - 50;
    char buf[64];

    sdl_texte(r, ctx->police_grande, "=== FIN DE PARTIE ===",
              (SDL_Color){255,220,60,255}, LARGEUR_FENETRE/2, cy);
    cy += 40;

    snprintf(buf, sizeof(buf), "Felicitations %s !", joueurs[gagnant].nom);
    sdl_texte(r, ctx->police_grande, buf,
              (SDL_Color){80,240,80,255}, LARGEUR_FENETRE/2, cy);
    cy += 35;

    for (int i = 0; i < nb_joueurs; i++) {
        if (!joueurs[i].actif) continue;
        SDL_Color col = (i == gagnant)
            ? (SDL_Color){255,220,60,255}
            : (SDL_Color){200,200,200,255};
        snprintf(buf, sizeof(buf), "%-14s  %4d pts", joueurs[i].nom, joueurs[i].score);
        sdl_texte(r, ctx->police, buf, col, LARGEUR_FENETRE/2, cy);
        cy += 20;
    }

    cy += 20;
    sdl_texte(r, ctx->police, "Espace pour quitter",
              (SDL_Color){130,130,130,255}, LARGEUR_FENETRE/2, cy);

    SDL_RenderPresent(r);

    SDL_Event evt;
    while (SDL_WaitEvent(&evt)) {
        if (evt.type == SDL_QUIT) break;
        if (evt.type == SDL_KEYDOWN &&
            (evt.key.keysym.sym == SDLK_SPACE ||
             evt.key.keysym.sym == SDLK_ESCAPE)) break;
    }
}
