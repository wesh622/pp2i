# Architecture SDL — Carcassonne

## Fichiers

### Headers (`include/`)

| Fichier | Rôle |
|---|---|
| `sdl_init.h` | Définit `ContexteSDL` (fenêtre, renderer, polices, textures) et les constantes globales (`TAILLE_CASE=60`, `RAYON_VUE=6`, `LARGEUR_FENETRE=1080`, `HAUTEUR_FENETRE=780`). Déclare `init_sdl()` et `close_sdl()`. |
| `sdl_rendu.h` | Déclare toutes les fonctions de dessin bas niveau : `sdl_texte()`, `sdl_dessiner_tuile()`, `sdl_dessiner_tuile_zoom()`, `couleur_joueur()`, `pixel_vers_grille()`. |
| `sdl_plateau.h` | Définit `VueSDL` (caméra : `centre_i`, `centre_j`) et déclare `vue_init()`, `sdl_afficher_plateau()`. |
| `sdl_panel.h` | Déclare `sdl_afficher_panel()` pour le panneau latéral droit (infos joueur, pioche, tuile courante). |
| `sdl_input.h` | Définit les constantes `ACTION_RIEN/POSER/ROTATION/QUITTER` et déclare `sdl_attendre_action()`. |
| `sdl_boucle.h` | Déclare `boucle_sdl_principale()`, le point d'entrée du mode graphique appelé depuis `main.c`. |
| `sdl_accueil.h` | Déclare `sdl_menu_principal()`, l'écran de démarrage avec règles et boutons. |
| `sdl_fin.h` | Déclare `sdl_afficher_fin_de_partie()`, l'overlay de résultats en fin de partie. |
| `sdl_meeple_overlay.h` | Déclare `sdl_choisir_emplacement_meeple()`, l'overlay de placement du meeple après qu'une tuile est posée. |

---

### Sources (`src/interfaces/SDL/`)

| Fichier | Rôle |
|---|---|
| `sdl_init.c` | Initialise SDL2, SDL_ttf, SDL_image. Crée la fenêtre et le renderer. Charge les polices TTF et les 73 textures PNG des tuiles. `close_sdl()` libère tout dans l'ordre inverse. |
| `sdl_rendu.c` | Fonctions de dessin réutilisables : calcul des rotations des tuiles, rendu d'une tuile (avec texture ou fallback couleur), rendu d'un meeple, affichage de texte centré via TTF. |
| `sdl_plateau.c` | Calcule quelles tuiles sont visibles selon la caméra `VueSDL`, dessine la grille (tuiles posées + cases vides disponibles en vert), place les meeples sur les cases. |
| `sdl_panel.c` | Dessine le panneau droit : nom et score de chaque joueur, meeples restants, nombre de tuiles en pioche, prévisualisation de la tuile courante. |
| `sdl_input.c` | Écoute un événement SDL (`SDL_WaitEvent`). Gère clavier (rotation R/L, déplacement caméra ↑↓←→, Échap) et souris (clic gauche → case du plateau). Retourne une constante `ACTION_*`. |
| `sdl_boucle.c` | Chef d'orchestre. Orchestre toute la partie : init → menu → boucle des tours → fin. Pour chaque tour : pioche, IA ou humain, overlay meeple, scoring. |
| `sdl_accueil.c` | Affiche le menu de démarrage sur deux pages (accueil + règles). Gère les survols (`SDL_MOUSEMOTION`) et clics sur les boutons JOUER / RÈGLES / QUITTER. |
| `sdl_fin.c` | Affiche un overlay semi-transparent avec le gagnant en vert, le classement complet, et attend Espace/Échap pour terminer. |
| `sdl_meeple_overlay.c` | Affiche la tuile agrandie (240×240 px) avec 5 zones cliquables (Nord/Est/Sud/Ouest/Centre). Zones occupées = croix rouge. Retourne 1–5 (emplacement choisi) ou 0 (passer). |

---

## Flux d'exécution — du lancement à la fin de partie

