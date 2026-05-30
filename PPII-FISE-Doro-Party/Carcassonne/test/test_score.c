#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "score.h"
#include "joueur.h"
#include "plateau.h"
#include "meeple.h"

static int centre;

static void setup_joueurs(Joueur j[], int nb) {
    char noms[4][4] = {"j1","j2","j3","j4"};
    for (int i = 0; i < nb; i++) {
        definirJoueur(&j[i], i+1, noms[i]);
        j[i].actif = 1;
    }
}

void test_init_scores(void) {
    printf("test_init_scores...\n");
    Joueur j[3];
    for (int i = 0; i < 3; i++) { j[i].score = 99; j[i].actif = 1; }
    init_scores(j, 3);
    for (int i = 0; i < 3; i++) assert(j[i].score == 0);
    printf("  -> OK\n");
}

void test_ajouter_score(void) {
    printf("test_ajouter_score...\n");
    Joueur j[3];
    for (int i = 0; i < 3; i++) { j[i].score = 0; j[i].actif = 1; }
    ajouter_score(j, 0, 10);
    assert(j[0].score == 10);
    ajouter_score(j, 0, 5);
    assert(j[0].score == 15);
    ajouter_score(j, 99, 100); // index invalide
    assert(j[0].score == 15);
    j[1].actif = 0;
    ajouter_score(j, 1, 50); // joueur inactif
    assert(j[1].score == 0);
    printf("  -> OK\n");
}

void test_afficher_scores(void) {
    printf("test_afficher_scores...\n");
    Joueur j[2];
    j[0].actif = 1; j[0].score = 12; strcpy(j[0].nom, "Alice");
    j[1].actif = 1; j[1].score = 8;  strcpy(j[1].nom, "Bob");
    afficher_scores(j, 2);
    printf("  -> OK (visuel)\n");
}

void test_joueur_gagnant(void) {
    printf("test_joueur_gagnant...\n");
    Joueur j[4];
    for (int i = 0; i < 4; i++) { j[i].actif = 1; j[i].score = 0; }
    j[0].score = 5; j[1].score = 12; j[2].score = 8;
    assert(joueur_gagnant(j, 3) == 1);
    j[0].score = 20;
    assert(joueur_gagnant(j, 3) == 0);
    j[0].score = 10; j[1].score = 10; j[2].score = 5;
    assert(joueur_gagnant(j, 3) == 0); // egalite -> premier
    j[0].score = 100; j[0].actif = 0; j[1].score = 30; j[2].score = 40;
    assert(joueur_gagnant(j, 3) == 2); // inactif ignore
    printf("  -> OK\n");
}

void test_compter_points_ville(void) {
    printf("test_compter_points_ville...\n");
    Plateau* p = init_plateau();
    int visite[TAILLE_MAX][TAILLE_MAX] = {0};
    // ville de 2 tuiles connectées est-ouest, 1 bouclier
    Tuiles t1 = { .id=99, .a=PRAIRIE, .b=VILLE_BOUCLIER, .c=PRAIRIE, .d=PRAIRIE, .center=PRAIRIE };
    Tuiles t2 = { .id=100, .a=PRAIRIE, .b=PRAIRIE,       .c=PRAIRIE, .d=VILLE,   .center=PRAIRIE };
    p->grille[centre][centre]   = t1; p->occupes[centre][centre]   = 1;
    p->grille[centre][centre+1] = t2; p->occupes[centre][centre+1] = 1;
    int pts = compter_points_ville(p, centre, centre, visite);
    assert(pts == 3); // 2 tuiles + 1 bouclier
    free_plateau(p);
    printf("  -> OK\n");
}

void test_compter_points_route(void) {
    printf("test_compter_points_route...\n");
    Plateau* p = init_plateau();
    int visite[TAILLE_MAX][TAILLE_MAX] = {0};
    // route de 3 tuiles entre deux dead-ends
    Tuiles a = { .id=1, .center=ROUTE_PRAIRIE, .b=ROUTE_PRAIRIE };
    Tuiles b = { .id=2, .center=ROUTE_PRAIRIE, .d=ROUTE_PRAIRIE, .b=ROUTE_PRAIRIE };
    Tuiles c = { .id=3, .center=ROUTE_PRAIRIE, .d=ROUTE_PRAIRIE };
    p->grille[centre][centre]   = a; p->occupes[centre][centre]   = 1;
    p->grille[centre][centre+1] = b; p->occupes[centre][centre+1] = 1;
    p->grille[centre][centre+2] = c; p->occupes[centre][centre+2] = 1;
    int pts = compter_points_route(p, centre, centre, visite);
    assert(pts == 3);
    free_plateau(p);
    printf("  -> OK\n");
}

