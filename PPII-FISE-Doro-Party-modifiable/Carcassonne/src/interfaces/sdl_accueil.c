#include "sdl_accueil.h"
#include "sdl_rendu.h"
#include "tuiles.h"
#include <SDL2/SDL.h>
#include <stdio.h>

// raccourci pour écrire une ligne à gauche
static void ligne(SDL_Renderer* r, TTF_Font* police,
                  const char* texte, SDL_Color col, int x, int* y, int pas) {
    sdl_texte_g(r, police, texte, col, x, *y);
    *y += pas;
}

// carré de couleur + label pour la légende
static void legende_zone(SDL_Renderer* r, TTF_Font* police,
                         int face, int x, int* y) {
    SDL_Color col = couleur_sdl_face(face);
    SDL_SetRenderDrawColor(r, col.r, col.g, col.b, col.a);
    SDL_Rect rect = {x, *y, 16, 16};
    SDL_RenderFillRect(r, &rect);
    SDL_SetRenderDrawColor(r, 200, 200, 200, 255);
    SDL_RenderDrawRect(r, &rect);
    sdl_texte_g(r, police, symbole_face(face), (SDL_Color){220,220,220,255}, x + 24, *y);
    *y += 20;
}

void sdl_afficher_accueil(ContexteSDL* ctx, config* conf, int total_joueurs) {
    SDL_Renderer* r = ctx->renderer;

    SDL_SetRenderDrawColor(r, 12, 12, 18, 255);
    SDL_RenderClear(r);

    int cx = LARGEUR_FENETRE / 2;
    char buf[128];

    // ── TITRE ──────────────────────────────────────────────
    sdl_texte(r, ctx->police_grande, "C A R C A S S O N N E",
              (SDL_Color){255, 220, 60, 255}, cx, 28);

    // ── CONFIG ─────────────────────────────────────────────
    int x = 40;
    int y = 60;

    sdl_texte_g(r, ctx->police_grande, "Configuration de la partie",
                (SDL_Color){200,200,200,255}, x, y);
    y += 24;

    snprintf(buf, sizeof(buf), "  Joueurs humains : %d", conf->nbr_joueur);
    sdl_texte_g(r, ctx->police, buf, (SDL_Color){180,255,180,255}, x, y); y += 17;
    snprintf(buf, sizeof(buf), "  Joueurs IA      : %d", conf->ai);
    sdl_texte_g(r, ctx->police, buf, (SDL_Color){180,255,180,255}, x, y); y += 17;
    snprintf(buf, sizeof(buf), "  Seed            : %d", conf->seed);
    sdl_texte_g(r, ctx->police, buf, (SDL_Color){180,255,180,255}, x, y); y += 17;
    if (conf->max_turn < 0)
        snprintf(buf, sizeof(buf), "  Tours max       : illimite");
    else
        snprintf(buf, sizeof(buf), "  Tours max       : %d", conf->max_turn);
    sdl_texte_g(r, ctx->police, buf, (SDL_Color){180,255,180,255}, x, y); y += 17;

    // couleurs joueurs
    y += 6;
    sdl_texte_g(r, ctx->police, "  Couleurs :", (SDL_Color){180,180,180,255}, x, y); y += 17;
    for (int k = 0; k < total_joueurs; k++) {
        SDL_Color col = couleur_joueur(k);
        SDL_SetRenderDrawColor(r, col.r, col.g, col.b, 255);
        SDL_Rect rect = {x + 16, y + 1, 14, 14};
        SDL_RenderFillRect(r, &rect);
        snprintf(buf, sizeof(buf), "     %s%s", conf->tab[k].nom,
                 conf->tab[k].est_IA ? " (IA)" : "");
        sdl_texte_g(r, ctx->police, buf, col, x + 34, y);
        y += 17;
    }

    // ── LÉGENDE ────────────────────────────────────────────
    int x2 = 320;
    int y2 = 60;

    sdl_texte_g(r, ctx->police_grande, "Legende des zones",
                (SDL_Color){200,200,200,255}, x2, y2); y2 += 24;

    legende_zone(r, ctx->police, ROUTE_PRAIRIE,  x2, &y2);
    legende_zone(r, ctx->police, PRAIRIE,        x2, &y2);
    legende_zone(r, ctx->police, VILLE,          x2, &y2);
    legende_zone(r, ctx->police, VILLE_BOUCLIER, x2, &y2);
    legende_zone(r, ctx->police, ABBAYE,         x2, &y2);
    legende_zone(r, ctx->police, CARREFOUR,      x2, &y2);

    sdl_texte_g(r, ctx->police, "Chaque tuile affiche : Nord / Centre / Sud",
                (SDL_Color){140,140,140,255}, x2, y2); y2 += 17;
    sdl_texte_g(r, ctx->police, "Les cases VERTES sont les placements valides",
                (SDL_Color){80,200,80,255}, x2, y2);

    // ── RÈGLES ─────────────────────────────────────────────
    int x3 = 580;
    int y3 = 60;

    sdl_texte_g(r, ctx->police_grande, "Regles du jeu",
                (SDL_Color){200,200,200,255}, x3, y3); y3 += 24;

    SDL_Color rc = {210, 210, 210, 255};
    int pas = 16;
    ligne(r, ctx->police, "1. Piochez une tuile a chaque tour.", rc, x3, &y3, pas);
    ligne(r, ctx->police, "2. Placez-la adjacent a une tuile existante.", rc, x3, &y3, pas);
    ligne(r, ctx->police, "   Les zones en contact doivent etre", rc, x3, &y3, pas);
    ligne(r, ctx->police, "   compatibles (route-route, ville-ville...).", rc, x3, &y3, pas);
    ligne(r, ctx->police, "3. Posez un meeple sur une zone LIBRE.", rc, x3, &y3, pas);
    ligne(r, ctx->police, "   Pas de meeple si la zone est deja occupee.", rc, x3, &y3, pas);
    ligne(r, ctx->police, "4. Route / Ville / Abbaye COMPLETE :", rc, x3, &y3, pas);
    ligne(r, ctx->police, "   les meeples sont recuperes + score.", rc, x3, &y3, pas);
    ligne(r, ctx->police, "5. Fin de partie : pioche vide ou tours max.", rc, x3, &y3, pas);
    ligne(r, ctx->police, "   Structures incompletes = score reduit.", rc, x3, &y3, pas);
    y3 += 6;
    ligne(r, ctx->police, "Chaque joueur a 7 meeples.", (SDL_Color){200,150,240,255}, x3, &y3, pas);
    ligne(r, ctx->police, "Si une tuile est impossible a poser,", (SDL_Color){200,150,240,255}, x3, &y3, pas);
    ligne(r, ctx->police, "elle est ecartee et la suivante est piochee.", (SDL_Color){200,150,240,255}, x3, &y3, pas);

    // ── CONTRÔLES ──────────────────────────────────────────
    int yc = 460;
    sdl_texte_g(r, ctx->police_grande, "Controles SDL",
                (SDL_Color){200,200,200,255}, x, yc); yc += 24;

    SDL_Color cc = {160, 200, 255, 255};
    ligne(r, ctx->police, "Clic gauche sur case verte  -> poser la tuile", cc, x, &yc, pas);
    ligne(r, ctx->police, "R                           -> tourner droite", cc, x, &yc, pas);
    ligne(r, ctx->police, "L                           -> tourner gauche", cc, x, &yc, pas);
    ligne(r, ctx->police, "Fleches                     -> deplacer la vue", cc, x, &yc, pas);
    ligne(r, ctx->police, "Clic zone surlignee         -> placer meeple", cc, x, &yc, pas);
    ligne(r, ctx->police, "Espace                      -> passer meeple", cc, x, &yc, pas);
    ligne(r, ctx->police, "Echap                       -> quitter", cc, x, &yc, pas);

    // ── SCORE RAPIDE ───────────────────────────────────────
    int ys = 460;
    sdl_texte_g(r, ctx->police_grande, "Scoring",
                (SDL_Color){200,200,200,255}, x3, ys); ys += 24;

    SDL_Color sc = {230, 200, 100, 255};
    ligne(r, ctx->police, "Route complete    : 1 pt / tuile", sc, x3, &ys, pas);
    ligne(r, ctx->police, "Ville complete    : 2 pts / tuile (+2/bouclier)", sc, x3, &ys, pas);
    ligne(r, ctx->police, "Abbaye complete   : 9 pts (elle + 8 voisines)", sc, x3, &ys, pas);
    ligne(r, ctx->police, "Fin de partie :", sc, x3, &ys, pas);
    ligne(r, ctx->police, "  Route / Ville   : 1 pt / tuile", sc, x3, &ys, pas);
    ligne(r, ctx->police, "  Abbaye          : 1 + nb voisines", sc, x3, &ys, pas);
    ligne(r, ctx->police, "Majorite meeples  : le joueur majoritaire", sc, x3, &ys, pas);
    ligne(r, ctx->police, "  dans une zone scored tous les points.", sc, x3, &ys, pas);

    // ── DÉMARRER ───────────────────────────────────────────
    sdl_texte(r, ctx->police_grande, "Appuyez sur ESPACE pour demarrer",
              (SDL_Color){255, 220, 60, 255}, cx, HAUTEUR_FENETRE - 30);

    SDL_RenderPresent(r);

    SDL_Event evt;
    while (SDL_WaitEvent(&evt)) {
        if (evt.type == SDL_QUIT) return;
        if (evt.type == SDL_KEYDOWN &&
            (evt.key.keysym.sym == SDLK_SPACE ||
             evt.key.keysym.sym == SDLK_RETURN)) return;
    }
}
