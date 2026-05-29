    #include <stdio.h>
    #include <stdlib.h>
    #include <assert.h>
    #include "plateau.h"
    #include "tuiles.h"
    #include "joueur.h"
    #include "pioche.h"
    #include "meeple.h"
    #include "config.h"
    #include "score.h"


    Plateau* init_plateau(void){
        Plateau* p = malloc(sizeof(Plateau));
        assert(p != NULL);
        
        //initialisation de toutes les cases a vide
        for(int i=0;i<TAILLE_MAX;i++){
            for(int j=0;j<TAILLE_MAX;j++){
                p->occupes[i][j] = 0;
            }
        }
        
        //placement de la tuile de depart au centre du plateau
        int centre = TAILLE_MAX/2;
        p->grille[centre][centre] = tuiles_jeu[0];
        p->occupes[centre][centre] = 1;
        
        printf("Plateau initialise. Tuile de depart en (%d,%d)\n",centre,centre);
        
        return p;
    }
    /*
    int validation_emplacement_tuile(Plateau* plateau, int x, int y) {
        if (plateau->occupes[x][y] == 1) {
            printf("Cette case est occupée, veuillez en choisir une autre\n");
            return 1;      
        }
        int a = plateau->occupes[x+1][y];   //est ce qu'il y a une tuile à droite
        int b = plateau->occupes[x-1][y];   //est ce qu'il y a une tuile à gauche
        int c = plateau->occupes[x][y+1];  //est ce qu'il y a une tuile au dessus
        int d = plateau->occupes[x][y-1];   //est ce qu'il y a une tuile en dessous
        int adjacence = a+b+c+d;
        else if (adjacence == 0) {
            printf("Cette case n'est reliée à aucune autre tuile, veuillez en choisir une autre\n");
            return 1;
        }
        


        // A FAIRE : ici il faudra vérifier que la tuile s'embrique bien avec celle autour


        else return 0;


    }

    */

    // fonction similaire à peut_poser_tuile mais sans les messages d'erreur, pour l'affichage du plateau By Mohamed
    int peut_poser_tuile_silent(Plateau* p, Tuiles* t, int x, int y){
        if(p->occupes[x][y] == 1) return 0;
        if(x < 0 || x >= TAILLE_MAX || y < 0 || y >= TAILLE_MAX) return 0;
        
        int voisins = 0;
        if(x > 0 && p->occupes[x-1][y]) voisins++;
        if(x < TAILLE_MAX-1 && p->occupes[x+1][y]) voisins++;
        if(y > 0 && p->occupes[x][y-1]) voisins++;
        if(y < TAILLE_MAX-1 && p->occupes[x][y+1]) voisins++;
        if (voisins == 0) return 0;
        int compatible = 0;
        if(x < TAILLE_MAX-1 && p->occupes[x+1][y]){
            int surface = p->grille[x+1][y].a;
            if (
                surface != t->c &&
                !((surface == 3 && t->c == 4) || (surface == 4 && t->c == 3) ||
                (surface == 6 && t->c == 1) || (surface == 1 && t->c == 6))
            ) {
                compatible = 1;
            }
        }
        if(x > 0 && p->occupes[x-1][y]){
            int surface = p->grille[x-1][y].c;
            if (
                surface != t->a &&
                !((surface == 3 && t->a == 4) || (surface == 4 && t->a == 3) ||
                (surface == 6 && t->a == 1) || (surface == 1 && t->a == 6))
            ) {
                compatible = 1;
            }
        }
        if(y < TAILLE_MAX-1 && p->occupes[x][y+1]){
            int surface = p->grille[x][y+1].d;
            if (
                surface != t->b &&
                !((surface == 3 && t->b == 4) || (surface == 4 && t->b == 3) ||
                (surface == 6 && t->b == 1) || (surface == 1 && t->b == 6))
            ) {
                compatible = 1;
            }
        }
        if(y > 0 && p->occupes[x][y-1]){
            int surface = p->grille[x][y-1].b;
            if (
                surface != t->d &&
                !((surface == 3 && t->d == 4) || (surface == 4 && t->d == 3) ||
                (surface == 6 && t->d == 1) || (surface == 1 && t->d == 6))
            ) {
                compatible = 1;
            }
        }
        if (compatible == 1) return 0;
        return 1;
    }

    int peut_poser_tuile(Plateau* p, Tuiles t, int x, int y){
        //verif1 : la case est elle occupee ?
        if(p->occupes[x][y] == 1){
            printf("Cette case est occupee, veuillez en choisir une autre\n");
            return 0;
        }
        
        //verification 2 : coordonne hors limites ?
        if(x < 0 || x >= TAILLE_MAX || y < 0 || y >= TAILLE_MAX){
            printf("Coordonnees hors du plateau\n");
            return 0;
        }
        
        //verification 3 : au moins une tuile adjacente ?
        int a = 0; //tuile a droite
        int b = 0; //tuile a gauche  
        int c = 0; //tuile en haut
        int d = 0; //tuile en bas
        
        if(x > 0 && p->occupes[x-1][y]){
            b = 1;
        }
        if(x < TAILLE_MAX-1 && p->occupes[x+1][y]){
            a = 1;
        }
        if(y > 0 && p->occupes[x][y-1]){
            d = 1;
        }
        if(y < TAILLE_MAX-1 && p->occupes[x][y+1]){
            c = 1;
        }
        
        int adjacence = a+b+c+d;
        
        if(adjacence == 0){
            printf("Cette case n'est reliee a aucune autre tuile, veuillez en choisir une autre\n");
            return 0;
        }

        //verification 4 : Tuiles adjacentes compatibles
        // x+1 = voisin au sud  : sa face nord (.a) doit matcher notre face sud (t.c)
        // x-1 = voisin au nord : sa face sud  (.c) doit matcher notre face nord (t.a)
        // y+1 = voisin a l'est : sa face ouest (.d) doit matcher notre face est  (t.b)
        // y-1 = voisin a l'ouest : sa face est (.b) doit matcher notre face ouest (t.d)
        int compatible = 0;
        if(x < TAILLE_MAX-1 && p->occupes[x+1][y]){
            int surface = p->grille[x+1][y].a;
            if (
                surface != t.c &&
                !((surface == 3 && t.c == 4) || (surface == 4 && t.c == 3) ||
                (surface == 6 && t.c == 1) || (surface == 1 && t.c == 6))
            ) {
                compatible = 1;
            }
        }
        if(x > 0 && p->occupes[x-1][y]){
            int surface = p->grille[x-1][y].c;
            if (
                surface != t.a &&
                !((surface == 3 && t.a == 4) || (surface == 4 && t.a == 3) ||
                (surface == 6 && t.a == 1) || (surface == 1 && t.a == 6))
            ) {
                compatible = 1;
            }
        }
        if(y < TAILLE_MAX-1 && p->occupes[x][y+1]){
            int surface = p->grille[x][y+1].d;
            if (
                surface != t.b &&
                !((surface == 3 && t.b == 4) || (surface == 4 && t.b == 3) ||
                (surface == 6 && t.b == 1) || (surface == 1 && t.b == 6))
            ) {
                compatible = 1;
            }
        }
        if(y > 0 && p->occupes[x][y-1]){
            int surface = p->grille[x][y-1].b;
            if (
                surface != t.d &&
                !((surface == 3 && t.d == 4) || (surface == 4 && t.d == 3) ||
                (surface == 6 && t.d == 1) || (surface == 1 && t.d == 6))
            ) {
                compatible = 1;
            }
        }
        if (compatible == 1) {
            printf("La tuile n'est pas compatible avec les tuiles adjacentes, veuillez en choisir une autre\n");
            return 0;
        }
        return 1;
    }

    void poser_tuile(Plateau* p, Tuiles tuile, int x, int y){
        p->grille[x][y] = tuile;
        p->occupes[x][y] = 1;
        printf("Tuile %d posee en (%d,%d)\n",tuile.id,x,y);
    }

    void choix_case_IA(Plateau* p, Tuiles tuile, Joueur* joueur, config* conf, int total_joueurs) {
        for (int k=0; k<4; k++) {
            for (int i=1 ;i<= TAILLE_MAX-1 ;i++) {
                for(int j=1 ; j<= TAILLE_MAX-1 ;j++){
                    if(p->occupes[i][j]){
                        if (i + 1 < TAILLE_MAX && !p->occupes[i + 1][j]) {
                            if(peut_poser_tuile(p, tuile, i+1, j)) {
                                poser_tuile(p, tuile, i+1, j);
                                verifier_et_scorer_structures(p, i+1, j, conf->tab, total_joueurs);
                                if (au_moins_un_meeple_disponible(joueur) == 1) {
                                    choix_meeple_IA(p, tuile, joueur, i+1, j);
                                }
                                return;
                            }
                        }
                        else if (i - 1 >= 0 && !p->occupes[i - 1][j]) {
                            if(peut_poser_tuile(p, tuile, i-1, j)) {
                                poser_tuile(p, tuile, i-1, j);
                                verifier_et_scorer_structures(p, i-1, j, conf->tab, total_joueurs);
                                if (au_moins_un_meeple_disponible(joueur) == 1) {
                                    choix_meeple_IA(p, tuile, joueur, i-1, j);
                                }
                                return;
                            }
                        }
                        else if (j + 1 < TAILLE_MAX && !p->occupes[i][j + 1]) {
                            if(peut_poser_tuile(p, tuile, i, j+1)) {
                                poser_tuile(p, tuile, i, j+1);
                                verifier_et_scorer_structures(p, i, j+1, conf->tab, total_joueurs);
                                if (au_moins_un_meeple_disponible(joueur) == 1) {
                                    choix_meeple_IA(p, tuile, joueur, i, j+1);
                                }
                                return;
                            }
                        }
                        else if (j - 1 >= 0 && !p->occupes[i][j - 1]) {
                            if(peut_poser_tuile(p, tuile, i, j-1)) {
                                poser_tuile(p, tuile, i, j-1);
                                verifier_et_scorer_structures(p, i, j-1, conf->tab, total_joueurs);
                                if (au_moins_un_meeple_disponible(joueur) == 1) {
                                    choix_meeple_IA(p, tuile, joueur, i, j-1);
                                }
                                return;
                            }
                        }      
                    }
                }
            }
            tourner_vers_droite(&tuile);
        }
    }


    void afficher_plateau(Plateau* p){
        int centre = TAILLE_MAX/2;
        int rayon = 5; //affiche de centre-5 a centre+5
        
        printf("\n=== PLATEAU ===\n");
        printf("   ");
        for(int j=centre-rayon;j<=centre+rayon;j++){
            printf("%3d ",j);
        }
        printf("\n");
        
        for(int i=centre-rayon;i<=centre+rayon;i++){
            printf("%3d ",i);
            for(int j=centre-rayon;j<=centre+rayon;j++){
                if(p->occupes[i][j]){
                    printf("[%02d] ",p->grille[i][j].id);
                }
                else{
                    printf("\033[0m .. \033[0m"); // Rouge pour impossible
                }
            }
            printf("\n");
        }
        printf("\n");
    }

    void afficher_plateau_pour_placer_tuile(Plateau* p,Pioche* pioche){
        int centre = TAILLE_MAX/2;
        int rayon = 5; //affiche de centre-5 a centre+5
        Tuiles* t = &pioche->pile[pioche->indicecourant];
        
        printf("\n=== PLATEAU ===\n");
        printf("Tuile initiale placée au centre (détail) :\n");
        afficher_tuile(p->grille[centre][centre]);
        printf("\n");

        printf("   ");
        for(int j=centre-rayon;j<=centre+rayon;j++){
            printf("%3d ",j);
        }
        printf("\n");
        
        for(int i=centre-rayon;i<=centre+rayon;i++){
            printf("%3d ",i);
            for(int j=centre-rayon;j<=centre+rayon;j++){
                if(p->occupes[i][j]){
                    printf("[%02d] ",p->grille[i][j].id);
                }
                else{
                    // Vérifier si la tuile peut être posé
                    if(peut_poser_tuile_silent(p, t, i, j)==1){
                        printf("\033[32m .. \033[0m"); // Vert pour possible
                    }
                    else{
                        printf("\033[31m .. \033[0m"); // Rouge pour impossible
                    }
                }
            }
            printf("\n");
        }
        printf("\n");
    }

 //partie MOHAMED: rajout des fonctions complémentaire de l affichage 


