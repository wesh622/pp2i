#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "score.h"
#include "joueur.h"
#include "plateau.h"

// ============================================================================
// Tests unitaires pour le module score (calcul des points, gestion des meeples)
// ============================================================================

/**
 * Teste l'initialisation des scores à zéro.
 */
void test_init_scores(void) {
    printf("Test init_scores...\n");
    
    Joueur joueurs[3];
    for (int i = 0; i < 3; i++) {
        joueurs[i].score = 100;   // valeurs arbitraires
        joueurs[i].actif = 1;
        sprintf(joueurs[i].nom, "J%d", i+1);
    }
    
    init_scores(joueurs, 3);
    
    for (int i = 0; i < 3; i++) {
        assert(joueurs[i].score == 0);
    }
    printf("  -> OK\n");
}

/**
 * Teste l'ajout de points à un joueur (fonction ajouter_score).
 * Vérifie les cas d'index invalide et de joueur inactif.
 */
void test_ajouter_score(void) {
    printf("Test ajouter_score...\n");
    
    Joueur joueurs[3];
    for (int i = 0; i < 3; i++) {
        joueurs[i].score = 0;
        joueurs[i].actif = 1;
    }
    
    ajouter_score(joueurs, 0, 10);
    assert(joueurs[0].score == 10);
    
    ajouter_score(joueurs, 0, 5);
    assert(joueurs[0].score == 15);
    
    // Index invalide (hors tableau)
    ajouter_score(joueurs, 99, 100);
    assert(joueurs[0].score == 15); // inchangé
    
    // Joueur inactif : ne doit pas recevoir de points
    joueurs[1].actif = 0;
    ajouter_score(joueurs, 1, 50);
    assert(joueurs[1].score == 0); // pas ajouté
    
    printf("  -> OK\n");
}

/**
 * Teste l'affichage des scores (vérification visuelle).
 */
void test_afficher_scores(void) {
    printf("Test afficher_scores...\n");
    
    Joueur joueurs[2];
    joueurs[0].actif = 1;
    joueurs[0].score = 12;
    strcpy(joueurs[0].nom, "Alice");
    joueurs[1].actif = 1;
    joueurs[1].score = 8;
    strcpy(joueurs[1].nom, "Bob");
    
    afficher_scores(joueurs, 2);
    printf("  -> OK (verification visuelle)\n");
}

/**
 * Teste la détermination du joueur gagnant (score le plus élevé).
 * Gère les égalités (premier rencontré) et les joueurs inactifs.
 */
void test_joueur_gagnant(void) {
    printf("Test joueur_gagnant...\n");
    
    Joueur joueurs[4];
    for (int i = 0; i < 4; i++) {
        joueurs[i].actif = 1;
        joueurs[i].score = 0;
    }
    
    // Cas 1 : un gagnant clair (joueur 1 avec 12 points)
    joueurs[0].score = 5;
    joueurs[1].score = 12;
    joueurs[2].score = 8;
    int gagnant = joueur_gagnant(joueurs, 3);
    assert(gagnant == 1);
    
    // Cas 2 : nouveau leader (joueur 0 avec 20)
    joueurs[0].score = 20;
    gagnant = joueur_gagnant(joueurs, 3);
    assert(gagnant == 0);
    
    // Cas 3 : égalité (joueur 0 et 1 à 10) -> retourne le premier (0)
    joueurs[0].score = 10;
    joueurs[1].score = 10;
    joueurs[2].score = 5;
    gagnant = joueur_gagnant(joueurs, 3);
    assert(gagnant == 0);
    
    // Cas 4 : joueur inactif ignoré (joueur 0 inactif malgré son score élevé)
    joueurs[0].score = 100;
    joueurs[0].actif = 0;
    joueurs[1].score = 30;
    joueurs[2].score = 40;
    gagnant = joueur_gagnant(joueurs, 3);
    assert(gagnant == 2);   // joueur 2 (score 40) gagne
    
    printf("  -> OK\n");
}

/**
 * Teste le calcul des points pour une ville (compter_points_ville).
 * Construit une petite ville de 2 tuiles avec un bouclier.
 * Points = nombre de tuiles (2) + 1 pour le bouclier = 3.
 */
