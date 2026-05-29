#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "plateau.h"
#include "joueur.h"
#include "tuiles.h"
#include "config.h"

void afficher_plateau_cli(Plateau *p);
void afficher_plateau_cli_ameliore_pour_placer_tuile(Plateau *p, Tuiles *t);
void afficher_tour(int numero_tour, Joueur *j);
void afficher_tuile(Tuiles t);

/** Affichage amélioré (couleurs) **/
void afficher_tuile_compacte(Tuiles t);
void afficher_plateau_cli_ameliore(Plateau *p);
void afficher_plateau_pour_placer_tuile(Plateau *p, Pioche *pioche);
void afficher_tour_ameliore(int numero_tour, Joueur *j);

/* Scores et stats */
void afficher_scores(Joueur joueurs[], int nb_joueurs);
void afficher_config(config *conf, int nb_joueurs_total);
void afficher_pioche(Pioche *pioche);

/* Interactif */
int afficher_menu_options(void);
void afficher_tuile_piochee(Tuiles t);

/* Fin de partie */
void afficher_fin_de_partie(Joueur joueurs[], int nb_joueurs);


#endif
