#include "sdl_accueil.h"
#include "sdl_rendu.h"
#include "tuiles.h"
#include <SDL2/SDL.h>
#include <stdio.h>

/* ── helpers ──────────────────────────────────────────────────────────────── */

static void ligne(SDL_Renderer* r, TTF_Font* police,
                  const char* texte, SDL_Color col, int x, int* y, int pas) {
    sdl_texte_g(r, police, texte, col, x, *y);
    *y += pas;
}

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

/* ── page règles ──────────────────────────────────────────────────────────── */

static void afficher_regles(ContexteSDL* ctx) {
    SDL_Renderer* r = ctx->renderer;
    SDL_SetRenderDrawColor(r, 12, 12, 18, 255);
    SDL_RenderClear(r);

    int cx = LARGEUR_FENETRE / 2;
    int pas = 16;

    sdl_texte(r, ctx->police_grande, "C A R C A S S O N N E",
              (SDL_Color){255, 220, 60, 255}, cx, 22);
    sdl_texte(r, ctx->police_grande, "Regles & Controles",
              (SDL_Color){200, 200, 200, 255}, cx, 50);

    /* colonne gauche : règles du jeu */
    int x1 = 30, y1 = 80;

    sdl_texte_g(r, ctx->police_grande, "Regles du jeu",
                (SDL_Color){255, 200, 60, 255}, x1, y1); y1 += 22;

    SDL_Color rc = {210, 210, 210, 255};
    ligne(r, ctx->police, "1. Piochez une tuile a chaque tour.", rc, x1, &y1, pas);
    ligne(r, ctx->police, "2. Placez-la adjacent a une tuile existante.", rc, x1, &y1, pas);
    ligne(r, ctx->police, "   Les zones en contact doivent etre compatibles", rc, x1, &y1, pas);
    ligne(r, ctx->police, "   (route-route, ville-ville, prairie-prairie...).", rc, x1, &y1, pas);
    ligne(r, ctx->police, "3. Posez un meeple sur une zone LIBRE.", rc, x1, &y1, pas);
    ligne(r, ctx->police, "   Impossible si la zone est deja occupee.", rc, x1, &y1, pas);
    ligne(r, ctx->police, "4. Route / Ville / Abbaye COMPLETE :", rc, x1, &y1, pas);
    ligne(r, ctx->police, "   meeples recuperes + score attribue.", rc, x1, &y1, pas);
    ligne(r, ctx->police, "5. Fin : pioche vide ou nombre de tours max.", rc, x1, &y1, pas);
    ligne(r, ctx->police, "   Structures incompletes = score reduit.", rc, x1, &y1, pas);
    y1 += 6;
    ligne(r, ctx->police, "Chaque joueur dispose de 7 meeples.",
          (SDL_Color){200,150,240,255}, x1, &y1, pas);
    ligne(r, ctx->police, "Si une tuile est impossible a placer,",
          (SDL_Color){200,150,240,255}, x1, &y1, pas);
    ligne(r, ctx->police, "elle est ecartee automatiquement.",
          (SDL_Color){200,150,240,255}, x1, &y1, pas);
    y1 += 10;

    sdl_texte_g(r, ctx->police_grande, "Scoring",
                (SDL_Color){255, 200, 60, 255}, x1, y1); y1 += 22;
    SDL_Color sc = {230, 200, 100, 255};
    ligne(r, ctx->police, "Route complete    : 1 pt / tuile", sc, x1, &y1, pas);
    ligne(r, ctx->police, "Ville complete    : 2 pts / tuile (+2/bouclier)", sc, x1, &y1, pas);
    ligne(r, ctx->police, "Abbaye complete   : 9 pts (tuile + 8 voisines)", sc, x1, &y1, pas);
    ligne(r, ctx->police, "Fin de partie :", sc, x1, &y1, pas);
    ligne(r, ctx->police, "  Route / Ville   : 1 pt / tuile", sc, x1, &y1, pas);
    ligne(r, ctx->police, "  Abbaye          : 1 + nb voisines", sc, x1, &y1, pas);
    ligne(r, ctx->police, "Majorite meeples  : le joueur majoritaire score.", sc, x1, &y1, pas);

    /* colonne droite : contrôles + légende */
    int x2 = 430, y2 = 80;

    sdl_texte_g(r, ctx->police_grande, "Controles",
                (SDL_Color){255, 200, 60, 255}, x2, y2); y2 += 22;
    SDL_Color cc = {160, 200, 255, 255};
    ligne(r, ctx->police, "Clic gauche case verte -> poser la tuile", cc, x2, &y2, pas);
    ligne(r, ctx->police, "R                      -> tourner droite", cc, x2, &y2, pas);
    ligne(r, ctx->police, "L                      -> tourner gauche", cc, x2, &y2, pas);
    ligne(r, ctx->police, "Fleches                -> deplacer la vue", cc, x2, &y2, pas);
    ligne(r, ctx->police, "Clic zone (overlay)    -> placer meeple", cc, x2, &y2, pas);
    ligne(r, ctx->police, "Espace                 -> passer le meeple", cc, x2, &y2, pas);
    ligne(r, ctx->police, "Echap                  -> quitter la partie", cc, x2, &y2, pas);
    y2 += 12;

    sdl_texte_g(r, ctx->police_grande, "Legende des zones",
                (SDL_Color){255, 200, 60, 255}, x2, y2); y2 += 22;
    legende_zone(r, ctx->police, ROUTE_PRAIRIE,  x2, &y2);
    legende_zone(r, ctx->police, PRAIRIE,        x2, &y2);
    legende_zone(r, ctx->police, VILLE,          x2, &y2);
    legende_zone(r, ctx->police, VILLE_BOUCLIER, x2, &y2);
    legende_zone(r, ctx->police, ABBAYE,         x2, &y2);
    legende_zone(r, ctx->police, CARREFOUR,      x2, &y2);
    y2 += 8;
    sdl_texte_g(r, ctx->police, "La rotation change l'orientation du terrain :",
                (SDL_Color){140,140,140,255}, x2, y2); y2 += pas;
    sdl_texte_g(r, ctx->police, "les couleurs bougent, le numero reste le meme.",
                (SDL_Color){140,140,140,255}, x2, y2); y2 += pas;
    y2 += 10;
    sdl_texte_g(r, ctx->police, "Zones grises dans l'overlay meeple :",
                (SDL_Color){140,140,140,255}, x2, y2); y2 += pas;
    sdl_texte_g(r, ctx->police, "=> zone deja occupee par un meeple.",
                (SDL_Color){140,140,140,255}, x2, y2);

    /* retour */
    sdl_texte(r, ctx->police_grande, "Appuyez sur ECHAP ou ESPACE pour revenir",
              (SDL_Color){255, 220, 60, 255}, cx, HAUTEUR_FENETRE - 28);

    SDL_RenderPresent(r);

    SDL_Event evt;
    while (SDL_WaitEvent(&evt)) {
        if (evt.type == SDL_QUIT) return;
        if (evt.type == SDL_KEYDOWN) {
            int k = evt.key.keysym.sym;
            if (k == SDLK_ESCAPE || k == SDLK_SPACE || k == SDLK_RETURN) return;
        }
    }
}

