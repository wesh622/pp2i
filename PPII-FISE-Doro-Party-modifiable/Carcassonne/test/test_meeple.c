//Fichier test totalement generer avec IA(chatgpt en particulier) par Omar 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "meeple.h"
#include "plateau.h"
#include "joueur.h"

// Test 1 : Initialisation d'un meeple
void test_init_meeple(void) {
    printf("Test init_meeple...\n");
    
    Meeple m;
    init_meeple(&m, 3, 2);
    
    // Verifie les valeurs initiales
    assert(m.id == 3);
    assert(m.idjoueur == 2);
    assert(m.etat == 1);
    assert(m.zone == 0);
    assert(m.emplacement == 0);
    assert(m.posX == -1);
    assert(m.posY == -1);
    
    printf("  -> OK\n");
}

// Test 2 : Placement d'un meeple disponible
void test_placer_meeple_disponible(void) {
    printf("Test placer_meeple (meeple disponible)...\n");
    
    Meeple m;
    init_meeple(&m, 5, 1);
    
    // Tenter de placer le meeple
    int resultat = placer_meeple(&m, 10, 15, VILLE, 2);
    
    assert(resultat == 1);
    assert(m.posX == 10);
    assert(m.posY == 15);
    assert(m.zone == VILLE);
    assert(m.emplacement == 2);
    assert(m.etat == 0);
    
    printf("  -> OK\n");
}

// Test 3 : Tentative de placement d'un meeple deja pose
void test_placer_meeple_deja_pose(void) {
    printf("Test placer_meeple (meeple deja pose)...\n");
    
    Meeple m;
    init_meeple(&m, 2, 3);
    
    // Premier placement
    placer_meeple(&m, 5, 5, ROUTE_PRAIRIE, 1);
    
    // Tentative de second placement
    int resultat = placer_meeple(&m, 8, 8, VILLE, 3);
    
    assert(resultat == 0);
    assert(m.posX == 5);
    assert(m.posY == 5);
    
    printf("  -> OK\n");
}

// Test 4 : Verification si on peut placer un meeple - coordonnees valides
void test_peut_placer_meeple_coordonnees_valides(void) {
    printf("Test peut_placer_meeple (coordonnees valides)...\n");
    
    Plateau* p = init_plateau();
    int centre = TAILLE_MAX / 2;
    
    // Coordonnees valides sur tuile de depart
    assert(peut_placer_meeple(p, centre, centre, PRAIRIE, 1) == 1);
    
    free_plateau(p);
    printf("  -> OK\n");
}

// Test 5 : Verification si on peut placer un meeple - coordonnees hors limites
void test_peut_placer_meeple_hors_limites(void) {
    printf("Test peut_placer_meeple (hors limites)...\n");
    
    Plateau* p = init_plateau();
    
    // Coordonnees negatives
    assert(peut_placer_meeple(p, -1, 5, PRAIRIE, 1) == 0);
    assert(peut_placer_meeple(p, 5, -1, PRAIRIE, 1) == 0);
    
    // Coordonnees trop grandes
    assert(peut_placer_meeple(p, TAILLE_MAX, 5, PRAIRIE, 1) == 0);
    assert(peut_placer_meeple(p, 5, TAILLE_MAX, PRAIRIE, 1) == 0);
    
    free_plateau(p);
    printf("  -> OK\n");
}

// Test 6 : Verification si on peut placer un meeple - case vide
void test_peut_placer_meeple_case_vide(void) {
    printf("Test peut_placer_meeple (case vide)...\n");
    
    Plateau* p = init_plateau();
    int centre = TAILLE_MAX / 2;
    
    // Case vide adjacente au centre
    assert(peut_placer_meeple(p, centre + 1, centre, PRAIRIE, 1) == 0);
    
    free_plateau(p);
    printf("  -> OK\n");
}