void test_attribution_et_retour_meeple(void) {
    printf("test_attribution_et_retour_meeple...\n");
    Joueur j[2];
    definirJoueur(&j[0], 1, "Alice"); j[0].actif = 1;
    definirJoueur(&j[1], 2, "Bob");   j[1].actif = 1;
    int visite[TAILLE_MAX][TAILLE_MAX] = {0};
    visite[50][50] = 1;
    j[0].stock[0].etat = 0;
    j[0].stock[0].posX = 50;
    j[0].stock[0].posY = 50;
    attribuer_points_et_recuperer_meeples(visite, 4, j, 2, 0);
    assert(j[0].score == 4);
    assert(j[0].stock[0].etat == 1); // meeple rendu
    assert(j[1].score == 0);
    printf("  -> OK\n");
}

// route de 2 tuiles completee quand on pose la 2eme
void test_route_simple_complete(void) {
    printf("test_route_simple_complete...\n");
    Plateau* p = init_plateau();
    Joueur j[2];
    setup_joueurs(j, 2);
    // tuile gauche : route vers l'est uniquement
    Tuiles tA = { .id=99, .a=PRAIRIE, .b=ROUTE_PRAIRIE, .c=PRAIRIE, .d=PRAIRIE, .center=ROUTE_PRAIRIE };
    // tuile droite : route vers l'ouest uniquement (dead-end)
    Tuiles tB = { .id=100, .a=PRAIRIE, .b=PRAIRIE, .c=PRAIRIE, .d=ROUTE_PRAIRIE, .center=ROUTE_PRAIRIE };
    p->grille[centre][centre]   = tA; p->occupes[centre][centre]   = 1;
    // meeple j1 sur tA
    j[0].stock[0].etat = 0;
    j[0].stock[0].posX = centre; j[0].stock[0].posY = centre;
    j[0].stock[0].zone = ROUTE_PRAIRIE;
    // pose tB et score
    p->grille[centre][centre+1] = tB; p->occupes[centre][centre+1] = 1;
    verifier_et_scorer_structures(p, centre, centre+1, j, 2);
    assert(j[0].score == 2); // 2 tuiles = 2 pts
    assert(j[0].stock[0].etat == 1); // meeple recupere
    assert(j[1].score == 0);
    free_plateau(p);
    printf("  -> OK\n");
}

// route fermee par un carrefour
void test_route_carrefour(void) {
    printf("test_route_carrefour...\n");
    Plateau* p = init_plateau();
    Joueur j[2];
    setup_joueurs(j, 2);
    // carrefour au centre avec route vers l'est uniquement
    Tuiles car = { .id=99, .a=PRAIRIE, .b=ROUTE_PRAIRIE, .c=PRAIRIE, .d=PRAIRIE, .center=CARREFOUR };
    // route simple a l'est (dead-end ouest)
    Tuiles route = { .id=100, .a=PRAIRIE, .b=PRAIRIE, .c=PRAIRIE, .d=ROUTE_PRAIRIE, .center=ROUTE_PRAIRIE };
    p->grille[centre][centre+1] = route; p->occupes[centre][centre+1] = 1;
    // meeple j1 sur la route
    j[0].stock[0].etat = 0;
    j[0].stock[0].posX = centre; j[0].stock[0].posY = centre+1;
    j[0].stock[0].zone = ROUTE_PRAIRIE;
    // pose le carrefour -> doit fermer la branche est
    p->grille[centre][centre] = car; p->occupes[centre][centre] = 1;
    verifier_et_scorer_structures(p, centre, centre, j, 2);
    assert(j[0].score == 2); // carrefour(1) + route(1) = 2 pts
    assert(j[0].stock[0].etat == 1);
    free_plateau(p);
    printf("  -> OK\n");
}

// ville completee de 2 tuiles
void test_ville_complete(void) {
    printf("test_ville_complete...\n");
    Plateau* p = init_plateau();
    Joueur j[2];
    setup_joueurs(j, 2);
    // ville 2 tuiles nord-sud sans bouclier
    Tuiles tNord = { .id=99, .a=PRAIRIE, .b=PRAIRIE, .c=VILLE, .d=PRAIRIE, .center=PRAIRIE };
    Tuiles tSud  = { .id=100, .a=VILLE,  .b=PRAIRIE, .c=PRAIRIE, .d=PRAIRIE, .center=PRAIRIE };
    p->grille[centre][centre]   = tNord; p->occupes[centre][centre]   = 1;
    j[0].stock[0].etat = 0;
    j[0].stock[0].posX = centre; j[0].stock[0].posY = centre;
    j[0].stock[0].zone = VILLE;
    p->grille[centre+1][centre] = tSud; p->occupes[centre+1][centre] = 1;
    verifier_et_scorer_structures(p, centre+1, centre, j, 2);
    assert(j[0].score == 4); // 2 tuiles * 2 = 4 pts (ville complete)
    assert(j[0].stock[0].etat == 1);
    free_plateau(p);
    printf("  -> OK\n");
}

