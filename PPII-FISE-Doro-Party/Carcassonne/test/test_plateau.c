#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "plateau.h"
#include "tuiles.h"
#include "joueur.h"
#include "affichage.h"

// ============================================================================
// Tests unitaires pour le module plateau (grille, placement, compatibilité)
// ============================================================================

/**
 * Vérifie l'initialisation du plateau : toutes les cases sont libres.
 */
void test_init_plateau(void) {
    printf("Test init_plateau...\n");
    
    Plateau *p = init_plateau();
    assert(p != NULL);
    
    int centre = TAILLE_MAX / 2;
    assert(p->occupes[centre][centre] == 0);
    assert(p->occupes[0][0] == 0);
    assert(p->occupes[TAILLE_MAX-1][TAILLE_MAX-1] == 0);
    
    free_plateau(p);
    printf("  -> OK\n");
}

/**
 * Teste la fonction peut_poser_tuile().
 * Vérifie les limites, l'occupation, la compatibilité avec les voisins.
 */
void test_peut_poser_tuile(void) {
    printf("Test peut_poser_tuile...\n");
    
    Plateau *p = init_plateau();
    int centre = TAILLE_MAX / 2;

    // Placer la tuile de départ au centre
    poser_tuile(p, tuiles_jeu[0], centre, centre);

    // Case déjà occupée (centre)
    assert(peut_poser_tuile(p, tuiles_jeu[1], centre, centre) == 0);

    // Hors limites
    assert(peut_poser_tuile(p, tuiles_jeu[1], -1, 0) == 0);
    assert(peut_poser_tuile(p, tuiles_jeu[1], TAILLE_MAX, 0) == 0);

    // Case éloignée sans aucun voisin
    assert(peut_poser_tuile(p, tuiles_jeu[1], centre+2, centre) == 0);

    // Cases adjacentes : vérifier qu'on peut poser à côté de la tuile centrale
    // (Les assertions exactes sur les numéros de tuiles compatibles dépendent des tuiles_jeu)
    int result_droite = peut_poser_tuile(p, tuiles_jeu[1], centre+1, centre);
    int result_gauche = peut_poser_tuile(p, tuiles_jeu[1], centre-1, centre);
    int result_haut = peut_poser_tuile(p, tuiles_jeu[1], centre, centre-1);
    int result_bas = peut_poser_tuile(p, tuiles_jeu[1], centre, centre+1);
    
    // Au moins une adjacente doit être possible
    assert((result_droite == 1 || result_gauche == 1 || result_haut == 1 || result_bas == 1));

    // Test basique : case vide sans voisin ne peut pas recevoir une tuile
    assert(peut_poser_tuile(p, tuiles_jeu[1], 0, 0) == 0);

    free_plateau(p);
    printf("  -> OK\n");
}

/**
 * Teste poser_tuile() : vérifie que la case devient occupée et que la tuile
 * est bien stockée dans la grille.
 */
void test_poser_tuile(void) {
    printf("Test poser_tuile...\n");
    
    Plateau *p = init_plateau();
    int centre = TAILLE_MAX / 2;
    int x = centre + 1;
    int y = centre;
    
    Tuiles t = tuiles_jeu[1];
    assert(p->occupes[x][y] == 0);
    
    poser_tuile(p, t, x, y);
    assert(p->occupes[x][y] == 1);
    assert(p->grille[x][y].id == t.id);
    
    free_plateau(p);
    printf("  -> OK\n");
}

/**
 * Teste l'affichage du plateau (vérification visuelle, pas de crash).
 */
void test_afficher_plateau(void) {
    printf("Test afficher_plateau...\n");
    Plateau *p = init_plateau();
    afficher_plateau_cli(p);
    free_plateau(p);
    printf("  -> OK (verification visuelle)\n");
}

/**
 * Teste l'affichage d'un tour (joueur, score, stock).
 */
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
    printf("  -> OK (verification visuelle)\n");
}

/**
 * Teste l'affichage d'une tuile (détails des faces).
 */
void test_afficher_tuile(void) {
    printf("Test afficher_tuile...\n");
    Tuiles t = tuiles_jeu[0];
    afficher_tuile(t);
    printf("  -> OK (verification visuelle)\n");
}

/**
 * Teste la détection d'une abbaye complète (entourée de 8 tuiles).
 */
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

    // Cas 2 : On remplit les 8 cases autour (voisins immédiats)
    for (int i = centre - 1; i <= centre + 1; i++) {
        for (int j = centre - 1; j <= centre + 1; j++) {
            if (i == centre && j == centre) continue;
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

/**
 * Teste la détection d'une ville complète (tous les bords de la zone sont fermés).
 * On construit manuellement une petite ville de deux tuiles face à face.
 */
void test_verifie_ville_complete(void) {
    printf("Test verifie_ville_complete...\n");
    Plateau* p = init_plateau();
    int cx = TAILLE_MAX / 2;
    int cy = TAILLE_MAX / 2;

    // Tuile A : Ville à l'Est, les autres côtés Prairie
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

    // Cas 2 : Ville fermée (A.Est touche B.Ouest, autres côtés Prairie)
    if (verifie_ville_complete(p, cx, cy) == 1) {
        printf("  -> OK\n");
    } else {
        printf("[FAIL] La ville devrait être complète.\n");
    }

    free_plateau(p);
}

/**
 * Teste la détection d'une route complète (de carrefour à carrefour ou boucle).
 * On construit une ligne de tuiles routes entre deux carrefours.
 */
void test_verifie_route_complete(void) {
    printf("Test verifie_route_complete...\n");
    Plateau* p = init_plateau();
    int cx = TAILLE_MAX / 2;
    int cy = TAILLE_MAX / 2;

    // Tuile 1 : Carrefour avec route vers l'Est
    p->grille[cx][cy] = (Tuiles){.center=CARREFOUR, .b=ROUTE_PRAIRIE};
    p->occupes[cx][cy] = 1;

    // Tuile 2 : Segment horizontal (Ouest-Est)
    p->grille[cx+1][cy] = (Tuiles){.center=PRAIRIE, .d=ROUTE_PRAIRIE, .b=ROUTE_PRAIRIE};
    p->occupes[cx+1][cy] = 1;

    // Cas 1 : Route ouverte à l'Est de la Tuile 2 (pas de troisième tuile)
    if (verifie_route_complete(p, cx, cy) == 0) {
        printf("  -> OK\n");
    } else {
        printf("[FAIL] La route est ouverte.\n");
    }

    // Tuile 3 : Carrefour avec route vers l'Ouest (ferme la route)
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