char face_vers_char(int face) {
    switch(face) { //j'ai essayer de me baser sur les infos donner par antoine dans tuiles.h pour faire la correspondance entre les int et les types de faces mais je ne suis pas sur que ce soit correct, a verifier   
        case 1: return 'R'; // route_prairie
        case 2: return 'P'; // prairie
        case 3: return 'V'; // ville
        case 4: return 'B'; // ville_bouclier
        case 5: return 'A'; // abbaye
        case 6: return 'C'; // carrefour
        default: return '?';
    }
}

    void afficher_tour(int numero_tour, Joueur *j) {
        printf("--- Tour %d ---\n", numero_tour);
        printf("Joueur actif : %s (joueur %d)\n", j->nom, j->idjoueur);
        printf("Score actuel : %d pts\n", j->score);
        int meeples_restants = 0;
        for (int i = 0; i < 7; i++) {
            if (j->stock[i].etat == 1) {
                meeples_restants++;
            }
        }
        printf("Meeples restants : %d\n\n", meeples_restants); 
    }

    void afficher_tuile(Tuiles t) {
        printf("Tuile piochee (id=%d) :\n", t.id);
        printf("        [%c]\n", face_vers_char(t.a));
        printf("   [%c] [%c] [%c]\n", face_vers_char(t.d),
                                    face_vers_char(t.center),
                                    face_vers_char(t.b));
        printf("        [%c]\n\n", face_vers_char(t.c));
    }

    void free_plateau(Plateau* p){
        if(p){
            free(p);
        }
    }

