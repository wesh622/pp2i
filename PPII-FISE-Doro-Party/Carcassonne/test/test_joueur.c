#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "joueur.h"

// ============================================================================
// Tests unitaires pour le module joueur (définition, points, gestion des meeples)
// ============================================================================

/**
 * Teste la fonction definirJoueur() : initialisation d'un joueur classique.
 */
void test_definirJoueur(void) {
    Joueur* j;          // Pointeur non initialisé (problème potentiel)
    definirJoueur(j, 1, "Alice");
    // Vérification des champs de base
    assert(j->idjoueur == 1);
    assert(strcmp(j->nom, "Alice") == 0);
    assert(j->score == 0);
    // Vérification du stock de meeples (le premier et le dernier)
    assert(j->stock[0].id == 1);
    assert(j->stock[0].etat == 1);   // 1 = disponible
    assert(j->stock[6].id == 7);     // 7 meeples au total, id de 1 à 7
    printf("test_definirJoueur: OK\n");
}

/**
 * Teste la fonction definirIA() : initialisation d'un joueur IA.
 */
void test_definirIA(void) {
    Joueur* j;
    definirIA(j, 99);
    assert(j->idjoueur == 99);
    // Le nom est construit automatiquement "IA_<id>"
    assert(strcmp(j->nom, "IA_99") == 0);
    assert(j->actif == 1);   // L'IA est active par défaut
    printf("test_definirIA: OK\n");
}

/**
 * Teste la fonction enleverJoueur() : désactivation et remise à zéro.
 */
void test_enleverJoueur(void) {
    Joueur* j;
    definirJoueur(j, 1, "Bob");
    enleverJoueur(j);
    assert(j->actif == 0);          // Joueur désactivé
    assert(j->idjoueur == -1);      // ID réinitialisé
    assert(strcmp(j->nom, "") == 0);// Nom vidé
    printf("test_enleverJoueur: OK\n");
}

/**
 * Teste l'ajout de points (fonction ajouterpoints).
 * Gère également les valeurs négatives.
 */
void test_ajouterpoints(void) {
    Joueur* j;
    j->score = 10;
    ajouterpoints(j, 5);
    assert(j->score == 15);
    ajouterpoints(j, -3);   // Doit fonctionner (diminution)
    assert(j->score == 12);
    printf("test_ajouterpoints: OK\n");
}

/**
 * Teste la logique de pose et de reprise des meeples.
 * - Au début, un meeple disponible (indice 0)
 * - On pose les 7 meeples, plus aucun disponible
 * - On rend le meeple d'ID 4, il redevient disponible à l'indice 3
 */
void test_meeple_logic(void) {
    Joueur* j;
    definirJoueur(j, 1, "Test");

    // Au début, le premier meeple est disponible (indice 0)
    int index = peutposermeeple(j);
    assert(index == 0);

    // On pose tous les meeples (7 au total)
    for (int i = 0; i < 7; i++) {
        posermeeple(j, i);   // le meeple i passe à l'état "posé" (etat=0)
    }

    // Plus aucun meeple disponible
    assert(peutposermeeple(j) == -1);

    // On rend le meeple dont l'ID est 4 (qui correspond à l'indice 3)
    rendremeeple(j, 4);
    // Vérification : le meeple à l'indice 3 est à nouveau disponible
    assert(peutposermeeple(j) == 3);
    assert(j->stock[3].etat == 1);

    printf("test_meeple_logic: OK\n");
}

int main(void) {
    test_definirJoueur();
    test_definirIA();
    test_enleverJoueur();
    test_ajouterpoints();
    test_meeple_logic();
    printf("Tous les tests sont passés !\n");
    return 0;
}
