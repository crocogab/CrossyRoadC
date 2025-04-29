#include "board.h"
#include "player.h"
#include <stdio.h>
#include <assert.h>
#include "macro.h"

int main(void) {
    // --- test grid_ground_make / grid_ground_free ---
    Ground **grid = grid_ground_make(NULL);
    assert(grid != NULL);
    // la première ligne est du grass à vitesse 0
    assert(grid[0]->velocity == 0.0f);
    // on peut modifier la vitesse
    grid[0]->velocity = 4.0f;
    assert(grid[0]->velocity == 4.0f);
    grid_ground_free(grid);

    // --- test board_make / board_free et mouvements ---
    Board *b = board_make(NULL);
    assert(b != NULL);

    // on crée et associe un joueur de départ
    Player *p = player_start();
    assert(p != NULL);
    board_set_player(b, p);
    assert(b->player == p);

    // on prépare la grille de démarrage
    //grid_ground_starter_set(b, NULL);
    // on vérifie qu'on a bien MAP_LEN lignes
    for (int i = 0; i < MAP_LEN; i++) {
        assert(b->grid_ground[i] != NULL);
    }


    // la ligne 0 existe toujours et ses champs sont valides
    Ground *line0 = b->grid_ground[0];
    assert(line0 != NULL);
    assert(line0->nb_obstacles >= 0);
    assert(line0->obstacles != NULL);

    board_free(b);

    printf("La librairie board passe tous les tests\n");
    return 0;
}