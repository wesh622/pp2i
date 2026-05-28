#include "sdl_boucle.h"
#include "sdl_init.h"
#include "sdl_plateau.h"
#include "sdl_panel.h"
#include "sdl_input.h"
#include "sdl_meeple_overlay.h"
#include "sdl_fin.h"
#include "score.h"
#include "meeple.h"
#include "plateau.h"
#include <SDL2/SDL.h>

static void tout_afficher(ContexteSDL* ctx, Plateau* plateau, Pioche* pioche,
                           config* conf, Tuiles* tuile, int tour,
                           int total_joueurs, Joueur* j, VueSDL* vue) {
    SDL_SetRenderDrawColor(ctx->renderer, 12, 12, 18, 255);
    SDL_RenderClear(ctx->renderer);
    sdl_afficher_plateau(ctx, plateau, tuile, *vue);
    sdl_afficher_panel(ctx, j, pioche, tuile, tour, total_joueurs, conf->tab);
    SDL_RenderPresent(ctx->renderer);
}

void boucle_sdl_principale(Plateau* plateau, Pioche* pioche,
                            config* conf, int total_joueurs) {
    ContexteSDL* ctx = init_sdl();
    if (!ctx) return;

    VueSDL vue = vue_init();
    int quitter = 0;

    for (int i = 0; i < conf->max_turn && !pioche_vide(pioche) && !quitter; i++) {
        int joueur_actuel = i % total_joueurs;
        Joueur* j = &conf->tab[joueur_actuel];

        Tuiles* tuile = piocher(pioche);
        if (!tuile) break;

        if (j->est_IA) {
            // l'IA joue, on reaffiche et on laisse une petite pause pour que ce soit visible
            choix_case_IA(plateau, *tuile, j, conf, total_joueurs);
            tout_afficher(ctx, plateau, pioche, conf, NULL, i+1, total_joueurs, j, &vue);
            SDL_Delay(700);
            continue;
        }

        // joueur humain : attendre un clic valide
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
                    verifier_et_scorer_structures(plateau, i_case, j_case, conf->tab, total_joueurs);
                    placement_ok = 1;

                    if (au_moins_un_meeple_disponible(j)) {
                        tout_afficher(ctx, plateau, pioche, conf, NULL, i+1, total_joueurs, j, &vue);
                        int px, py;
                        grille_vers_pixel(i_case, j_case, vue, &px, &py);
                        int emplacement = sdl_choisir_emplacement_meeple(ctx, *tuile, px, py);
                        if (emplacement > 0 && peut_placer_meeple(plateau, i_case, j_case, emplacement)) {
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
                // clic invalide → on reboucle silencieusement
            }
        }
    }

    if (!quitter) {
        score_final(plateau, conf->tab, total_joueurs);
        tout_afficher(ctx, plateau, pioche, conf, NULL, conf->max_turn, total_joueurs, &conf->tab[0], &vue);
        sdl_afficher_fin_de_partie(ctx, conf->tab, total_joueurs);
    }

    close_sdl(ctx);
}
