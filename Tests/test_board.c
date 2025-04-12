#include "board.h"
#include <stdio.h>
#include <assert.h>
#include "macro.h"

void print_grid(Board *b) {
    char **g = grid_tui_make(b);
    printf("DEBUT DE LA GRILLE\n");
    for (int i = 0; i < MAP_LEN; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            printf("%c", g[i][j]);
        }
        printf("\n");
    }
    printf("FIN DE LA GRILLE\n");
    grid_tui_free(g);
}

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
    b->player = player_start();
    print_grid(b);
    grid_ground_starter_set(b);
    print_grid(b);
    ground_move(b, DOWN);
    print_grid(b);
    ground_move(b, UP);
    print_grid(b);
    printf("Obstacles de la ligne 0 : %d\n", b->grid_ground[0]->nb_obstacles);
    for (int i = 0; i < b->grid_ground[0]->nb_obstacles; i++)
    {
        printf("Obstacle %d : %f\n", i, b->grid_ground[0]->obstacles[i]->h_position);
    }
    board_free(b);

    printf("La librairie board passe tout les tests\n");
    return 0;
}

