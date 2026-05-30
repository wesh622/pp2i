.PHONY: test clean all req cli sdl
cc = clang
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -g -Iinclude
SDL_FLAGS  = $(shell sdl2-config --cflags --libs) -lSDL2_ttf -lSDL2_image

SRC = src/jeu/main.c \
      src/jeu/tuiles.c \
      src/jeu/plateau.c \
      src/jeu/pioche.c \
      src/jeu/joueur.c \
      src/jeu/score.c \
      src/jeu/affichage.c \
      src/autres/argc.c \
      src/autres/config.c \
	  src/jeu/meeple.c

SRC_SDL_INTERFACES = src/interfaces/SDL/sdl_init.c \
                     src/interfaces/SDL/sdl_rendu.c \
                     src/interfaces/SDL/sdl_plateau.c \
                     src/interfaces/SDL/sdl_panel.c \
                     src/interfaces/SDL/sdl_input.c \
                     src/interfaces/SDL/sdl_meeple_overlay.c \
                     src/interfaces/SDL/sdl_boucle.c \
                     src/interfaces/SDL/sdl_fin.c \
                     src/interfaces/SDL/sdl_accueil.c

OBJ = $(SRC:%.c=exec/%.o)

TESTSRC = test/test_argc.c
TESTOBJ = $(TESTSRC:.c=.o)

TESTSRC_PLATEAU = test/test_plateau.c
TEST_PLATEAU = test_plateau

TESTSRC_SCORE = test/test_score.c
TEST_SCORE = test_score

TESTSRC_JOUEUR = test/test_joueur.c
TEST_JOUEUR = test_joueur

TESTSRC_PIOCHE = test/test_pioche.c
TEST_PIOCHE = test_pioche

TESTSRC_TUILE = test/test_tuile.c
TEST_TUILE = test_tuile

TESTSRC_MEEPLE = test/test_meeple.c
TEST_MEEPLE = test_meeple

TESTSRC_AFFICHAGE = test/test_affichage.c
TEST_AFFICHAGE = test_affichage

NAME = carcassonne
NAME_SDL = carcassonne_sdl
TEST = test_argc

all: $(NAME)

exec/%.o: %.c
	@mkdir -p $(dir $@)
	$(cc) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(cc) $(CFLAGS) $(OBJ) -o $(NAME)

$(NAME_SDL): $(SRC) $(SRC_SDL_INTERFACES)
	@mkdir -p exec
	$(cc) $(CFLAGS) -DAVEC_SDL $(SDL_FLAGS) $^ -o $@

test: $(TESTOBJ) src/autres/argc.c src/autres/config.c src/jeu/joueur.c src/jeu/meeple.c
	CARCASSONNE_TEST=1 $(cc) $(CFLAGS) $(TESTSRC) src/autres/argc.c src/autres/config.c src/jeu/joueur.c src/jeu/meeple.c -o $(TEST)	
	./$(TEST)

test_plateau: $(TESTSRC_PLATEAU)
	$(cc) $(CFLAGS) $(TESTSRC_PLATEAU) src/jeu/plateau.c src/jeu/tuiles.c src/jeu/joueur.c src/jeu/meeple.c src/jeu/score.c src/jeu/pioche.c src/jeu/affichage.c -o $(TEST_PLATEAU)
	./$(TEST_PLATEAU)

test_score: $(TESTSRC_SCORE)
	$(cc) $(CFLAGS) $(TESTSRC_SCORE) src/jeu/score.c src/jeu/joueur.c src/jeu/plateau.c src/jeu/tuiles.c src/jeu/meeple.c -o $(TEST_SCORE)
	./$(TEST_SCORE)

test_pioche: $(TESTSRC_PIOCHE)
	$(cc) $(CFLAGS) $(TESTSRC_PIOCHE) src/jeu/pioche.c src/jeu/tuiles.c -o $(TEST_PIOCHE)
	./$(TEST_PIOCHE)

test_joueur: $(TESTSRC_JOUEUR)
	$(cc) $(CFLAGS) $(TESTSRC_JOUEUR) src/jeu/joueur.c src/jeu/meeple.c -o $(TEST_JOUEUR)
	./$(TEST_JOUEUR)

test_tuile: $(TESTSRC_TUILE)
	$(cc) $(CFLAGS) $(TESTSRC_TUILE) src/jeu/tuiles.c -o $(TEST_TUILE)
	./$(TEST_TUILE)

test_meeple: $(TESTSRC_MEEPLE)
	$(cc) $(CFLAGS) $(TESTSRC_MEEPLE) src/jeu/meeple.c src/jeu/plateau.c src/jeu/pioche.c src/jeu/tuiles.c src/jeu/joueur.c src/jeu/score.c -o $(TEST_MEEPLE)
	./$(TEST_MEEPLE)

test_affichage: $(TESTSRC_AFFICHAGE)
	$(cc) $(CFLAGS) $(TESTSRC_AFFICHAGE) src/jeu/plateau.c src/jeu/tuiles.c src/jeu/joueur.c src/jeu/meeple.c src/jeu/score.c src/jeu/pioche.c src/jeu/affichage.c src/autres/config.c -o $(TEST_AFFICHAGE)
	./$(TEST_AFFICHAGE)

req:
	sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev

cli: $(NAME)
	./$(NAME) -m 1 -p 2

sdl: $(NAME_SDL)
	./$(NAME_SDL) -m 2 -p 2

clean:
	rm -rf exec $(TESTOBJ) $(NAME) $(NAME_SDL) $(TEST) $(TEST_PLATEAU) $(TEST_SCORE) $(TEST_PIOCHE) $(TEST_JOUEUR) $(TEST_TUILE) $(TEST_MEEPLE) $(TEST_AFFICHAGE)

	