// Test 7 : Verification si on peut placer un meeple - emplacement invalide
void test_peut_placer_meeple_emplacement_invalide(void) {
    printf("Test peut_placer_meeple (emplacement invalide)...\n");
    
    Plateau* p = init_plateau();
    int centre = TAILLE_MAX / 2;
    
    // Emplacement 0 (invalide)
    assert(peut_placer_meeple(p, centre, centre, PRAIRIE, 0) == 0);
    
    // Emplacement 6 (invalide)
    assert(peut_placer_meeple(p, centre, centre, PRAIRIE, 6) == 0);
    
    free_plateau(p);
    printf("  -> OK\n");
}

// Test 8 : Retirer un meeple
void test_retirer_meeple(void) {
    printf("Test retirer_meeple...\n");
    
    Meeple m;
    init_meeple(&m, 1, 4);
    
    // Placer le meeple
    placer_meeple(&m, 20, 25, VILLE_BOUCLIER, 4);
    
    // Verifier qu'il est pose
    assert(m.etat == 0);
    
    // Retirer le meeple
    retirer_meeple(&m);
    
    // Verifier qu'il est disponible
    assert(m.etat == 1);
    assert(m.posX == -1);
    assert(m.posY == -1);
    assert(m.zone == 0);
    assert(m.emplacement == 0);
    
    printf("  -> OK\n");
}

// Test 9 : Verfier si un meeple est disponible
void test_meeple_disponible(void) {
    printf("Test meeple_disponible...\n");
    
    Meeple m;
    init_meeple(&m, 7, 2);
    
    // Meeple disponible
    assert(meeple_disponible(&m) == 1);
    
    // Placer le meeple
    placer_meeple(&m, 12, 12, ABBAYE, 5);
    
    // Meeple non disponible
    assert(meeple_disponible(&m) == 0);
    
    // Retirer le meeple
    retirer_meeple(&m);
    
    // Meeple disponible a nouveau
    assert(meeple_disponible(&m) == 1);
    
    printf("  -> OK\n");
}

// Test 10 : Affichage d'un meeple
void test_afficher_meeple(void) {
    printf("Test afficher_meeple...\n");
    
    Meeple m;
    init_meeple(&m, 4, 3);
    
    printf("  Meeple disponible :\n  ");
    afficher_meeple(&m);
    
    placer_meeple(&m, 30, 40, ROUTE_PRAIRIE, 3);
    
    printf("  Meeple pose :\n  ");
    afficher_meeple(&m);
    
    printf("  -> OK (verification visuelle)\n");
}

// Test 11 : Affichage d'un meeple NULL
void test_afficher_meeple_null(void) {
    printf("Test afficher_meeple (NULL)...\n");
    
    // Ne doit pas crasher
    afficher_meeple(NULL);
    
    printf("  -> OK (pas de crash)\n");
}

// Test 12 : Cycle complet placement-retrait-replacement
void test_cycle_complet(void) {
    printf("Test cycle complet placement-retrait-replacement...\n");
    
    Meeple m;
    init_meeple(&m, 6, 1);
    
    // Cycle 1
    assert(placer_meeple(&m, 5, 5, VILLE, 2) == 1);
    assert(m.etat == 0);
    retirer_meeple(&m);
    assert(m.etat == 1);
    
    // Cycle 2
    assert(placer_meeple(&m, 10, 10, ROUTE_PRAIRIE, 1) == 1);
    assert(m.posX == 10);
    assert(m.posY == 10);
    retirer_meeple(&m);
    assert(m.posX == -1);
    
    // Cycle 3
    assert(placer_meeple(&m, 15, 20, ABBAYE, 5) == 1);
    assert(m.zone == ABBAYE);
    
    printf("  -> OK\n");
}

int main(void) {
    printf("\n=== TESTS DU MODULE MEEPLE ===\n\n");
    
    test_init_meeple();
    test_placer_meeple_disponible();
    test_placer_meeple_deja_pose();
    test_peut_placer_meeple_coordonnees_valides();
    test_peut_placer_meeple_hors_limites();
    test_peut_placer_meeple_case_vide();
    test_peut_placer_meeple_emplacement_invalide();
    test_retirer_meeple();
    test_meeple_disponible();
    test_afficher_meeple();
    test_afficher_meeple_null();
    test_cycle_complet();
    
    printf("\n=== TOUS LES TESTS MEEPLE REUSSIS ===\n");
    return 0;
}

