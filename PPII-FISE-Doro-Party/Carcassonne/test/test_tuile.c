#include <stdio.h>
#include "../include/tuiles.h"
#include <assert.h>

// ============================================================================
// Tests unitaires pour les rotations de tuiles (module tuiles)
// ============================================================================

/**
 * Teste la rotation d'une tuile vers la droite.
 * Les faces a, b, c, d sont décalées circulairement : a <- d, b <- a, c <- b, d <- c.
 */
void test_tourner_vers_droite(void) {
    Tuiles t = {1, 10, 20, 30, 40, 5};  // id=1, a=10, b=20, c=30, d=40, center=5
    tourner_vers_droite(&t);
    // Après rotation à droite : a devient ancien d (40), b devient ancien a (10),
    // c devient ancien b (20), d devient ancien c (30)
    assert(t.a == 40);
    assert(t.b == 10);
    assert(t.c == 20);
    assert(t.d == 30);
    
    // Deuxième rotation
    tourner_vers_droite(&t);
    assert(t.a == 30);
    assert(t.b == 40);
    assert(t.c == 10);
    assert(t.d == 20);
    
    printf("test_tourner_vers_droite OK\n");
}

/**
 * Teste la rotation d'une tuile vers la gauche.
 * Décalage inverse : a <- b, b <- c, c <- d, d <- a.
 */
void test_tourner_vers_gauche(void) {
    Tuiles t = {2, 1, 2, 3, 4, 6};
    tourner_vers_gauche(&t);
    // Après rotation gauche : a devient ancien b (2), b devient ancien c (3),
    // c devient ancien d (4), d devient ancien a (1)
    assert(t.a == 2);
    assert(t.b == 3);
    assert(t.c == 4);
    assert(t.d == 1);
    
    // Deuxième rotation
    tourner_vers_gauche(&t);
    assert(t.a == 3);
    assert(t.b == 4);
    assert(t.c == 1);
    assert(t.d == 2);
    
    printf("test_tourner_vers_gauche OK\n");
}

int main(void) {
    test_tourner_vers_droite();
    test_tourner_vers_gauche();
    printf("Tous les tests tuiles passes !\n");
    return 0;
}
