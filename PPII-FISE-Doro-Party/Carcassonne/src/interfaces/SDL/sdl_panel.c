#include "sdl_panel.h"
#include "sdl_rendu.h"
#include <stdio.h>

#define PX_PANEL (TAILLE_CASE * (2*RAYON_VUE+1))

// partie Antoine

// affiche la barre de progression de la pioche
static void dessiner_barre_pioche(SDL_Renderer* r, TTF_Font* police,
                                   Pioche* pioche, int y) {
    int restantes     = pioche->nbresrestantes;
    int largeur_barre = LARGEUR_PANEL - 20;
    int rempli        = (restantes * largeur_barre) / 71;

    SDL_SetRenderDrawColor(r, 50, 50, 50, 255);
    SDL_Rect fond = {PX_PANEL + 10, y, largeur_barre, 14};
    SDL_RenderFillRect(r, &fond);

    SDL_SetRenderDrawColor(r, 80, 200, 80, 255);
    SDL_Rect bar = {PX_PANEL + 10, y, rempli, 14};
    SDL_RenderFillRect(r, &bar);

    char buf[20];
    snprintf(buf, sizeof(buf), "Pioche: %d/71", restantes);
    sdl_texte(r, police, buf, (SDL_Color){255,255,255,255},
              PX_PANEL + LARGEUR_PANEL/2, y + 7);
}

// affiche le panneau latéral avec les infos du joueur
void sdl_afficher_panel(ContexteSDL* ctx, Joueur* j, Pioche* pioche,
                        Tuiles* tuile_courante, int tour,
                        int total_joueurs, Joueur* tous_joueurs) {
    SDL_Renderer* r = ctx->renderer;

    SDL_SetRenderDrawColor(r, 20, 20, 30, 255);
    SDL_Rect fond = {PX_PANEL, 0, LARGEUR_PANEL, HAUTEUR_FENETRE};
    SDL_RenderFillRect(r, &fond);

    int y = 15;
    char buf[64];

    snprintf(buf, sizeof(buf), "=== TOUR %d ===", tour);
    sdl_texte(r, ctx->police_grande, buf,
              (SDL_Color){255,220,60,255}, PX_PANEL + LARGEUR_PANEL/2, y);
    y += 28;

    snprintf(buf, sizeof(buf), "%s%s", j->nom, j->est_IA ? " (IA)" : "");
    sdl_texte(r, ctx->police_grande, buf,
              couleur_joueur(j->idjoueur - 1), PX_PANEL + LARGEUR_PANEL/2, y);
    y += 30;

    sdl_texte(r, ctx->police, "--- Scores ---",
              (SDL_Color){160,160,160,255}, PX_PANEL + LARGEUR_PANEL/2, y);
    y += 18;
    for (int k = 0; k < total_joueurs; k++) {
        if (!tous_joueurs[k].actif) continue;
        SDL_Color col = (&tous_joueurs[k] == j)
            ? couleur_joueur(tous_joueurs[k].idjoueur - 1)
            : (SDL_Color){160,160,160,255};
        snprintf(buf, sizeof(buf), "%-12s %4d pts", tous_joueurs[k].nom, tous_joueurs[k].score);
        sdl_texte(r, ctx->police, buf, col, PX_PANEL + LARGEUR_PANEL/2, y);
        y += 16;
    }
    y += 8;

    int dispo = 0;
    for (int k = 0; k < 7; k++)
        if (j->stock[k].etat == 1) dispo++;
    snprintf(buf, sizeof(buf), "Meeples: %d/7", dispo);
    sdl_texte(r, ctx->police, buf,
              (SDL_Color){200,150,240,255}, PX_PANEL + LARGEUR_PANEL/2, y);
    y += 20;

    dessiner_barre_pioche(r, ctx->police, pioche, y);
    y += 24;

    if (tuile_courante) {
        sdl_texte(r, ctx->police, "Tuile piochee:",
                  (SDL_Color){160,160,160,255}, PX_PANEL + LARGEUR_PANEL/2, y);
        y += 16;
        int tx = PX_PANEL + LARGEUR_PANEL/2 - TAILLE_CASE/2;
        sdl_dessiner_tuile(ctx, *tuile_courante, tx, y, 0);
        y += TAILLE_CASE + 8;
    }

    // couleurs des joueurs
    y += 4;
    sdl_texte(r, ctx->police, "--- Joueurs ---",
              (SDL_Color){160,160,160,255}, PX_PANEL + LARGEUR_PANEL/2, y);
    y += 18;
    for (int k = 0; k < total_joueurs; k++) {
        if (!tous_joueurs[k].actif) continue;
        SDL_Color col = couleur_joueur(tous_joueurs[k].idjoueur - 1);
        SDL_SetRenderDrawColor(r, col.r, col.g, col.b, 255);
        SDL_Rect carre_j = {PX_PANEL + 10, y + 2, 12, 12};
        SDL_RenderFillRect(r, &carre_j);
        char nom_buf[32];
        snprintf(nom_buf, sizeof(nom_buf), "%s%s", tous_joueurs[k].nom,
                 tous_joueurs[k].est_IA ? " (IA)" : "");
        sdl_texte_g(r, ctx->police, nom_buf, col, PX_PANEL + 28, y);
        y += 16;
    }

    // legende des zones
    y += 8;
    sdl_texte(r, ctx->police, "--- Zones ---",
              (SDL_Color){160,160,160,255}, PX_PANEL + LARGEUR_PANEL/2, y);
    y += 18;
    static const int zones[] = {
        ROUTE_PRAIRIE, PRAIRIE, VILLE, VILLE_BOUCLIER, ABBAYE, CARREFOUR
    };
    for (int k = 0; k < 6; k++) {
        SDL_Color col = couleur_sdl_face(zones[k]);
        SDL_SetRenderDrawColor(r, col.r, col.g, col.b, 255);
        SDL_Rect carre_z = {PX_PANEL + 10, y + 2, 12, 12};
        SDL_RenderFillRect(r, &carre_z);
        SDL_SetRenderDrawColor(r, 180, 180, 180, 255);
        SDL_RenderDrawRect(r, &carre_z);
        sdl_texte_g(r, ctx->police, symbole_face(zones[k]),
                    (SDL_Color){200,200,200,255}, PX_PANEL + 28, y);
        y += 16;
    }

    y = HAUTEUR_FENETRE - 90;
    sdl_texte(r, ctx->police, "Clic G : poser tuile",    (SDL_Color){120,120,120,255}, PX_PANEL + LARGEUR_PANEL/2, y); y += 15;
    sdl_texte(r, ctx->police, "R : tourner droite",      (SDL_Color){120,120,120,255}, PX_PANEL + LARGEUR_PANEL/2, y); y += 15;
    sdl_texte(r, ctx->police, "L : tourner gauche",      (SDL_Color){120,120,120,255}, PX_PANEL + LARGEUR_PANEL/2, y); y += 15;
    sdl_texte(r, ctx->police, "Fleches : deplacer vue",  (SDL_Color){120,120,120,255}, PX_PANEL + LARGEUR_PANEL/2, y); y += 15;
    sdl_texte(r, ctx->police, "Echap : quitter",         (SDL_Color){120,120,120,255}, PX_PANEL + LARGEUR_PANEL/2, y);
}
