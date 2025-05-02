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

int main() {
    

    return 0;
}