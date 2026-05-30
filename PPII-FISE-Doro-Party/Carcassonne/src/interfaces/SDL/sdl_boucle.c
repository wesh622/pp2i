#include "sdl_boucle.h"
#include "sdl_init.h"
#include "sdl_accueil.h"
#include "sdl_plateau.h"
#include "sdl_panel.h"
#include "sdl_input.h"
#include "sdl_meeple_overlay.h"
#include "sdl_fin.h"
#include "score.h"
#include "meeple.h"
#include "plateau.h"
#include <SDL2/SDL.h>

static void tout_afficher(ContexteSDL* ctx, Plateau* plateau, Pioche* pioche,config* conf, Tuiles* tuile, int tour,int total_joueurs, Joueur* j, VueSDL* vue) {
    SDL_SetRenderDrawColor(ctx->renderer, 12, 12, 18, 255);
    SDL_RenderClear(ctx->renderer);
    sdl_afficher_plateau(ctx, plateau, tuile, *vue, conf->tab, total_joueurs);
    sdl_afficher_panel(ctx, j, pioche, tuile, tour, total_joueurs, conf->tab);
    SDL_RenderPresent(ctx->renderer);
}

static int tuile_placable_quelque_part(Plateau* p, Tuiles* t) {
    for (int rot = 0; rot < 4; rot++) {
        for (int i = 0; i < TAILLE_MAX; i++) {
            for (int j = 0; j < TAILLE_MAX; j++) {
                if (!p->occupes[i][j] && peut_poser_tuile(p, *t, i, j))
                    return 1;
            }
        }
        tourner_vers_droite(t);
    }
    return 0;
}

// Construit interdit[5] : interdit[e-1]==1 si un meeple est deja poser en (x,y,e). 
static void calculer_zones_interdites(config* conf, int total_joueurs,int x, int y, int interdit[5]) {
    for (int i = 0; i < 5; i++) interdit[i] = 0;
    for (int p = 0; p < total_joueurs; p++) {
        for (int k = 0; k < 7; k++) {
            Meeple* m = &conf->tab[p].stock[k];
            if (!meeple_disponible(m) && m->posX == x && m->posY == y) {
                int e = m->emplacement;
                if (e >= 1 && e <= 5) interdit[e-1] = 1;
            }
        }
    }
}

void boucle_sdl_principale(Plateau* plateau, Pioche* pioche,
                            config* conf, int total_joueurs) {
    ContexteSDL* ctx = init_sdl();
    if (!ctx) return;

    // menu principale 
    if (!sdl_menu_principal(ctx, conf, total_joueurs)) {
        close_sdl(ctx);
        return;
    }

    VueSDL vue = vue_init();
    int quitter = 0;

    for (int i = 0; i < conf->max_turn && !pioche_vide(pioche) && !quitter; i++) {
        int joueur_actuel = i % total_joueurs;
        Joueur* j = &conf->tab[joueur_actuel];

        // piochée une tuile placable 
        Tuiles* tuile = NULL;
        while (!pioche_vide(pioche)) {
            tuile = piocher(pioche);
            if (!tuile) break;
            if (tuile_placable_quelque_part(plateau, tuile)) break;
            tuile = NULL;
        }
        if (!tuile) break;

        SDL_Color col_j = couleur_joueur(j->idjoueur - 1);

        if (j->est_IA) {
            choix_case_IA(plateau, *tuile, j, conf, total_joueurs);
            tout_afficher(ctx, plateau, pioche, conf, NULL, i+1, total_joueurs, j, &vue);
            SDL_Delay(700);
            continue;
        }

        // joueur humain 
        int placement_ok = 0;
        while (!placement_ok && !quitter) {
            tout_afficher(ctx, plateau, pioche, conf, tuile, i+1, total_joueurs, j, &vue);

            int i_case, j_case, rot;
            int action = sdl_attendre_action(&vue, &i_case, &j_case, &rot);

            if (action == ACTION_QUITTER) { quitter = 1; break; }

            if (action == ACTION_ROTATION) {
                if (rot == 1) tourner_vers_droite(tuile);
                else          tourner_vers_gauche(tuile);
            }

            if (action == ACTION_POSER && i_case >= 0) {
                if (peut_poser_tuile(plateau, *tuile, i_case, j_case)) {
                    poser_tuile(plateau, *tuile, i_case, j_case);
                    verifier_et_scorer_structures(plateau, i_case, j_case,
                                                  conf->tab, total_joueurs);
                    placement_ok = 1;

                    if (au_moins_un_meeple_disponible(j)) {
                        tout_afficher(ctx, plateau, pioche, conf, NULL,
                                      i+1, total_joueurs, j, &vue);

                        int interdit[5];
                        calculer_zones_interdites(conf, total_joueurs,
                                                  i_case, j_case, interdit);

                        int emplacement = sdl_choisir_emplacement_meeple(
                                            ctx, *tuile, col_j, interdit);

                        if (emplacement > 0 &&
                            !interdit[emplacement-1] &&
                            peut_placer_meeple(plateau, i_case, j_case, emplacement)) {
                            Meeple* m = premier_meeple_disponible(j);
                            int zone = 0;
                            if      (emplacement == 1) zone = tuile->a;
                            else if (emplacement == 2) zone = tuile->b;
                            else if (emplacement == 3) zone = tuile->c;
                            else if (emplacement == 4) zone = tuile->d;
                            else                       zone = tuile->center;
                            placer_meeple(m, i_case, j_case, zone, emplacement);
                        }
                    }
                }
            }
        }
    }

    if (!quitter) {
        calculer_score_prairies_fin_partie(plateau, conf->tab, total_joueurs);
        tout_afficher(ctx, plateau, pioche, conf, NULL, conf->max_turn,
                      total_joueurs, &conf->tab[0], &vue);
        sdl_afficher_fin_de_partie(ctx, conf->tab, total_joueurs);
    }

    close_sdl(ctx);
}
