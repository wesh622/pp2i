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
    
    int centre = TAILLE_MAX / 2;
    assert(p->occupes[centre][centre] == 1);
    assert(p->occupes[0][0] == 0);
    assert(p->occupes[TAILLE_MAX-1][TAILLE_MAX-1] == 0);
    
    free_plateau(p);
    printf("  -> OK\n");
}

void test_peut_poser_tuile(void) {
    printf("Test peut_poser_tuile...\n");

    Plateau *p = init_plateau();
    int centre = TAILLE_MAX / 2;

    /*
     * Tuile de depart (tuiles_jeu[0]) : a=PRAIRIE(2), b=ROUTE(1), c=ROUTE(1), d=PRAIRIE(2)
     * Convention coordonnees : x=ligne (x+1=sud, x-1=nord), y=col (y+1=est, y-1=ouest)
     * Compatibilite : face voisin opposee doit egal face posee (VILLE~VILLE_BOUCLIER, ROUTE~CARREFOUR)
     */

    /* --- 1. Case deja occupee --- */
    assert(peut_poser_tuile(p, tuiles_jeu[1], centre, centre) == 0);

    /* --- 2. Hors limites --- */
    assert(peut_poser_tuile(p, tuiles_jeu[1], -1, 0) == 0);
    assert(peut_poser_tuile(p, tuiles_jeu[1], TAILLE_MAX, 0) == 0);

    /* --- 3. Aucun voisin --- */
    assert(peut_poser_tuile(p, tuiles_jeu[1], centre+2, centre) == 0);

    /* --- 4. Compatibilite dans les 4 directions avec la tuile de depart ---
     *
     * Sud (centre+1, centre) : tuile.a doit == c(ROUTE=1) de la tuile de depart
     *   tuiles_jeu[4] = {id=5, a=ROUTE(1), b=VILLE(3), c=VILLE(3), d=VILLE(3)}  → a=1 OK
     */
    assert(peut_poser_tuile(p, tuiles_jeu[4], centre+1, centre) == 1);

    /* Nord (centre-1, centre) : tuile.c doit == a(PRAIRIE=2) de la tuile de depart
     *   tuiles_jeu[6] = {id=7, a=PRAIRIE(2), b=PRAIRIE(2), c=PRAIRIE(2), d=VILLE(3)} → c=2 OK
     */
    assert(peut_poser_tuile(p, tuiles_jeu[6], centre-1, centre) == 1);

    /* Est (centre, centre+1) : tuile.d doit == b(ROUTE=1) de la tuile de depart
     *   tuiles_jeu[2] = {id=3, a=PRAIRIE(2), b=ROUTE(1), c=PRAIRIE(2), d=ROUTE(1)} → d=1 OK
     */
    assert(peut_poser_tuile(p, tuiles_jeu[2], centre, centre+1) == 1);

    /* Ouest (centre, centre-1) : tuile.b doit == d(PRAIRIE=2) de la tuile de depart
     *   tuiles_jeu[6] = {id=7, a=PRAIRIE(2), b=PRAIRIE(2), ...}                  → b=2 OK
     */
    assert(peut_poser_tuile(p, tuiles_jeu[6], centre, centre-1) == 1);

    /* --- 5. Incompatibilite ---
     * On place tuiles_jeu[4] au sud : sa face sud c=VILLE(3)
     * Une tuile en (centre+2, centre) doit avoir a=VILLE(3) pour etre compatible.
     */
    poser_tuile(p, tuiles_jeu[4], centre+1, centre);

    /* tuiles_jeu[0] : a=PRAIRIE(2) != VILLE(3) → incompatible */
    assert(peut_poser_tuile(p, tuiles_jeu[0], centre+2, centre) == 0);

    /* tuiles_jeu[13] = {id=14, a=VILLE(3), b=ROUTE(1), c=PRAIRIE(2), d=ROUTE(1)} → a=3 compatible */
    assert(peut_poser_tuile(p, tuiles_jeu[13], centre+2, centre) == 1);

    /* --- 6. Compatibilite apres liberation de la case centrale ---
     * tuiles_jeu[4] au sud a pour face nord a=ROUTE(1).
     * tuiles_jeu[0] : c=ROUTE(1) == a(1) de tuiles_jeu[4] → compatible
     * tuiles_jeu[6] : c=PRAIRIE(2) != ROUTE(1)            → incompatible
     */
    p->occupes[centre][centre] = 0;
    assert(peut_poser_tuile(p, tuiles_jeu[0], centre, centre) == 1);
    assert(peut_poser_tuile(p, tuiles_jeu[6], centre, centre) == 0);

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
