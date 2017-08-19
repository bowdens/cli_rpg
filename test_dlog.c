#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main(int argc, char **argv){
    if(argc != 2){
        fprintf(stderr, "USAGE: %s FILE\n",argv[0]);
        return EXIT_FAILURE;
    }
    read_dlog(argv[1]);
    return EXIT_SUCCESS;
}
