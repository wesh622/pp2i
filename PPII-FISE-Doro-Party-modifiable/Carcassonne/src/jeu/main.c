#include "tuiles.h"
#include "plateau.h"
#include "pioche.h"
#include "score.h"
#include <stdio.h>
#include <stdlib.h>
#include "argc.h"



void boucle_de_jeu(Pioche* pioche, Plateau* plateau, Joueur* j, config* conf, int total_joueurs) {
    Tuiles* tuile = piocher(pioche);
        if(!tuile){
            printf("Error: Unable to draw tile\n");
        }

    printf("\nDrawn tile: ID=%d\n", tuile->id);
    printf("  North=%d, East=%d, South=%d, West=%d, Center=%d\n",
           tuile->a, tuile->b, tuile->c, tuile->d, tuile->center);

    
    if (j->est_IA == 0) {
        int x = 0;
        int y = 0;
        int placement_ok = 0;
  
        while(!placement_ok){
            afficher_plateau_pour_placer_tuile(plateau,pioche);
            int validation_rota = 0;
            int rotation = 0;
            int premiere_question = 2;
            while(validation_rota==0) {
                printf("\nVoulez-vous tourner la tuile ? 1 pour oui, 0 sinon : ");
                scanf("%d", &premiere_question);
                if (premiere_question == 1) {
                    printf("\n 1 pour tourner vers la droite, 2 pour tourner vers la gauche, 3 pour un tour complet : ");
                    scanf("%d", &rotation);
                    if (rotation != 1 && rotation != 2 && rotation != 3) {
                        rotation = 0;
                    }
                    afficher_plateau_pour_placer_tuile(plateau,pioche);
                }
                printf("\n Valider la rotation ? 1 pour oui, 0 pour non ");
                scanf("%d", &validation_rota);
                if (validation_rota != 1) {
                    validation_rota = 0;
                }
            }
            if (rotation == 1) tourner_vers_droite(tuile);
            else if (rotation == 2) tourner_vers_gauche(tuile);
            else if (rotation == 3) {
                tourner_vers_droite(tuile);
                tourner_vers_droite(tuile);
            }
            printf("\nWhere to place the tile?\n");
            printf("X (column): ");
            scanf("%d", &x);
            printf("Y (row): ");
            scanf("%d", &y);
        
            if(peut_poser_tuile(plateau, *tuile, x, y)){
                poser_tuile(plateau, *tuile, x, y);
                placement_ok = 1;
                // PARTIE MORAN SCORING JALON 2 A INTEGRER DANS LE MAIN.C AU DESSUS OU LE LAISSER LA
                verifier_et_scorer_structures(plateau, x, y, conf->tab, total_joueurs);
                // FIN PARTIE MORAN
            }

            else {
                printf("Invalid placement, please try again\n");
            }
        }

        if (au_moins_un_meeple_disponible(j) == 1) {
            // On demande si le joueur veut placer un pion
            char response;
            printf("Voulez-vous placer un meeple sur cette tuile ? (Y/N)\n");
            scanf(" %c", &response); // espace avant %c pour ignorer les retours à la ligne
            if (response == 'Y' || response == 'y') {
                int zone = 0;
                int emplacement = 0;
                printf("A quel endroit de la tuile ? (1: en haut, 2: à droite, 3: en bas, 4: à gauche; 5 ou autre: au milieu))\n");
                scanf(" %d", &emplacement);
                if (emplacement > 5) {
                    emplacement = 5;
                }
                if (emplacement == 1) {
                    zone = tuile->a;
                }
                if (emplacement == 2) {
                    zone = tuile->b;
                }
                if (emplacement == 3) {
                    zone = tuile->c;
                }
                if (emplacement == 4) {
                    zone = tuile->d;
                }
                if (emplacement == 5) {
                    zone = tuile->center;
                }

                if (peut_placer_meeple(plateau, x, y, emplacement)) {
                    Meeple* m = premier_meeple_disponible(j);
                    placer_meeple(m, x, y, zone, emplacement);
                }
            }
        }
    }
    else if (j->est_IA == 1) {
        choix_case_IA(plateau, *tuile, j, conf, total_joueurs);
    }
    
    afficher_plateau(plateau);
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

    afficher_plateau(plateau);
    printf("%d", conf->max_turn);

    // Boucle de jeu :
    for (int i = 0; i < conf->max_turn; i++) {
        int tour = i+1;
        printf("\n=== TOUR N°%d ===\n", tour );
        int joueur_actuel = i % total_joueurs;
        Joueur* j = &(conf->tab[joueur_actuel]);
        boucle_de_jeu(pioche, plateau, j, conf, total_joueurs);
    }

    // score final (structures incompletes en fin de partie)
    score_final(plateau, conf->tab, total_joueurs);

    // Libération mémoire
    free_pioche(pioche);
    free_plateau(plateau);
    free_config(conf);
    return 0;
}




