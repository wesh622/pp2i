#include "tuiles.h"
#include "plateau.h"
#include "pioche.h"
#include "score.h"
#include <stdio.h>
#include <stdlib.h>
#include "argc.h"
#include "affichage.h"

void premier_tour(Pioche* pioche, Plateau* plateau, Joueur* j, config* conf, int total_joueurs) {
    Tuiles* tuile_ptr = piocher(pioche);
    if(!tuile_ptr){
        printf("Error: Unable to draw tile\n");
        return;
    }
    Tuiles tuile = *tuile_ptr;
    printf("\nTuile piochée: \n");
    afficher_tuile_compacte(tuile);
    int validation_rota = 0;
    char rotation;
    char premiere_question;
    while(validation_rota==0) {
                printf("\nVoulez-vous tourner la tuile ? (Y/N) ");
                scanf(" %c", &premiere_question);
                if (premiere_question == 'Y' || premiere_question == 'y')  {
                    printf("\n D pour tourner vers la droite, G pour tourner vers la gauche, T pour faire un tour complet : ");
                    scanf(" %c", &rotation);
                    if (rotation == 'D' || rotation == 'd') tuile = *tourner_vers_droite(&tuile);
                    else if (rotation == 'g' || rotation == 'G') tuile = *tourner_vers_gauche(&tuile);
                    else if (rotation == 't' || rotation == 'T') {
                        tuile = *tourner_vers_droite(&tuile);
                        tuile = *tourner_vers_droite(&tuile);
                    }
                    printf("\nTuile piochée: \n");
                    afficher_tuile_compacte(tuile);
                    afficher_plateau_cli_ameliore(plateau, conf->tab, total_joueurs);
                } else if (premiere_question == 'N' || premiere_question == 'n') {
                    validation_rota = 1;
                }
            }
    poser_tuile(plateau, tuile, 72, 72);
    if (au_moins_un_meeple_disponible(j) == 1) {
            char response;
            printf("Voulez-vous placer un meeple sur cette tuile ? (Y/N)\n");
            scanf(" %c", &response); 
            if (response == 'Y' || response == 'y') {
                int zone = 0;
                int emplacement = 0;
                printf("A quel endroit de la tuile ? (1: en haut, 2: à droite, 3: en bas, 4: à gauche; 5 ou autre: au milieu))\n");
                scanf(" %d", &emplacement);
                if (emplacement > 5) {
                    emplacement = 5;
                }
                if (emplacement == 1) {
                    zone = tuile.a;
                }
                if (emplacement == 2) {
                    zone = tuile.b;
                }
                if (emplacement == 3) {
                    zone = tuile.c;
                }
                if (emplacement == 4) {
                    zone = tuile.d;
                }
                if (emplacement == 5) {
                    zone = tuile.center;
                }

                if (peut_placer_meeple(plateau, 72, 72, emplacement)) {
                    Meeple* m = premier_meeple_disponible(j);
                    placer_meeple(m, 72, 72, zone, emplacement);
                }
            }
        }
    verifier_et_scorer_structures(plateau, 72, 72, conf->tab, total_joueurs);
}