void test_compter_points_ville(void) {
    printf("Test compter_points_ville...\n");
    Plateau* p = init_plateau();
    int visite[TAILLE_MAX][TAILLE_MAX] = {0};
    int centre = TAILLE_MAX / 2;

    // Tuile 1: Ville au Nord et Est, avec bouclier
    Tuiles t1 = { .id = 99, .a = VILLE_BOUCLIER, .b = VILLE, .c = PRAIRIE, .d = PRAIRIE };
    // Tuile 2: Ville à l'Ouest (connectée à l'Est de t1)
    Tuiles t2 = { .id = 100, .a = PRAIRIE, .b = PRAIRIE, .c = PRAIRIE, .d = VILLE };

    p->grille[centre][centre] = t1;
    p->occupes[centre][centre] = 1;
    p->grille[centre+1][centre] = t2;
    p->occupes[centre+1][centre] = 1;

    int score = compter_points_ville(p, centre, centre, visite);
    assert(score == 3);
    printf("  -> OK\n");

    free_plateau(p);
}

/**
 * Teste le calcul des points pour une route (compter_points_route).
 * Une route de 3 tuiles (carrefour - segment - carrefour) donne 3 points.
 */
void test_compter_points_route(void) {
    printf("Test compter_points_route...\n");
    Plateau* p = init_plateau();
    int visite[TAILLE_MAX][TAILLE_MAX] = {0};
    int centre = TAILLE_MAX / 2;

    // Carrefour (début) avec route vers l'Est
    Tuiles carrefour = { .id = 1, .center = CARREFOUR, .b = ROUTE_PRAIRIE };
    // Segment horizontal
    Tuiles segment = { .id = 2, .center = PRAIRIE, .d = ROUTE_PRAIRIE, .b = ROUTE_PRAIRIE };
    // Carrefour (fin) avec route vers l'Ouest
    Tuiles carrefour2 = { .id = 3, .center = CARREFOUR, .d = ROUTE_PRAIRIE };

    p->grille[centre][centre] = carrefour;
    p->occupes[centre][centre] = 1;
    p->grille[centre+1][centre] = segment;
    p->occupes[centre+1][centre] = 1;
    p->grille[centre+2][centre] = carrefour2;
    p->occupes[centre+2][centre] = 1;

    int score = compter_points_route(p, centre, centre, visite);
    assert(score == 3);
    printf("  -> OK\n");

    free_plateau(p);
}

/**
 * Teste l'attribution des points aux joueurs et la récupération de leurs meeples
 * sur une zone scorée.
 * Simule Alice avec un meeple sur la zone, Bob sans meeple.
 * Vérifie qu'Alice reçoit les points et que son meeple est rendu.
 */
void test_attribution_et_retour_meeple(void) {
    printf("Test attribution_et_retour_meeple...\n");
    Joueur j[2];
    definirJoueur(&j[0], 1, "Alice");
    j[0].actif = 1;
    definirJoueur(&j[1], 2, "Bob");
    j[1].actif = 1;

    int visite[TAILLE_MAX][TAILLE_MAX] = {0};
    visite[50][50] = 1; // Zone scorée (case marquée)

    // Alice place son premier meeple sur la zone (posX=50, posY=50)
    j[0].stock[0].etat = 0;     // meeple posé
    j[0].stock[0].posX = 50;
    j[0].stock[0].posY = 50;

    // Bob n'a pas de meeple sur cette zone

    // Attribution de 4 points
    attribuer_points_et_recuperer_meeples(visite, 4, j, 2);

    // Vérifications
    int ok = 1;
    if (j[0].score != 4) ok = 0;                // Alice doit avoir les 4 points
    if (j[0].stock[0].etat != 1) ok = 0;       // Le meeple d'Alice doit être rendu (état=1)
    if (j[1].score != 0) ok = 0;                // Bob ne gagne rien

    if (ok) {
        printf("  -> OK\n");
    } else {
        printf("  -> ECHEC\n");
    }
}

int main(void) {
    printf("\n=== TESTS DU MODULE SCORE ===\n\n");
    test_init_scores();
    test_ajouter_score();
    test_afficher_scores();
    test_joueur_gagnant();
    test_compter_points_ville();
    test_compter_points_route();
    test_attribution_et_retour_meeple();
    printf("\n=== TOUS LES TESTS SCORE RÉUSSIS ===\n");
    return 0;
}