// ville avec bouclier
void test_ville_bouclier(void) {
    printf("test_ville_bouclier...\n");
    Plateau* p = init_plateau();
    Joueur j[2];
    setup_joueurs(j, 2);
    Tuiles tNord = { .id=99, .a=PRAIRIE, .b=PRAIRIE, .c=VILLE_BOUCLIER, .d=PRAIRIE, .center=PRAIRIE };
    Tuiles tSud  = { .id=100,.a=VILLE,   .b=PRAIRIE, .c=PRAIRIE, .d=PRAIRIE, .center=PRAIRIE };
    p->grille[centre][centre]   = tNord; p->occupes[centre][centre]   = 1;
    j[0].stock[0].etat = 0;
    j[0].stock[0].posX = centre; j[0].stock[0].posY = centre;
    j[0].stock[0].zone = VILLE;
    p->grille[centre+1][centre] = tSud; p->occupes[centre+1][centre] = 1;
    verifier_et_scorer_structures(p, centre+1, centre, j, 2);
    assert(j[0].score == 6); // (2 tuiles + 1 bouclier) * 2 = 6 pts
    assert(j[0].stock[0].etat == 1);
    free_plateau(p);
    printf("  -> OK\n");
}

// egalite de meeples -> les deux joueurs scorent
void test_egalite_meeples(void) {
    printf("test_egalite_meeples...\n");
    Plateau* p = init_plateau();
    Joueur j[2];
    setup_joueurs(j, 2);
    Tuiles tA = { .id=99,  .a=PRAIRIE, .b=ROUTE_PRAIRIE, .c=PRAIRIE, .d=PRAIRIE, .center=ROUTE_PRAIRIE };
    Tuiles tB = { .id=100, .a=PRAIRIE, .b=PRAIRIE, .c=PRAIRIE, .d=ROUTE_PRAIRIE, .center=ROUTE_PRAIRIE };
    p->grille[centre][centre]   = tA; p->occupes[centre][centre]   = 1;
    p->grille[centre][centre+1] = tB; p->occupes[centre][centre+1] = 1;
    j[0].stock[0].etat = 0; j[0].stock[0].posX = centre; j[0].stock[0].posY = centre;
    j[1].stock[0].etat = 0; j[1].stock[0].posX = centre; j[1].stock[0].posY = centre+1;
    verifier_et_scorer_structures(p, centre, centre+1, j, 2);
    assert(j[0].score == 2 && j[1].score == 2); // egalite -> les deux recoivent
    assert(j[0].stock[0].etat == 1 && j[1].stock[0].etat == 1);
    free_plateau(p);
    printf("  -> OK\n");
}

// fin de partie : route incomplete 1pt/tuile
void test_score_incomplet_route(void) {
    printf("test_score_incomplet_route...\n");
    Plateau* p = init_plateau();
    Joueur j[2];
    setup_joueurs(j, 2);
    Tuiles tA = { .id=99,  .center=ROUTE_PRAIRIE, .b=ROUTE_PRAIRIE };
    Tuiles tB = { .id=100, .center=ROUTE_PRAIRIE, .d=ROUTE_PRAIRIE, .b=ROUTE_PRAIRIE };
    Tuiles tC = { .id=101, .center=ROUTE_PRAIRIE, .d=ROUTE_PRAIRIE };
    p->grille[centre][centre]   = tA; p->occupes[centre][centre]   = 1;
    p->grille[centre][centre+1] = tB; p->occupes[centre][centre+1] = 1;
    p->grille[centre][centre+2] = tC; p->occupes[centre][centre+2] = 1;
    j[0].stock[0].etat = 0; j[0].stock[0].posX = centre; j[0].stock[0].posY = centre+1;
    j[0].stock[0].zone = ROUTE_PRAIRIE;
    calculer_score_incomplet_fin_partie(p, j, 2);
    assert(j[0].score == 3); // 3 tuiles = 3 pts meme incomplete
    assert(j[0].stock[0].etat == 1);
    free_plateau(p);
    printf("  -> OK\n");
}

// fin de partie : ville incomplete 1pt/tuile
void test_score_incomplet_ville(void) {
    printf("test_score_incomplet_ville...\n");
    Plateau* p = init_plateau();
    Joueur j[2];
    setup_joueurs(j, 2);
    Tuiles t = { .id=99, .a=VILLE, .b=PRAIRIE, .c=PRAIRIE, .d=PRAIRIE, .center=PRAIRIE };
    p->grille[centre][centre] = t; p->occupes[centre][centre] = 1;
    j[0].stock[0].etat = 0; j[0].stock[0].posX = centre; j[0].stock[0].posY = centre;
    j[0].stock[0].zone = VILLE;
    calculer_score_incomplet_fin_partie(p, j, 2);
    assert(j[0].score == 1); // 1 tuile = 1 pt
    free_plateau(p);
    printf("  -> OK\n");
}

int main(void) {
    centre = TAILLE_MAX / 2;
    printf("\n=== TESTS MODULE SCORE ===\n\n");
    test_init_scores();
    test_ajouter_score();
    test_afficher_scores();
    test_joueur_gagnant();
    test_compter_points_ville();
    test_compter_points_route();
    test_attribution_et_retour_meeple();
    test_route_simple_complete();
    test_route_carrefour();
    test_ville_complete();
    test_ville_bouclier();
    test_egalite_meeples();
    test_score_incomplet_route();
    test_score_incomplet_ville();
    printf("\n=== TOUS LES TESTS SCORE OK ===\n");
    return 0;
}