void boucle_de_jeu(Pioche* pioche, Plateau* plateau, Joueur* j, config* conf, int total_joueurs) {

    Tuiles* tuile_ptr = piocher_tuile_jouable(pioche, plateau);
    if(!tuile_ptr){
        printf("Error: Unable to draw tile\n");
        return;
    }
    Tuiles tuile = *tuile_ptr;

    if (j->est_IA == 0) {
        int x = 0;
        int y = 0;
        int placement_ok = 0;
        while(!placement_ok){
            printf("\nTuile piochée: \n");
            afficher_tuile_compacte(tuile);
            afficher_plateau_cli_ameliore_pour_placer_tuile(plateau, &tuile, conf->tab, total_joueurs);
            int validation_rota = 0;
            char rotation;
            char premiere_question;
            while(validation_rota==0) {
                printf("\nVoulez-vous tourner la tuile ? (Y/N) ");
                scanf(" %c", &premiere_question);
                if (premiere_question == 'Y' || premiere_question == 'y')  {
                    printf("\n D pour tourner vers la droite, G pour tourner vers la gauche, T pour faire un tour complet : ");
                    scanf(" %c", &rotation);
                    if (rotation == 'D' || rotation == 'd') tuile = *tourner_vers_droite(&tuile);
                    else if (rotation == 'g' || rotation == 'G') tuile = *tourner_vers_gauche(&tuile);
                    else if (rotation == 't' || rotation == 'T') {
                        tuile = *tourner_vers_droite(&tuile);
                        tuile = *tourner_vers_droite(&tuile);
                    }
                    printf("\nTuile piochée: \n");
                    afficher_tuile_compacte(tuile);
                    afficher_plateau_cli_ameliore_pour_placer_tuile(plateau, &tuile, conf->tab, total_joueurs);
                } else if (premiere_question == 'N' || premiere_question == 'n') {
                    validation_rota = 1;
                }
            }
            printf("\nOù voulez vous placer la tuile? \n");
            printf("X (ligne): ");
            scanf("%d", &x);
            printf("Y (colonne): ");
            scanf("%d", &y);

            if(peut_poser_tuile(plateau, tuile, x, y)){
                poser_tuile(plateau, tuile, x, y);
                placement_ok = 1;
            }
        }

        if (au_moins_un_meeple_disponible(j) == 1) {
            char response;
            printf("Voulez-vous placer un meeple sur cette tuile ? (Y/N)\n");
            scanf(" %c", &response); 
            if (response == 'Y' || response == 'y') {
                int zone = 0;
                int emplacement = 0;
                printf("A quel endroit de la tuile ? (1: en haut, 2: à droite, 3: en bas, 4: à gauche; 5 ou autre: au milieu))\n");
                scanf(" %d", &emplacement);
                if (emplacement > 5) {
                    emplacement = 5;
                }
                if (emplacement == 1) {
                    zone = tuile.a;
                }
                if (emplacement == 2) {
                    zone = tuile.b;
                }
                if (emplacement == 3) {
                    zone = tuile.c;
                }
                if (emplacement == 4) {
                    zone = tuile.d;
                }
                if (emplacement == 5) {
                    zone = tuile.center;
                }

                if (peut_placer_meeple(plateau, x, y, emplacement)) {
                    Meeple* m = premier_meeple_disponible(j);
                    placer_meeple(m, x, y, zone, emplacement);
                }
            }
        }
        
        verifier_et_scorer_structures(plateau, x, y, conf->tab, total_joueurs);
    }
    else if (j->est_IA == 1) {
        choix_case_IA(plateau, tuile, j, conf, total_joueurs);
    }
    afficher_scores(conf->tab, total_joueurs);
}


int main(int argc, char** argv){
    config* conf = parse_argument(argc, argv);
    if(!conf) return 0;
    print_config(conf);

    Plateau* plateau = init_plateau();
    if(!plateau) { free_config(conf); return 1; }

    Pioche* pioche = init_pioche(conf->seed);
    if(!pioche) { free_plateau(plateau); free_config(conf); return 1; }

    int total_joueurs = conf->nbr_joueur + conf->ai;

    afficher_plateau_cli_ameliore(plateau, conf->tab, total_joueurs);

    premier_tour(pioche, plateau, &conf->tab[0], conf, total_joueurs);

    for (int i = 1; i < conf->max_turn ; i++) {
        int tour = i+1;
        printf("\n=== TOUR N°%d ===\n", tour );
        int joueur_actuel = i % total_joueurs;
        Joueur* j = &(conf->tab[joueur_actuel]);
        boucle_de_jeu(pioche, plateau, j, conf, total_joueurs);
    }

    calculer_score_incomplet_fin_partie(plateau, conf->tab, total_joueurs);

    calculer_score_prairies_fin_partie(plateau, conf->tab,total_joueurs);
    afficher_plateau_cli_ameliore(plateau, conf->tab, total_joueurs);
    afficher_fin_de_partie(conf->tab, total_joueurs);

    free_pioche(pioche);
    free_plateau(plateau);
    free_config(conf);
    return 0;
}