/*

//Omar: Proposer par ia pour l'instant pour tester le jalon 1


int main(int argc, char** argv){
    printf("=== CARCASSONNE - Jalon 1 ===\n\n");
    
    // Parse command-line arguments
    config* conf = parse_argument(argc, argv);
    if(!conf){
        return 0;
    }
    print_config(conf);
    
    // Initialize game board
    Plateau* plateau = init_plateau();
    if(!plateau){
        printf("Error: Failed to create board\n");
        free_config(conf);
        return 1;
    }
    
    // Initialize tile deck
    Pioche* pioche = init_pioche(conf->seed);
    if(!pioche){
        printf("Error: Failed to create deck\n");
        free_plateau(plateau);
        free_config(conf);
        return 1;
    }
    
    // Display initial board state
    afficher_plateau(plateau);
    
    // Main game loop (single turn for Milestone 1 testing)
    printf("\n=== TEST TURN ===\n");
    
    // Draw a tile from the deck
    Tuiles* tuile = piocher(pioche);
    if(!tuile){
        printf("Error: Unable to draw tile\n");
        goto cleanup;
    }
    
    printf("\nDrawn tile: ID=%d\n", tuile->id);
    printf("  North=%d, East=%d, South=%d, West=%d, Center=%d\n",
           tuile->a, tuile->b, tuile->c, tuile->d, tuile->center);

    // Get tile placement and rotation from user
    int x = 0;
    int y = 0;
    int placement_ok = 0;

    // Le nombre total de joueurs est la somme des joueurs physiques et des IA
    int total_joueurs = conf->nbr_joueur + conf->ai;

    
    while(!placement_ok){
        afficher_plateau_pour_placer_tuile(plateau,pioche);
        int validation_rota = 0;
        int rotation = 0;
        int premiere_question = 2;
        while(validation_rota==0) {
            printf("\nVoulez-vous tourner la tuile ? 1 pour oui, 0 sinon : ");
            scanf("%d", &premiere_question);
            if (premiere_question == 1) {
                printf("\n 1 pour tourner vers la droite, 2 pour tourner vers la gauche, 3 pour un tour complet : ");
                scanf("%d", &rotation);
                if (rotation != 1 && rotation != 2 && rotation != 3) {
                    rotation = 0;
                }
                afficher_plateau_pour_placer_tuile(plateau,pioche);
            }
            printf("\n Valider la rotation ? 1 pour oui, 0 pour non ");
            scanf("%d", &validation_rota);
            if (validation_rota != 1) {
                validation_rota = 0;
            }
        }
        if (rotation == 1) tourner_vers_droite(tuile);
        else if (rotation == 2) tourner_vers_gauche(tuile);
        else if (rotation == 3) {
            tourner_vers_droite(tuile);
            tourner_vers_droite(tuile);
        }
        printf("\nWhere to place the tile?\n");
        printf("X (column): ");
        scanf("%d", &x);
        printf("Y (row): ");
        scanf("%d", &y);
        
        if(peut_poser_tuile(plateau, *tuile, x, y)){
            poser_tuile(plateau, *tuile, x, y);
            placement_ok = 1;
            // PARTIE MORAN SCORING JALON 2 A INTEGRER DANS LE MAIN.C AU DESSUS OU LE LAISSER LA
            verifier_et_scorer_structures(plateau, x, y, conf->tab, total_joueurs);
            // FIN PARTIE MORAN
        }

        else {
            printf("Invalid placement, please try again\n");
        }
    }
    
    // Display updated board
    afficher_plateau(plateau);
    
    printf("\nMilestone 1 functional!\n");

    afficher_scores(conf->tab, total_joueurs);
    
cleanup:
    free_pioche(pioche);
    free_plateau(plateau);
    free_config(conf);
    
    return 0;
}


*/