int verifier_abbaye_complete(Plateau* p, int x, int y) {
    // x et y sont les coordonnées de l'abbaye à vérifier
    if (x < 0 || x >= TAILLE_MAX || y < 0 || y >= TAILLE_MAX) return 0;
    
    if (p->occupes[x][y] == 0 || p->grille[x][y].center != ABBAYE) {
        return 0;
    }

    int cases_pleines = 0;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && i < TAILLE_MAX && j >= 0 && j < TAILLE_MAX) {
                if (p->occupes[i][j]) {
                    cases_pleines++;
                }
            }
        }
    }
    
    if (cases_pleines == 9) {
        return 1; // Abbaye finie
    }
    
    return 0;
}

int ville_recursive(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]) {
    if (x < 0 || x >= TAILLE_MAX || y < 0 || y >= TAILLE_MAX || p->occupes[x][y] == 0) {
        return 0;
    }

    if (visite[x][y]) {
        return 1; // Déjà validé sur ce chemin
    } 

    visite[x][y] = 1; // On marque la tuile comme visitée
    Tuiles t = p->grille[x][y];

    // Vérification du NORD (a)
    if (t.a == VILLE || t.a == VILLE_BOUCLIER) {
        if (y + 1 >= TAILLE_MAX || p->occupes[x][y+1] == 0 || (p->grille[x][y+1].c != VILLE && p->grille[x][y+1].c != VILLE_BOUCLIER)) 
            return 0; // Bord vide ou type incompatible
        if (!ville_recursive(p, x, y+1, visite)) {
            return 0;
        } 
    }
    // Vérification de l'EST (b)
    if (t.b == VILLE || t.b == VILLE_BOUCLIER) {
        if (x + 1 >= TAILLE_MAX || p->occupes[x+1][y] == 0 || (p->grille[x+1][y].d != VILLE && p->grille[x+1][y].d != VILLE_BOUCLIER)) 
            return 0;
        if (!ville_recursive(p, x+1, y, visite)) {
            return 0;
        } 
    }
    // Vérification du SUD (c)
    if (t.c == VILLE || t.c == VILLE_BOUCLIER) {
        if (y - 1 < 0 || p->occupes[x][y-1] == 0 || (p->grille[x][y-1].a != VILLE && p->grille[x][y-1].a != VILLE_BOUCLIER)) 
            return 0;
        if (!ville_recursive(p, x, y-1, visite)) {
            return 0;
        }
    }
    // Vérification de l'OUEST (d)
    if (t.d == VILLE || t.d == VILLE_BOUCLIER) {
        if (x - 1 < 0 || p->occupes[x-1][y] == 0 || (p->grille[x-1][y].b != VILLE && p->grille[x-1][y].b != VILLE_BOUCLIER)) 
            return 0;
        if (!ville_recursive(p, x-1, y, visite)) {
            return 0;
        }
    }

    return 1;
}

