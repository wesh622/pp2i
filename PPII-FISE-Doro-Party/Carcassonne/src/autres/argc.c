#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "argc.h"
#include "config.h"

//fonction qui prend les parametres en entrée et verifie qu'ils sont correct

config* parse_argument(int argc,char** argv){
    int mode = 0;
    int nb_joueur = 2;
    int seed = rand();
    int ai = 0;
    int max_turns = -1; // -1 = infini oui oui je fais ce que je veux
    bool deja_vu[] = {false,false,false,false,false};
    if(argc == 2 && (strcmp(argv[1],"--help") == 0)){
        printf(
        "Usage: carcassonne [options]\n"
        "\n"
        "Options:\n"
        "  -m --mode MODE        1--> cli ou 2--> sdl\n"
        "  -p --players N        nombre de joueurs (2-5)\n"
        "  -a --ai N             nombre d'IA\n"
        "  -s --seed N           graine de la partie\n"
        "  -t --max-turns N      limite de tours\n"
        "  --help                affiche cette aide\n"
        );
        return NULL;
    }
    if((argc - 1) % 2 != 0){
        printf("Erreur\n");
        return NULL;
    }
    else{
        for(int i =0;i<argc/2;i++){
            bool a = false;
            char *end;
            if(strcmp(argv[2*i+1],"--mode") == 0 || strcmp(argv[2*i+1],"-m") == 0){
                if(deja_vu[0]){
                    printf("Erreur\n");
                    return NULL;
                }
                deja_vu[0] = true;
                long x = strtol(argv[2*(i+1)], &end, 10);
                assert(*end == '\0');
                int mode_prime = atoi(argv[2*(i+1)]);
                if(mode_prime >2 || mode_prime<0){
                    printf("Erreur\n");
                    return NULL;
                }
                mode = mode_prime;
                a = true;
                end = NULL;
            }
            else if(strcmp(argv[2*i+1],"--players") == 0 || strcmp(argv[2*i+1],"-p") == 0){
                if(deja_vu[1]){
                    printf("Erreur\n");
                    return NULL;
                }
                deja_vu[1] = true;
                long x = strtol(argv[2*(i+1)], &end, 10);
                assert(*end == '\0');
                int prime = atoi(argv[2*(i+1)]);
                if(prime >5 || prime<2){
                    printf("Erreur\n");
                    return NULL;
                }
                nb_joueur = prime;
                a = true;
                end = NULL;
            }
            else if(strcmp(argv[2*i+1],"--seed") == 0 || strcmp(argv[2*i+1],"-s") == 0){
                if(deja_vu[2]){
                    printf("Erreur\n");
                    return NULL;
                }
                deja_vu[2] = true;
                long x = strtol(argv[2*(i+1)], &end, 10);
                assert(*end == '\0');
                int prime = atoi(argv[2*(i+1)]);
                if(prime<0){
                    printf("Erreur\n");
                    return NULL;
                }
                seed = prime;
                a = true;
                end = NULL;
            }
            else if(strcmp(argv[2*i+1],"--ai") == 0 || strcmp(argv[2*i+1],"-a") == 0){
                if(deja_vu[3]){
                    printf("Erreur\n");
                    return NULL;
                }
                deja_vu[3] = true;
                long x = strtol(argv[2*(i+1)], &end, 10);
                assert(*end == '\0');
                int prime = atoi(argv[2*(i+1)]);
                if(prime >5 || prime<0){
                    printf("Erreur\n");
                    return NULL;
                }
                ai = prime;
                a = true;
                end = NULL;
            }
            else if(strcmp(argv[2*i+1],"--max-turns") == 0 || strcmp(argv[2*i+1],"-t")== 0){
                if(deja_vu[4]){
                    printf("Erreur\n");
                    return NULL;
                }
                deja_vu[4] = true;
                long x = strtol(argv[2*(i+1)], &end, 10);
                assert(*end == '\0');
                int prime = atoi(argv[2*(i+1)]);
                if(prime<0){
                    printf("Erreur\n");
                    return NULL;
                }
                max_turns = prime;
                a = true;
                end = NULL;
            }
            if(!a){
                printf("Erreur\n");
                return NULL;
            }
        }
        if(ai>nb_joueur){
            printf("Erreur\n");
            return NULL;
        }
        return configuration(mode,nb_joueur,seed,ai,max_turns);
    }
    return NULL;
}
