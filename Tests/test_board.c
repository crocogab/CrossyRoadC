#include "board.h"
#include <stdio.h>
#include <assert.h>
#include "macro.h"

int main(void) {
    //test des fonctions sur grid ground
    
    Ground **grid_ground = grid_ground_make();
    assert(grid_ground[0]->velocity == 0);
    //printf("la velocité de la première ligne est %f \n",grid_ground[0]->velocity );
    grid_ground[0]->velocity  = 4.;
    assert(grid_ground[0]->velocity == 4);
    //printf("la velocité de la première ligne est %f \n",grid_ground[0]->velocity );
    grid_ground_free(grid_ground); 

    // test mémoire sur les mouvements de board
    Board *b = board_make();
    ground_move(b, DOWN);
    ground_move(b, UP);
    board_free(b);

    printf("La librairie board passe tout les tests\n");
    return 0;
}

