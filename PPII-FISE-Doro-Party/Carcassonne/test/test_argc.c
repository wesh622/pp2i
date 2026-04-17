#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "argc.h"
#include "config.h"

//quelques tests unitaires pour test.c

void test_argc(){

    {
        int argc = 1;
        char *argv[] = {"carcassonne"};
        assert(parse_argument(argc,argv) == true);
    }

    {
        int argc = 3;
        char *argv[] = {"carcassonne","-m","1"};
        assert(parse_argument(argc,argv) == true);
    }

    {
        int argc = 3;
        char *argv[] = {"carcassonne","-t","50"};
        assert(parse_argument(argc,argv) == true);
    }

    {
        int argc = 9;
        char *argv[] = {
        "carcassonne",
        "-m","1",
        "-p","3",
        "-a","1",
        "-s","42"
        };
        assert(parse_argument(argc,argv) == true);
    }

    {
        int argc = 11;
        char *argv[] = {
        "carcassonne",
        "-m","2",
        "-p","4",
        "-a","2",
        "-s","123",
        "-t","100"
        };
        assert(parse_argument(argc,argv) == true);
    }

    {
        int argc = 3;
        char *argv[] = {"carcassonne","-p","1"};
        assert(parse_argument(argc,argv) == true);
    }

    {
        int argc = 3;
        char *argv[] = {"carcassonne","-p","abc"};
        assert(parse_argument(argc,argv) == false);
    }

    {
        int argc = 2;
        char *argv[] = {"carcassonne","--help"};
        assert(parse_argument(argc,argv) == true);
    }

    printf("Tout les tests sont passés");
}
