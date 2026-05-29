#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "plateau.h"
#include "tuiles.h"
#include "joueur.h"



void test_init_plateau(void) {
    printf("Test init_plateau...\n");

    Plateau *p = init_plateau();
    assert(p != NULL);

    // init_plateau initialise le plateau vide, la tuile de depart est posee par main
    assert(p->occupes[0][0] == 0);
    assert(p->occupes[TAILLE_MAX-1][TAILLE_MAX-1] == 0);

    free_plateau(p);
    printf("  -> OK\n");
}

void test_peut_poser_tuile(void) {
    printf("Test peut_poser_tuile...\n");

    Plateau *p = init_plateau();
    int centre = TAILLE_MAX / 2;

    // tuiles_jeu[0] = {id=1, a=2(PRAIRIE), b=2(PRAIRIE), c=1(ROUTE), d=1(ROUTE)}
    // tuiles_jeu[2] = {id=3, a=1(ROUTE), b=2(PRAIRIE), c=1(ROUTE), d=2(PRAIRIE)}
    // dans cette version x+1=Sud, y+1=Est
    poser_tuile(p, tuiles_jeu[0], centre, centre);

    // case deja occupee
    assert(peut_poser_tuile(p, tuiles_jeu[1], centre, centre) == 0);

    // hors limites
    assert(peut_poser_tuile(p, tuiles_jeu[1], -1, 0) == 0);
    assert(peut_poser_tuile(p, tuiles_jeu[1], TAILLE_MAX, 0) == 0);

    // case eloignee sans voisin
    assert(peut_poser_tuile(p, tuiles_jeu[1], centre+2, centre) == 0);

    // tuiles_jeu[2].a=ROUTE correspond a tuiles_jeu[0].c=ROUTE → compatible au sud (x+1)
    assert(peut_poser_tuile(p, tuiles_jeu[2], centre+1, centre) == 1);
    // tuiles_jeu[2].c=ROUTE correspond a tuiles_jeu[0].a=PRAIRIE → incompatible au nord (x-1)
    // mais tuiles_jeu[2].b=PRAIRIE correspond a tuiles_jeu[0].d=ROUTE → verif est (y+1)
    assert(peut_poser_tuile(p, tuiles_jeu[2], centre, centre+1) == 1);

    // incompatibilite forcee
    Tuiles incompatible = tuiles_jeu[2];
    incompatible.a = 99;
    assert(peut_poser_tuile(p, incompatible, centre+1, centre) == 0);

    // on pose tuiles_jeu[2] au sud et on libere centre pour tester la compatibilite retour
    poser_tuile(p, tuiles_jeu[2], centre+1, centre);
    p->occupes[centre][centre] = 0;
    // tuiles_jeu[0].c=ROUTE doit matcher tuiles_jeu[2].a=ROUTE → compatible
    assert(peut_poser_tuile(p, tuiles_jeu[0], centre, centre) == 1);

    free_plateau(p);
    printf("  -> OK\n");
}

void test_poser_tuile(void) {
    printf("Test poser_tuile...\n");
    
    Plateau *p = init_plateau();
    int centre = TAILLE_MAX / 2;
    int x = centre + 1;
    int y = centre;
    
    // On utilise la deuxième tuile du tableau global (indice 1)
    Tuiles t = tuiles_jeu[1];
    assert(p->occupes[x][y] == 0);
    
    poser_tuile(p, t, x, y);
    assert(p->occupes[x][y] == 1);
    assert(p->grille[x][y].id == t.id);
    
    free_plateau(p);
    printf("  -> OK\n");
}

void test_afficher_plateau(void) {
    printf("Test afficher_plateau...\n");
    // Rien à vérifier automatiquement, on regarde juste que ça ne crash pas
    Plateau *p = init_plateau();
    afficher_plateau(p);
    free_plateau(p);
    printf("  -> OK (vérification visuelle)\n");
}

void test_afficher_tour(void) {
    printf("Test afficher_tour...\n");
    Joueur j;
    strcpy(j.nom, "Alice");
    j.idjoueur = 1;
    j.actif = 1;
    j.score = 42;
    for (int i = 0; i < 7; i++) {
        j.stock[i].id = i + 1;
        j.stock[i].etat = 1;
        j.stock[i].idjoueur = 1;
    }
    afficher_tour(3, &j);
    printf("  -> OK (vérification visuelle)\n");
}

void test_afficher_tuile(void) {
    printf("Test afficher_tuile...\n");
    Tuiles t = tuiles_jeu[0];
    afficher_tuile(t);
    printf("  -> OK (vérification visuelle)\n");
}

