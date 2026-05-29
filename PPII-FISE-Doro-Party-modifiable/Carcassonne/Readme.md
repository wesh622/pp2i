# Carcassonne — PPII FISE

Implémentation du jeu de société Carcassonne en C, avec mode CLI (terminal) et mode graphique SDL.

---

## Membres du groupe

- Omar
- Moran
- *(compléter)*

---

## Dépendances

| Bibliothèque | Usage |
|---|---|
| `libsdl2-dev` | Fenêtre, renderer, événements souris/clavier |
| `libsdl2-ttf-dev` | Rendu du texte en SDL |
| `libsdl2-image-dev` | (réservé pour futures images de tuiles) |
| `clang` | Compilateur C (C99) |

Installer d'un coup :

```bash
sudo make req
```

> Nécessite `apt-get` (Debian/Ubuntu). Sur macOS : `brew install sdl2 sdl2_ttf`.

---

## Compilation

```bash
# Version CLI uniquement
make

# Version SDL
make sdl
```

Les binaires générés :
- `carcassonne` — version CLI
- `carcassonne_sdl` — version SDL (compilé avec `-DAVEC_SDL`)

```bash
# Nettoyer les artefacts
make clean

# Lancer les tests unitaires
make test
make test_plateau
make test_score
make test_joueur
make test_pioche
make test_tuile
make test_meeple
```

---

## Lancement

### Mode CLI

```bash
make cli
# ou directement :
./carcassonne -m 1 -p 2
```

### Mode SDL

```bash
make sdl
# ou directement :
./carcassonne_sdl -m 2 -p 2
```

---

## Paramètres

| Paramètre | Forme courte | Défaut | Description |
|---|:---:|:---:|---|
| `--mode MODE` | `-m` | `1` | Mode d'affichage : `1`=CLI, `2`=SDL |
| `--players N` | `-p` | `2` | Nombre de joueurs humains (2 à 5) |
| `--ai N` | `-ai` | `0` | Nombre de joueurs IA |
| `--seed N` | `-s` | aléatoire | Graine (même seed = même partie) |
| `--max-turns N` | `-t` | 71 | Nombre de tours maximum |

Exemples :

```bash
# 2 humains en SDL
./carcassonne_sdl -m 2 -p 2

# 1 humain + 1 IA en SDL avec seed fixe
./carcassonne_sdl -m 2 -p 1 -ai 1 -s 42

# 3 joueurs en CLI
./carcassonne -m 1 -p 3

# 2 IA seulement (partie automatique)
./carcassonne_sdl -m 2 -p 0 -ai 2
```

---

## Contrôles SDL

| Action | Effet |
|---|---|
| `Clic gauche` sur case verte | Poser la tuile à cet emplacement |
| `R` | Tourner la tuile à droite |
| `L` | Tourner la tuile à gauche |
| `Flèches directionnelles` | Déplacer la vue sur le plateau |
| `Clic gauche` sur zone jaune | Placer un meeple (N / E / S / O / Centre) |
| `Espace` ou `Entrée` | Passer (ne pas poser de meeple) |
| `Échap` | Quitter la partie |

---

## Structure du projet

```
Carcassonne/
├── include/           # Headers (.h)
│   ├── tuiles.h       # Définition des tuiles et constantes de zone
│   ├── plateau.h      # Plateau de jeu (grille 72x72)
│   ├── joueur.h       # Structure Joueur
│   ├── meeple.h       # Structure Meeple
│   ├── pioche.h       # Pioche (deck de tuiles)
│   ├── score.h        # Calcul des scores
│   ├── affichage.h    # Affichage CLI (ANSI couleurs)
│   ├── config.h       # Configuration de la partie
│   ├── argc.h         # Parsing des arguments
│   ├── sdl_init.h     # Init SDL (fenêtre, renderer, TTF)
│   ├── sdl_rendu.h    # Dessin d'une tuile en SDL
│   ├── sdl_plateau.h  # Rendu du plateau SDL + conversion coordonnées
│   ├── sdl_panel.h    # Panneau droit (scores, meeples, pioche)
│   ├── sdl_meeple_overlay.h  # Overlay choix emplacement meeple
│   ├── sdl_input.h    # Gestion événements SDL
│   ├── sdl_boucle.h   # Boucle de jeu SDL
│   └── sdl_fin.h      # Écran fin de partie SDL
├── src/
│   ├── jeu/           # Moteur de jeu (partagé CLI et SDL)
│   │   ├── main.c
│   │   ├── tuiles.c
│   │   ├── plateau.c
│   │   ├── joueur.c
│   │   ├── meeple.c
│   │   ├── pioche.c
│   │   ├── score.c
│   │   └── affichage.c
│   ├── autres/        # Config et parsing arguments
│   │   ├── argc.c
│   │   └── config.c
│   └── interfaces/    # Couche SDL (compilée uniquement avec -DAVEC_SDL)
│       ├── sdl_init.c
│       ├── sdl_rendu.c
│       ├── sdl_plateau.c
│       ├── sdl_panel.c
│       ├── sdl_meeple_overlay.c
│       ├── sdl_input.c
│       ├── sdl_boucle.c
│       └── sdl_fin.c
├── test/              # Tests unitaires
└── ressources/        # Assets (images, polices)
```

---

## Architecture

Le moteur de jeu (`src/jeu/`) est **entièrement indépendant** de l'interface :
- `plateau.c` / `score.c` / `tuiles.c` ne font aucun appel SDL ni printf d'affichage
- `affichage.c` gère l'affichage CLI (ANSI couleurs)
- `src/interfaces/sdl_*.c` gère l'affichage SDL, compilé uniquement avec `-DAVEC_SDL`
- `main.c` branche sur `conf->mode` : mode 1 → CLI, mode 2 → SDL

---

## Règles implémentées

- Placement de tuiles avec validation d'adjacence (N/E/S/O compatibles)
- Rotation des tuiles (droite, gauche, demi-tour)
- Placement de meeples sur les zones (route, ville, abbaye, prairie, carrefour)
- Scoring en cours de partie : routes complètes, villes complètes, abbayes complètes
- Scoring final : structures incomplètes comptent partiellement
- IA : sélection d'un emplacement valide aléatoire

---

## Limitations connues

- Les **paysans** (prairies en fin de partie) ne sont pas scorés
- L'**IA** joue un coup légal aléatoire, sans stratégie
- Pas de détection de **fin de pioche** comme condition d'arrêt automatique (utiliser `-t` pour limiter les tours)
- La **police TTF** est chargée depuis les chemins système courants ; si absente, le texte SDL est désactivé (les tuiles restent visibles)
- Pas de **sauvegarde** de partie

---

## Tests

```bash
make test          # test_argc (parsing arguments)
make test_plateau  # placement et validation de tuiles
make test_score    # calcul des scores
make test_joueur   # gestion des joueurs
make test_pioche   # pioche et mélange
make test_tuile    # rotation des tuiles
make test_meeple   # placement des meeples
```