```
main.c
  └── boucle_sdl_principale()          [sdl_boucle.c]
        │
        ├── init_sdl()                 [sdl_init.c]
        │     ├── SDL_Init(VIDEO)
        │     ├── TTF_Init()
        │     ├── SDL_CreateWindow()
        │     ├── SDL_CreateRenderer()
        │     ├── TTF_OpenFont() × 2   (police normale + grande)
        │     └── IMG_Load() × 73      (textures PNG des tuiles)
        │
        ├── sdl_menu_principal()        [sdl_accueil.c]
        │     ├── Affiche écran d'accueil (fond, titre, boutons)
        │     ├── Affiche page règles si demandé
        │     └── Retourne 1 (jouer) ou 0 (quitter)
        │
        │   [Si quitter → close_sdl() et return]
        │
        ├── vue_init()                 [sdl_plateau.c]
        │     └── Initialise la caméra au centre du plateau
        │
        └── for (chaque tour jusqu'à pioche vide)
              │
              ├── piocher()            [pioche.c — logique jeu]
              │
              ├── [Tour IA]
              │     ├── choix_case_IA()          [plateau.c]
              │     ├── tout_afficher()
              │     │     ├── SDL_RenderClear()
              │     │     ├── sdl_afficher_plateau()   [sdl_plateau.c]
              │     │     │     └── sdl_dessiner_tuile() × N  [sdl_rendu.c]
              │     │     ├── sdl_afficher_panel()     [sdl_panel.c]
              │     │     │     └── sdl_texte() × N          [sdl_rendu.c]
              │     │     └── SDL_RenderPresent()
              │     └── SDL_Delay(700ms)
              │
              └── [Tour humain]
                    │
                    ├── (boucle jusqu'à tuile posée)
                    │     ├── tout_afficher()    (même séquence que IA)
                    │     └── sdl_attendre_action()    [sdl_input.c]
                    │           ├── SDL_WaitEvent()    (bloquant)
                    │           ├── SDL_KEYDOWN → ROTATION / QUITTER / déplacement caméra
                    │           └── SDL_MOUSEBUTTONDOWN → ACTION_POSER + case (i,j)
                    │
                    ├── [Si ACTION_ROTATION]
                    │     └── tourner_vers_droite/gauche()   [plateau.c]
                    │
                    └── [Si ACTION_POSER valide]
                          ├── poser_tuile()              [plateau.c]
                          ├── verifier_et_scorer_structures()  [score.c]
                          │
                          └── [Si meeple disponible]
                                ├── tout_afficher()      (plateau sans tuile courante)
                                ├── calculer_zones_interdites()
                                └── sdl_choisir_emplacement_meeple()  [sdl_meeple_overlay.c]
                                      ├── Fond semi-transparent
                                      ├── sdl_dessiner_tuile_zoom()   [sdl_rendu.c]
                                      ├── Surlignage 5 zones (libre/interdit)
                                      ├── SDL_WaitEvent() → clic sur zone
                                      └── Retourne emplacement (1–5) ou 0

        [Fin de boucle]
        │
        ├── calculer_score_prairies_fin_partie()   [score.c]
        ├── tout_afficher()                        (état final)
        ├── sdl_afficher_fin_de_partie()           [sdl_fin.c]
        │     ├── Overlay noir alpha 210
        │     ├── Gagnant en vert, classement complet
        │     └── SDL_WaitEvent() → Espace/Échap
        │
        └── close_sdl()                            [sdl_init.c]
              ├── SDL_DestroyTexture() × 73+1
              ├── TTF_CloseFont() × 2
              ├── SDL_DestroyRenderer()
              ├── SDL_DestroyWindow()
              ├── TTF_Quit()
              └── SDL_Quit()
```

---

## Dépendances entre modules

```
sdl_boucle.c
  ├── sdl_init.c        (cycle de vie SDL)
  ├── sdl_accueil.c     (menu)
  ├── sdl_plateau.c ──► sdl_rendu.c   (dessin tuiles/meeples)
  ├── sdl_panel.c   ──► sdl_rendu.c   (textes panneau)
  ├── sdl_input.c   ──► sdl_plateau.c (pixel_vers_grille)
  ├── sdl_meeple_overlay.c ──► sdl_rendu.c (tuile zoomée + textes)
  └── sdl_fin.c     ──► sdl_rendu.c   (textes résultats)

sdl_rendu.c  ← utilisé par TOUS les modules d'affichage
sdl_init.c   ← fournit ContexteSDL à TOUS les modules
```

---

## Constantes clés (définies dans `sdl_init.h`)

| Constante | Valeur | Usage |
|---|---|---|
| `TAILLE_CASE` | 60 px | Taille d'une tuile sur le plateau |
| `RAYON_VUE` | 6 | Demi-largeur de la fenêtre de caméra (13×13 tuiles visibles) |
| `LARGEUR_PANEL` | 300 px | Largeur du panneau latéral droit |
| `LARGEUR_FENETRE` | 1080 px | `TAILLE_CASE × (2×RAYON_VUE+1) + LARGEUR_PANEL` |
| `HAUTEUR_FENETRE` | 780 px | `TAILLE_CASE × (2×RAYON_VUE+1)` |