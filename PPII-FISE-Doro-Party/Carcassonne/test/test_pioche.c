#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/pioche.h"

// ============================================================================
// Tests unitaires pour le module pioche (gestion du paquet de tuiles)
// ============================================================================

/**
 * Teste l'initialisation d'une pioche.
 * Vérifie que le nombre de tuiles restantes est 71 (pour une extension standard)
 * et que l'indice courant est 0.
 */
void test_init_pioche(void) {
    Pioche* p = init_pioche(42);   // 42 = graine aléatoire (non utilisée ici)
    assert(p != NULL);
    assert(p->nbresrestantes == 71);
    assert(p->indicecourant == 0);
    free_pioche(p);
    printf("test_init_pioche OK\n");
}

/**
 * Teste la fonction piocher().
 * Vérifie qu'une tuile est bien retournée et que le compteur diminue.
 */
void test_piocher(void) {
    Pioche* p = init_pioche(123);
    int n = p->nbresrestantes;
    Tuiles* t = piocher(p);
    assert(t != NULL);
    assert(p->nbresrestantes == n - 1);
    assert(p->indicecourant == 1);
    free_pioche(p);
    printf("test_piocher OK\n");
}

/**
 * Teste le comportement quand la pioche est vide.
 * Après 71 pioches, pioche_vide() doit retourner 1 et piocher() retourne NULL.
 */
void test_pioche_vide(void) {
    Pioche* p = init_pioche(1);
    for(int i = 0; i < 71; i++) {
        assert(pioche_vide(p) == 0);
        Tuiles* t = piocher(p);
        assert(t != NULL);
    }
    assert(pioche_vide(p) == 1);
    Tuiles* t = piocher(p);
    assert(t == NULL);
    free_pioche(p);
    printf("test_pioche_vide OK\n");
}

/**
 * Teste la libération de la mémoire (free_pioche).
 * Vérifie simplement qu'il n'y a pas de crash.
 */
void test_free_pioche(void) {
    Pioche* p = init_pioche(7);
    free_pioche(p);
    printf("test_free_pioche OK\n");
}

int main(void) {
    test_init_pioche();
    test_piocher();
    test_pioche_vide();
    test_free_pioche();
    printf("Tous les tests pioche passes !\n");
    return 0;
}