void test_verifier_abbaye_complete(void) {
    printf("Test verifier_abbaye_complete...\n");
    Plateau* p = init_plateau();
    int centre = TAILLE_MAX / 2;

    // On place l'abbaye au centre
    p->grille[centre][centre].center = ABBAYE;
    p->occupes[centre][centre] = 1;

    // Cas 1 : Abbaye incomplète (seulement le centre)
    if (verifier_abbaye_complete(p, centre, centre) == 0) {
        printf("[OK] Abbaye incomplète détectée.\n");
    } else {
        printf("[FAIL] L'abbaye ne devrait pas être complète.\n");
    }

    // Cas 2 : On remplit les 8 cases autour
    for (int i = centre - 1; i <= centre + 1; i++) {
        for (int j = centre - 1; j <= centre + 1; j++) {
            p->occupes[i][j] = 1;
        }
    }

    if (verifier_abbaye_complete(p, centre, centre) == 1) {
        printf("  -> OK\n");
    } else {
        printf("[FAIL] L'abbaye devrait être complète.\n");
    }

    free_plateau(p);
}

void test_verifie_ville_complete(void) {
    printf("Test verifie_ville_complete...\n");
    Plateau* p = init_plateau();
    int cx = TAILLE_MAX / 2;
    int cy = TAILLE_MAX / 2;

    // Création d'une petite ville de 2 tuiles face à face
    // Tuile A : Ville à l'Est, le reste en Prairie
    p->grille[cx][cy] = (Tuiles){.a=PRAIRIE, .b=VILLE, .c=PRAIRIE, .d=PRAIRIE};
    p->occupes[cx][cy] = 1;

    // Cas 1 : Ville ouverte (il n'y a rien à l'Est de la Tuile A)
    if (verifie_ville_complete(p, cx, cy) == 0) {
        printf("[OK] Ville ouverte détectée.\n");
    } else {
        printf("[FAIL] La ville est ouverte et ne devrait pas être complète.\n");
    }

    // Tuile B : Ville à l'Ouest (posée à droite de la Tuile A)
    p->grille[cx+1][cy] = (Tuiles){.a=PRAIRIE, .b=PRAIRIE, .c=PRAIRIE, .d=VILLE};
    p->occupes[cx+1][cy] = 1;

    // Cas 2 : Ville fermée (A.Est touche B.Ouest, les autres côtés sont Prairie)
    if (verifie_ville_complete(p, cx, cy) == 1) {
        printf("  -> OK\n");
    } else {
        printf("[FAIL] La ville devrait être complète.\n");
    }

    free_plateau(p);
}

void test_verifie_route_complete(void) {
    printf("Test verifie_route_complete...\n");
    Plateau* p = init_plateau();
    int cx = TAILLE_MAX / 2;
    int cy = TAILLE_MAX / 2;

    // Tuile 1 : Carrefour (bouchon) avec route vers l'Est
    p->grille[cx][cy] = (Tuiles){.center=CARREFOUR, .b=ROUTE_PRAIRIE};
    p->occupes[cx][cy] = 1;

    // Tuile 2 : Segment horizontal (Ouest-Est)
    p->grille[cx+1][cy] = (Tuiles){.center=PRAIRIE, .d=ROUTE_PRAIRIE, .b=ROUTE_PRAIRIE};
    p->occupes[cx+1][cy] = 1;

    // Cas 1 : Route ouverte à l'Est de la Tuile 2
    if (verifie_route_complete(p, cx, cy) == 0) {
        printf("  -> OK\n");
    } else {
        printf("[FAIL] La route est ouverte.\n");
    }

    // Tuile 3 : Carrefour (bouchon) avec route vers l'Ouest
    p->grille[cx+2][cy] = (Tuiles){.center=CARREFOUR, .d=ROUTE_PRAIRIE};
    p->occupes[cx+2][cy] = 1;

    // Cas 2 : Route fermée entre deux carrefours
    if (verifie_route_complete(p, cx, cy) == 1) {
        printf("  -> OK\n");
    } else {
        printf("[FAIL] La route devrait être complète.\n");
    }

    free_plateau(p);
}

int main(void) {
    printf("\n=== TESTS DU MODULE PLATEAU ===\n\n");
    test_init_plateau();
    test_peut_poser_tuile();
    test_poser_tuile();
    test_afficher_plateau();
    test_afficher_tour();
    test_afficher_tuile();
    test_verifier_abbaye_complete();
    test_verifie_ville_complete();
    test_verifie_route_complete();
    printf("\n=== TOUS LES TESTS PLATEAU RÉUSSIS ===\n");
    return 0;
}
