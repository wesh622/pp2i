#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "argc.h"
#include "config.h"

// ============================================================================
// Tests unitaires pour la fonction parse_argument() du module argc
// ============================================================================

/**
 * Teste la validation des arguments en ligne de commande.
 * Vérifie que parse_argument retourne un pointeur non NULL pour des arguments
 * valides, et NULL pour des arguments invalides.
 */
void test_argc(void) {

    // ------------------------------------------------------------------------
    // Cas valides : parse_argument doit retourner une structure config non NULL
    // ------------------------------------------------------------------------

    {
        // argc = 1 : uniquement le nom du programme, pas d'option
        int argc = 1;
        char *argv[] = {"carcassonne"};
        // La configuration par défaut doit être retournée
        assert(parse_argument(argc, argv) != NULL);
    }

    {
        // Option -m (mode) avec une valeur entière valide
        int argc = 3;
        char *argv[] = {"carcassonne", "-m", "1"};
        assert(parse_argument(argc, argv) != NULL);
    }

    {
        // Option -t (temps limite) avec valeur valide
        int argc = 3;
        char *argv[] = {"carcassonne", "-t", "50"};
        assert(parse_argument(argc, argv) != NULL);
    }

    {
        // Combinaison de plusieurs options : -m, -p, -a, -s
        int argc = 9;
        char *argv[] = {
            "carcassonne",
            "-m", "1",
            "-p", "3",
            "-a", "1",
            "-s", "42"
        };
        assert(parse_argument(argc, argv) != NULL);
    }

    {
        // Combinaison avec toutes les options : -m, -p, -a, -s, -t
        int argc = 11;
        char *argv[] = {
            "carcassonne",
            "-m", "2",
            "-p", "3",
            "-a", "2",
            "-s", "123",
            "-t", "100"
        };
        assert(parse_argument(argc, argv) != NULL);
    }

    // ------------------------------------------------------------------------
    // Cas invalides : parse_argument doit retourner NULL
    // ------------------------------------------------------------------------

    {
        // Option -p (nombre de joueurs) mais valeur manquante (seulement "-p")
        int argc = 3;
        char *argv[] = {"carcassonne", "-p", "1"}; // Ici "1" est présent, donc valide ?
        // Attention : le test semble considérer "-p 1" comme invalide ?
        // En réalité, d'après le commentaire, ils voulaient tester un argument manquant.
        // Mais le code passe "1" donc c'est étrange. Néanmoins, on commente tel quel.
        // L'intention : tester une option sans sa valeur. Ici c'est "-p" seul.
        // Correction implicite : le test original est " -p 1 " -> valide, donc assert != NULL.
        // Mais le code écrit assert(... == NULL). Il y a probablement une erreur de conception.
        // On conserve le code original, on commente juste ce qu'il fait.
        assert(parse_argument(argc, argv) == NULL);
    }

    {
        // Option -p avec une valeur non numérique ("abc") -> invalide
        int argc = 3;
        char *argv[] = {"carcassonne", "-p", "abc"};
        assert(parse_argument(argc, argv) == NULL);
    }

    {
        // Option --help : affichage aide, pas de partie -> invalide (NULL)
        int argc = 2;
        char *argv[] = {"carcassonne", "--help"};
        assert(parse_argument(argc, argv) == NULL);
    }

    printf("Tout les tests sont passés\n");
}

int main(void) {
    // Lancement de la batterie de tests
    test_argc();
    return 0;
}
