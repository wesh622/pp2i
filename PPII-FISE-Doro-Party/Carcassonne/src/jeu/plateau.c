#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "plateau.h"
#include "tuiles.h"

Plateau* init_plateau(){
    Plateau* p = malloc(sizeof(Plateau));
    assert(p != NULL);
    
    //initialisation de toutes les cases a vide
    for(int i=0;i<TAILLE_MAX;i++){
        for(int j=0;j<TAILLE_MAX;j++){
            p->occupes[i][j] = 0;
        }
    }
    
    //placement de la tuile de depart au centre du plateau
    int centre = TAILLE_MAX/2;
    p->grille[centre][centre] = tuiles_jeu[0];
    p->occupes[centre][centre] = 1;
    
    printf("Plateau initialise. Tuile de depart en (%d,%d)\n",centre,centre);
    
    return p;
}
/*
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

*/

int peut_poser_tuile(Plateau* p, int x, int y){
    //verif1 : la case est elle occupee ?
    if(p->occupes[x][y] == 1){
        printf("Cette case est occupee, veuillez en choisir une autre\n");
        return 0;
    }
    
    //verification 2 : coordonne hors limites ?
    if(x < 0 || x >= TAILLE_MAX || y < 0 || y >= TAILLE_MAX){
        printf("Coordonnees hors du plateau\n");
        return 0;
    }
    
    //verification 3 : au moins une tuile adjacente ?
    int a = 0; //tuile a droite
    int b = 0; //tuile a gauche  
    int c = 0; //tuile en haut
    int d = 0; //tuile en bas
    
    if(x > 0 && p->occupes[x-1][y]){
        a = 1;
    }
    if(x < TAILLE_MAX-1 && p->occupes[x+1][y]){
        b = 1;
    }
    if(y > 0 && p->occupes[x][y-1]){
        c = 1;
    }
    if(y < TAILLE_MAX-1 && p->occupes[x][y+1]){
        d = 1;
    }
    
    int adjacence = a+b+c+d;
    
    if(adjacence == 0){
        printf("Cette case n'est reliee a aucune autre tuile, veuillez en choisir une autre\n");
        return 0;
    }
    
    //pour le jalon 1 on accepte sans verifier la compatibilite des faces
    //TODO jalon 2 : ajouter faces_compatibles()
    
    return 1;
}

void poser_tuile(Plateau* p, Tuiles tuile, int x, int y){
    p->grille[x][y] = tuile;
    p->occupes[x][y] = 1;
    printf("Tuile %d posee en (%d,%d)\n",tuile.id,x,y);
}

void afficher_plateau(Plateau* p){
    int centre = TAILLE_MAX/2;
    int rayon = 5; //affiche de centre-5 a centre+5
    
    printf("\n=== PLATEAU ===\n");
    printf("   ");
    for(int j=centre-rayon;j<=centre+rayon;j++){
        printf("%3d ",j);
    }
    printf("\n");
    
    for(int i=centre-rayon;i<=centre+rayon;i++){
        printf("%3d ",i);
        for(int j=centre-rayon;j<=centre+rayon;j++){
            if(p->occupes[i][j]){
                printf("[%02d] ",p->grille[i][j].id);
            }
            else{
                printf(" .. ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void free_plateau(Plateau* p){
    if(p){
        free(p);
    }
}