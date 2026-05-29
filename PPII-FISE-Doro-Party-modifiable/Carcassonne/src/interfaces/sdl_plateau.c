#include "sdl_plateau.h"

VueSDL vue_init(void) {
    return (VueSDL){TAILLE_MAX/2, TAILLE_MAX/2};
}

void grille_vers_pixel(int i, int j, VueSDL vue, int* px, int* py) {
    *px = (j - vue.centre_j + RAYON_VUE) * TAILLE_CASE;
    *py = (i - vue.centre_i + RAYON_VUE) * TAILLE_CASE;
}

void pixel_vers_grille(int mx, int my, VueSDL vue, int* i, int* j) {
    if (mx >= TAILLE_CASE * (2*RAYON_VUE+1)) {
        *i = -1; *j = -1; return;
    }
    *i = my / TAILLE_CASE + vue.centre_i - RAYON_VUE;
    *j = mx / TAILLE_CASE + vue.centre_j - RAYON_VUE;
}

static void dessiner_meeple_sur_tuile(SDL_Renderer* r, int px, int py,
                                      int emplacement, SDL_Color col) {
    int tc    = TAILLE_CASE;
    int tiers = tc / 3;
    int mx, my;
    switch (emplacement) {
        case 1: mx = px + tc/2;        my = py + tiers/2;      break;  // Nord
        case 2: mx = px + tc - tiers/2; my = py + tc/2;        break;  // Est
        case 3: mx = px + tc/2;        my = py + tc - tiers/2; break;  // Sud
        case 4: mx = px + tiers/2;     my = py + tc/2;         break;  // Ouest
        case 5: mx = px + tc/2;        my = py + tc/2;         break;  // Centre
        default: return;
    }
    int s = 8;
    SDL_SetRenderDrawColor(r, col.r, col.g, col.b, 255);
    SDL_Rect rect = {mx - s/2, my - s/2, s, s};
    SDL_RenderFillRect(r, &rect);
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    SDL_RenderDrawRect(r, &rect);
}

void sdl_afficher_plateau(ContexteSDL* ctx, Plateau* p,
                          Tuiles* tuile_courante, VueSDL vue,
                          Joueur* joueurs, int total_joueurs) {
    for (int i = vue.centre_i - RAYON_VUE; i <= vue.centre_i + RAYON_VUE; i++) {
        for (int j = vue.centre_j - RAYON_VUE; j <= vue.centre_j + RAYON_VUE; j++) {
            if (i < 0 || i >= TAILLE_MAX || j < 0 || j >= TAILLE_MAX) continue;
            int px, py;
            grille_vers_pixel(i, j, vue, &px, &py);

            if (p->occupes[i][j]) {
                sdl_dessiner_tuile(ctx->renderer, ctx->police, p->grille[i][j], px, py, 0);

                // dessiner les meeples posés sur cette case
                for (int k = 0; k < total_joueurs; k++) {
                    for (int m = 0; m < 7; m++) {
                        Meeple* meeple = &joueurs[k].stock[m];
                        if (meeple->etat == 0 && meeple->posX == i && meeple->posY == j) {
                            SDL_Color col = couleur_joueur(joueurs[k].idjoueur - 1);
                            dessiner_meeple_sur_tuile(ctx->renderer, px, py, meeple->emplacement, col);
                        }
                    }
                }
            } else {
                int valide = tuile_courante
                           ? peut_poser_tuile(p, *tuile_courante, i, j)
                           : 0;
                sdl_dessiner_case_vide(ctx->renderer, px, py, valide);
            }
        }
    }
}
