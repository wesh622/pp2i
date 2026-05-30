#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "plateau.h"
#include "tuiles.h"
#include "joueur.h"
#include "affichage.h"
#include "pioche.h"
#include "config.h"

// ============================================================================
// Tests unitaires pour le module affichage
// ============================================================================

/**
 * Teste l'affichage simple du plateau.
 */
void test_afficher_plateau_cli(void) {
    printf("\nTest afficher_plateau_cli...\n");
    
    Plateau *p = init_plateau();
    assert(p != NULL);
    
    // Placer quelques tuiles
    int centre = TAILLE_MAX / 2;
    poser_tuile(p, tuiles_jeu[0], centre, centre);
    poser_tuile(p, tuiles_jeu[1], centre+1, centre);
    
    afficher_plateau_cli(p);
    
    free_plateau(p);
    printf("  -> OK (verification visuelle)\n");
}

/**
 * Teste l'affichage compact d'une tuile.
 */
void test_afficher_tuile_compacte(void) {
    printf("\nTest afficher_tuile_compacte...\n");
    
    afficher_tuile_compacte(tuiles_jeu[0]);
    
    printf("  -> OK\n");
}

/**
 * Teste l'affichage amélioré du plateau avec joueurs.
 */
void test_afficher_plateau_cli_ameliore(void) {
    printf("\nTest afficher_plateau_cli_ameliore...\n");
    
    Plateau *p = init_plateau();
    int centre = TAILLE_MAX / 2;
    poser_tuile(p, tuiles_jeu[0], centre, centre);
    
    // Créer des joueurs
    Joueur joueurs[2];
    for (int i = 0; i < 2; i++) {
        definirJoueur(&joueurs[i], i+1, (i == 0) ? "Alice" : "Bob");
    }
    
    afficher_plateau_cli_ameliore(p, joueurs, 2);
    
    free_plateau(p);
    printf("  -> OK (verification visuelle)\n");
}

/**
 * Teste l'affichage du tour amélioré.
 */
void test_afficher_tour_ameliore(void) {
    printf("\nTest afficher_tour_ameliore...\n");
    
    Joueur j;
    definirJoueur(&j, 1, "Alice");
    j.score = 42;
    
    afficher_tour_ameliore(1, &j);
    
    printf("  -> OK\n");
}


/**
 * Teste l'affichage de la pioche.
 */
void test_afficher_pioche(void) {
    printf("\nTest afficher_pioche...\n");
    
    Pioche *pioche = init_pioche(42);
    assert(pioche != NULL);
    
    afficher_pioche(pioche);
    
    free_pioche(pioche);
    printf("  -> OK\n");
}

/**
 * Teste l'affichage d'une tuile piochée.
 */
void test_afficher_tuile_piochee(void) {
    printf("\nTest afficher_tuile_piochee...\n");
    
    afficher_tuile_piochee(tuiles_jeu[0]);
    
    printf("  -> OK\n");
}

/**
 * Teste l'affichage de fin de partie.
 */
void test_afficher_fin_de_partie(void) {
    printf("\nTest afficher_fin_de_partie...\n");
    
    Joueur joueurs[2];
    definirJoueur(&joueurs[0], 1, "Alice");
    definirJoueur(&joueurs[1], 2, "Bob");
    
    joueurs[0].score = 85;
    joueurs[1].score = 42;
    
    afficher_fin_de_partie(joueurs, 2);
    
    printf("  -> OK\n");
}

/**
 * Teste l'affichage amélioré pour placer une tuile.
 */
void test_afficher_plateau_cli_ameliore_pour_placer_tuile(void) {
    printf("\nTest afficher_plateau_cli_ameliore_pour_placer_tuile...\n");
    
    Plateau *p = init_plateau();
    int centre = TAILLE_MAX / 2;
    poser_tuile(p, tuiles_jeu[0], centre, centre);
    
    Joueur joueurs[2];
    for (int i = 0; i < 2; i++) {
        definirJoueur(&joueurs[i], i+1, (i == 0) ? "Alice" : "Bob");
    }
    
    Tuiles tuile_test = tuiles_jeu[1];
    afficher_plateau_cli_ameliore_pour_placer_tuile(p, &tuile_test, joueurs, 2);
    
    free_plateau(p);
    printf("  -> OK (verification visuelle)\n");
}

int main(void) {
    printf("=== TESTS DU MODULE AFFICHAGE ===\n");
    
    test_afficher_plateau_cli();
    test_afficher_tuile_compacte();
    test_afficher_plateau_cli_ameliore();
    test_afficher_tour_ameliore();
    test_afficher_pioche();
    test_afficher_tuile_piochee();
    test_afficher_fin_de_partie();
    test_afficher_plateau_cli_ameliore_pour_placer_tuile();
    
    printf("\n=== TOUS LES TESTS SONT PASSES ===\n");
    return 0;
}
