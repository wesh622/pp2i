#include "tuiles.h"
#include "plateau.h"
#include "pioche.h"
#include <stdio.h>
#include <stdlib.h>
#include "argc.h"

int validation_emplacement_tuile(Plateau* plateau, int x, int y) {
    if (plateau->occupes[x][y] == 1) {
        printf("Cette case est occupée, veuillez en choisir une autre\n");
        return 1;      
    }
    int a = plateau->occupes[x+1][y];   //est ce qu'il y a une tuile à droite
    int b = plateau->occupes[x-1][y];   //est ce qu'il y a une tuile à gauche
    int c = plateau->occupes[x][y+1];  //est ce qu'il y a une tuile au dessus
    int d = plateau->occupes[x][y-1];   //est ce qu'il y a une tuile en dessous
    int adjacence = a+b+c+d;
    else if (adjacence == 0) {
        printf("Cette case n'est reliée à aucune autre tuile, veuillez en choisir une autre\n");
        return 1;
    }
    


    // A FAIRE : ici il faudra vérifier que la tuile s'embrique bien avec celle autour


    else return 0;


}

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


int main(int argc,char** argv[]){
    config* configuration = parse_argument(argc,argv);
    print_config(configuration);
    return 0;
}

