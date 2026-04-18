cc = clang
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -g -Iinclude

SRC = src/jeu/main.c \
      src/jeu/tuiles.c \
      src/jeu/plateau.c \
      src/jeu/pioche.c \
      src/jeu/joueur.c \
      src/autres/argc.c \
      src/autres/config.c
	  
OBJ = $(SRC:.c=.o)

TESTSRC = test/test_argc.c
TESTOBJ = $(TESTSRC:.c=.o)

NAME = carcassonne
TEST = test_argc

all: $(NAME)

$(NAME): $(OBJ)
	$(cc) $(CFLAGS) $(OBJ) -o $(NAME)

test: $(TESTOBJ) src/autres/argc.c src/autres/config.c
	$(cc) $(CFLAGS) $(TESTSRC) src/autres/argc.c src/autres/config.c -o $(TEST)
	./$(TEST)

clean:
	rm -f $(OBJ) $(TESTOBJ) $(NAME) $(TEST)