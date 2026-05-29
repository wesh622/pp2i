#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "joueur.h"

void test_definirJoueur(void) {
    Joueur* j;
    definirJoueur(j, 1, "Alice");
    assert(j->idjoueur == 1);
    assert(strcmp(j->nom, "Alice") == 0);
    assert(j->score == 0);
    // Vérification du stock (le premier et le dernier)
    assert(j->stock[0].id == 1);
    assert(j->stock[0].etat == 1);
    assert(j->stock[6].id == 7);
    printf("test_definirJoueur: OK\n");
}

void test_definirIA(void) {
    Joueur* j;
    definirIA(j, 99);
    assert(j->idjoueur == 99);
    assert(strcmp(j->nom, "IA_99") == 0);
    assert(j->actif == 1);
    printf("test_definirIA: OK\n");
}

void test_enleverJoueur(void) {
    Joueur* j;
    definirJoueur(j, 1, "Bob");
    enleverJoueur(j);
    assert(j->actif == 0);
    assert(j->idjoueur == -1);
    assert(strcmp(j->nom, "") == 0);
    printf("test_enleverJoueur: OK\n");
}

void test_ajouterpoints(void) {
    Joueur* j;
    j->score = 10;
    ajouterpoints(j, 5);
    assert(j->score == 15);
    ajouterpoints(j, -3);
    assert(j->score == 12);
    printf("test_ajouterpoints: OK\n");
}

void test_meeple_logic(void) {
    Joueur* j;
    definirJoueur(j, 1, "Test");

    // Au début, il peut poser (doit retourner l'indice 0)
    int index = peutposermeeple(j);
    assert(index == 0);

    // On pose tous les meeples (7 au total)
    for (int i = 0; i < 7; i++) {
        posermeeple(j, i);
    }

    // Plus de meeple dispo
    assert(peutposermeeple(j) == -1);

    // On rend le meeple dont l'ID est 4 (indice 3)
    rendremeeple(j, 4);
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
