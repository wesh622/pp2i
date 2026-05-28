#include "sdl_plateau.h"

VueSDL vue_init(void) {
    return (VueSDL){TAILLE_MAX/2, TAILLE_MAX/2};
}

void grille_vers_pixel(int i, int j, VueSDL vue, int* px, int* py) {
    *px = (j - vue.centre_j + RAYON_VUE) * TAILLE_CASE;
    *py = (i - vue.centre_i + RAYON_VUE) * TAILLE_CASE;
}

void pixel_vers_grille(int mx, int my, VueSDL vue, int* i, int* j) {
    // clic dans le panel → hors plateau
    if (mx >= TAILLE_CASE * (2*RAYON_VUE+1)) {
        *i = -1; *j = -1; return;
    }
    *i = my / TAILLE_CASE + vue.centre_i - RAYON_VUE;
    *j = mx / TAILLE_CASE + vue.centre_j - RAYON_VUE;
}

void sdl_afficher_plateau(ContexteSDL* ctx, Plateau* p,
                          Tuiles* tuile_courante, VueSDL vue) {
    for (int i = vue.centre_i - RAYON_VUE; i <= vue.centre_i + RAYON_VUE; i++) {
        for (int j = vue.centre_j - RAYON_VUE; j <= vue.centre_j + RAYON_VUE; j++) {
            if (i < 0 || i >= TAILLE_MAX || j < 0 || j >= TAILLE_MAX) continue;
            int px, py;
            grille_vers_pixel(i, j, vue, &px, &py);

            if (p->occupes[i][j]) {
                sdl_dessiner_tuile(ctx->renderer, ctx->police, p->grille[i][j], px, py, 0);
            } else {
                int valide = tuile_courante
                           ? peut_poser_tuile(p, *tuile_courante, i, j)
                           : 0;
                sdl_dessiner_case_vide(ctx->renderer, px, py, valide);
            }
        }
    }
}