/* ── menu principal ───────────────────────────────────────────────────────── */

#define NB_OPTIONS 3
static const char* LABELS[NB_OPTIONS] = { "JOUER", "REGLES", "QUITTER" };

static void dessiner_menu(ContexteSDL* ctx, config* conf, int total_joueurs, int sel) {
    SDL_Renderer* r = ctx->renderer;
    SDL_SetRenderDrawColor(r, 12, 12, 18, 255);
    SDL_RenderClear(r);

    int cx = LARGEUR_FENETRE / 2;
    char buf[128];

    /* titre */
    sdl_texte(r, ctx->police_grande, "C A R C A S S O N N E",
              (SDL_Color){255, 220, 60, 255}, cx, 60);

    /* sous-titre config */
    snprintf(buf, sizeof(buf), "%d joueur(s)  |  %d IA  |  seed %d",
             conf->nbr_joueur, conf->ai, conf->seed);
    sdl_texte(r, ctx->police, buf, (SDL_Color){120, 120, 120, 255}, cx, 100);

    /* couleurs joueurs */
    int ky = 126;
    for (int k = 0; k < total_joueurs; k++) {
        SDL_Color col = couleur_joueur(k);
        SDL_SetRenderDrawColor(r, col.r, col.g, col.b, 255);
        SDL_Rect sq = {cx - 70, ky + k*20, 14, 14};
        SDL_RenderFillRect(r, &sq);
        snprintf(buf, sizeof(buf), "%s%s", conf->tab[k].nom,
                 conf->tab[k].est_IA ? " (IA)" : "");
        sdl_texte_g(r, ctx->police, buf, col, cx - 52, ky + k*20);
    }

    /* options de menu */
    int y_menu = 310;
    int dy     = 65;
    int larg   = 240;
    int haut   = 46;
    for (int i = 0; i < NB_OPTIONS; i++) {
        int y = y_menu + i * dy;
        SDL_Rect rect = {cx - larg/2, y - haut/2, larg, haut};
        if (i == sel) {
            SDL_SetRenderDrawColor(r, 80, 60, 0, 255);
            SDL_RenderFillRect(r, &rect);
            SDL_SetRenderDrawColor(r, 255, 220, 60, 255);
            SDL_RenderDrawRect(r, &rect);
            sdl_texte(r, ctx->police_grande, LABELS[i],
                      (SDL_Color){255, 220, 60, 255}, cx, y);
        } else {
            SDL_SetRenderDrawColor(r, 30, 30, 36, 255);
            SDL_RenderFillRect(r, &rect);
            SDL_SetRenderDrawColor(r, 80, 80, 80, 255);
            SDL_RenderDrawRect(r, &rect);
            sdl_texte(r, ctx->police_grande, LABELS[i],
                      (SDL_Color){150, 150, 150, 255}, cx, y);
        }
    }

    /* aide navigation */
    sdl_texte(r, ctx->police,
              "Haut/Bas : naviguer   Entree : valider   Clic : selectionner",
              (SDL_Color){70, 70, 70, 255}, cx, HAUTEUR_FENETRE - 22);

    SDL_RenderPresent(r);
}

