#include "tuiles.h"
#include "plateau.h"
#include "pioche.h"
#include <stdio.h>
#include <stdlib.h>
#include "argc.h"

//Omar: validation_emplacement est dans plateau.c je savais pas qqun avait deja fait du coup jen avais coder une peut_poser_tuiles. Reste plus qua choisir celle qui convient

/*
void boucle_de_jeu(Pioche* pioche, Plateau* plateau) {
    Tuiles tuile = pioche->pile[0];  //on prend la première tuile de la picohe
    for (int i = 1; i < pioche->nbresrestantes; i++) {  //on décale toute les tuiles vers la gauche dans la pioche pour enlever la première
        pioche->pile[i - 1] = pioche->pile[i];
    }
    pioche->nbresrestantes--;   //on actualise l'indice et le nombre de tuile restantes de la pioche
    pioche->indicecourant--;
    

    //solution temporaire : on demande dans le terminal une coordonée x et y pour placer la tuile
    int x, y;
    do {
        printf("Veuillez choisir une coordonnée x pour votre tuile : ");
        scanf("%d", &x);
        printf("Veuillez choisir une coordonnée y pour votre tuile : ");
        scanf("%d", &y);
    } while (validation_emplacement_tuile(plateau, x, y) != 0);

    // On place la tuile sur le plateau
    plateau->grille[x][y] = tuile;
    plateau->occupes[x][y] = 1;




    // On demande si le joueur veut placer un pion
    char response;
    printf("Voulez-vous placer un meeple sur cette tuile ? (Y/N)\n");
    scanf(" %c", &response); // espace avant %c pour ignorer les retours à la ligne
    if (response == 'Y' || response == 'y') {

        // A FAIRE : placer le meeple à l'endroit souhaité sur la tuile

    }




    // On modifie le score selon les actions du tour

    // A FAIRE : calculer et mettre à jour le score en fonction de la tuile posée et du meeple

}


int main(int argc,char** argv){
    config* configuration = parse_argument(argc,argv);
    print_config(configuration);
    return 0;
}


*/


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
    
    // Get tile placement from user
    int x = 0;
    int y = 0;
    int placement_ok = 0;
    
    while(!placement_ok){
        printf("\nWhere to place the tile?\n");
        printf("X (column): ");
        scanf("%d", &x);
        printf("Y (row): ");
        scanf("%d", &y);
        
        if(peut_poser_tuile(plateau, x, y)){
            poser_tuile(plateau, *tuile, x, y);
            placement_ok = 1;
        }
        else{
            printf("Invalid placement, please try again\n");
        }
    }
    
    // Display updated board
    afficher_plateau(plateau);
    
    printf("\nMilestone 1 functional!\n");
    
cleanup:
    free_pioche(pioche);
    free_plateau(plateau);
    free_config(conf);
    
    return 0;
}
