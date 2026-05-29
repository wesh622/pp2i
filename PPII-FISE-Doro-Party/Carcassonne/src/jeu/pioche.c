#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pioche.h"
#include "tuiles.h"

// Debut: Moran

// Initialisation de la pioche
Pioche* init_pioche(int seed){
    Pioche* p = malloc(sizeof(Pioche));
    assert(p != NULL);
    
    //copier les 71 tuiles (on saute la tuile 0 = tuile de depart)
    for(int i=1;i<72;i++){
        p->pile[i-1] = tuiles_jeu[i];
    }
    
    p->nbresrestantes = 71;
    p->indicecourant = 0;
    
    //melanger avec la seed fournie
    srand(seed);
    for(int i=70;i>0;i--){
        int j = rand() % (i+1);
        //swap pile[i] et pile[j]
        Tuiles temp = p->pile[i];
        p->pile[i] = p->pile[j];
        p->pile[j] = temp;
    }
    
    printf("Pioche initialisee avec seed %d\n",seed);
    return p;
}

// Piocher une tuile de la pioche
Tuiles* piocher(Pioche* p){
    if(p->nbresrestantes == 0){
        printf("Pioche vide\n");
        return NULL;
    }
    
    Tuiles* tuile = &p->pile[p->indicecourant];
    p->indicecourant++;
    p->nbresrestantes--;
    
    printf("Tuile %d piochee (%d restantes)\n",tuile->id,p->nbresrestantes);
    
    return tuile;
}

// Vérifie si la pioche est vide
int pioche_vide(Pioche* p){
    if(p->nbresrestantes == 0){
        return 1;
    }
    return 0;
}

// Libération de la mémoire de la pioche
void free_pioche(Pioche* p){
    if(p){
        free(p);
    }
}

