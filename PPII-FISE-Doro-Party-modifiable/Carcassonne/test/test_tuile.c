#include <stdio.h>
#include "../include/tuiles.h"
#include <assert.h>

void test_tourner_vers_droite(void) {
	Tuiles t = {1, 10, 20, 30, 40, 5};
	tourner_vers_droite(&t);
	assert(t.a == 40);
	assert(t.b == 10);
	assert(t.c == 20);
	assert(t.d == 30);
	// Test d'une deuxième rotation
	tourner_vers_droite(&t);
	assert(t.a == 30);
	assert(t.b == 40);
	assert(t.c == 10);
	assert(t.d == 20);
	printf("test_tourner_vers_droite OK\n");
}

void test_tourner_vers_gauche(void) {
	Tuiles t = {2, 1, 2, 3, 4, 6};
	tourner_vers_gauche(&t);
	assert(t.a == 2);
	assert(t.b == 3);
	assert(t.c == 4);
	assert(t.d == 1);
	// Test d'une deuxième rotation
	tourner_vers_gauche(&t);
	assert(t.a == 3);
	assert(t.b == 4);
	assert(t.c == 1);
	assert(t.d == 2);
	printf("test_tourner_vers_gauche OK\n");
}

int main(void) {
	test_tourner_vers_droite();
	test_tourner_vers_gauche();
	printf("Tous les tests tuiles passes !\n");
	return 0;
}
