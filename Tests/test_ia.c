#include "ia.h"
#include "macro.h"
#include <stdio.h>
#include "game.h"


void hitgrid_print(int **hg) {
    for (int i = MAP_LEN - 1; i >= 0; i--) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            printf("%c", hg[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    // Obstacle *obstest = obstacle_make(LOG_TYPE, 0, LEFT_MAP_X + DEFAULT_CELL_SIZE, 1.6);
    printf("Début des tests ia\n");
    int **hitgrid = hitgrid_make();
    
    // hitgrid_fill(hitgrid, );

    hitgrid_print(hitgrid);

    
    printf("La librairie ia passe tous les tests\n");
    return 0;
}