int route_recursive(Plateau* p, int x, int y, int visite[TAILLE_MAX][TAILLE_MAX]) {
    if (x < 0 || x >= TAILLE_MAX || y < 0 || y >= TAILLE_MAX || p->occupes[x][y] == 0) {
        return 0; 
    }
    
    if (visite[x][y] == 1) { // Déjà validé sur ce chemin
        return 1; 
    }

    visite[x][y] = 1; // On marque la tuile comme visitée
    Tuiles t = p->grille[x][y];

    // Vérification du NORD (a)
    if (t.a == ROUTE_PRAIRIE) { // Si le côté Nord est une route
        if (y + 1 >= TAILLE_MAX || p->occupes[x][y+1] == 0 || p->grille[x][y+1].c != ROUTE_PRAIRIE) {
            return 0; // Vide ou pas de route en face
        }
        if (route_recursive(p, x, y + 1, visite) == 0) {
            return 0;
        } 
    }

    // Vérification de l'EST (b)
    if (t.b == ROUTE_PRAIRIE) {
        if (x + 1 >= TAILLE_MAX || p->occupes[x+1][y] == 0 || p->grille[x+1][y].d != ROUTE_PRAIRIE) {
            return 0;
        }
        if (route_recursive(p, x + 1, y, visite) == 0) {
            return 0;
        }
    }

    // Vérification du SUD (c)
    if (t.c == ROUTE_PRAIRIE) {
        if (y - 1 < 0 || p->occupes[x][y-1] == 0 || p->grille[x][y-1].a != ROUTE_PRAIRIE) {
            return 0;
        }
        if (route_recursive(p, x, y - 1, visite) == 0) return 0;
    }

    // Vérification de l'OUEST (d)
    if (t.d == ROUTE_PRAIRIE) {
        if (x - 1 < 0 || p->occupes[x-1][y] == 0 || p->grille[x-1][y].b != ROUTE_PRAIRIE) {
            return 0;
        }
        if (route_recursive(p, x - 1, y, visite) == 0) {
            return 0;
        }
    }

    return 1; // Tout est fermé ou connecté
}

int verifie_ville_complete(Plateau* p, int x, int y) {
    int visite[TAILLE_MAX][TAILLE_MAX] = {0}; 
    return ville_recursive(p, x, y, visite);
}

int verifie_route_complete(Plateau* p, int x, int y) {
    int visite[TAILLE_MAX][TAILLE_MAX] = {0}; 
    return route_recursive(p, x, y, visite);
}