/* retourne 1=jouer, 0=quitter */
int sdl_menu_principal(ContexteSDL* ctx, config* conf, int total_joueurs) {
    int sel   = 0;
    int cx    = LARGEUR_FENETRE / 2;
    int y0    = 310;
    int dy    = 65;
    int larg  = 240;
    int haut  = 46;

    dessiner_menu(ctx, conf, total_joueurs, sel);

    SDL_Event evt;
    while (SDL_WaitEvent(&evt)) {
        if (evt.type == SDL_QUIT) return 0;

        if (evt.type == SDL_KEYDOWN) {
            switch (evt.key.keysym.sym) {
                case SDLK_UP:
                    sel = (sel + NB_OPTIONS - 1) % NB_OPTIONS;
                    dessiner_menu(ctx, conf, total_joueurs, sel);
                    break;
                case SDLK_DOWN:
                    sel = (sel + 1) % NB_OPTIONS;
                    dessiner_menu(ctx, conf, total_joueurs, sel);
                    break;
                case SDLK_RETURN:
                case SDLK_SPACE:
                    if (sel == 0) return 1;
                    if (sel == 1) { afficher_regles(ctx); dessiner_menu(ctx, conf, total_joueurs, sel); }
                    if (sel == 2) return 0;
                    break;
                case SDLK_ESCAPE:
                    return 0;
            }
        }

        if (evt.type == SDL_MOUSEBUTTONDOWN &&
            evt.button.button == SDL_BUTTON_LEFT) {
            int mx = evt.button.x, my = evt.button.y;
            for (int i = 0; i < NB_OPTIONS; i++) {
                int y = y0 + i * dy;
                SDL_Rect rect = {cx - larg/2, y - haut/2, larg, haut};
                if (mx >= rect.x && mx < rect.x + rect.w &&
                    my >= rect.y && my < rect.y + rect.h) {
                    sel = i;
                    dessiner_menu(ctx, conf, total_joueurs, sel);
                    if (i == 0) return 1;
                    if (i == 1) { afficher_regles(ctx); dessiner_menu(ctx, conf, total_joueurs, sel); }
                    if (i == 2) return 0;
                }
            }
        }

        if (evt.type == SDL_MOUSEMOTION) {
            int mx = evt.motion.x, my = evt.motion.y;
            int ns = sel;
            for (int i = 0; i < NB_OPTIONS; i++) {
                int y = y0 + i * dy;
                SDL_Rect rect = {cx - larg/2, y - haut/2, larg, haut};
                if (mx >= rect.x && mx < rect.x + rect.w &&
                    my >= rect.y && my < rect.y + rect.h) {
                    ns = i; break;
                }
            }
            if (ns != sel) {
                sel = ns;
                dessiner_menu(ctx, conf, total_joueurs, sel);
            }
        }
    }
    return 0;
}
