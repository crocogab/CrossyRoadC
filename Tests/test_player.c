#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "player.h"
#include "macro.h"

#define GRID_CELL_SIZE 10.0

int main() {

    Player *testing_player = player_make(GRID_CELL_SIZE, 5.0, UP, true, false, 0);
    assert(testing_player != NULL); //Le player n'est pas bien initialisé
    assert(testing_player->alive == true); //Le player n'est pas bien initialisé

    move_player(LEFT, testing_player);
    assert(testing_player->h_position == 5.0); //Le player a bougé hors de la map
    assert(testing_player->direction == LEFT); //Le player n'a pas la bonne direction

    move_player(RIGHT, testing_player);
    assert(testing_player->h_position == 5.0 + GRID_CELL_SIZE); //Le joueur n'a pas la bonne position latérale
    assert(testing_player->direction == RIGHT); //Le player n'a pas la bonne direction

    move_player(UP, testing_player);
    assert(testing_player->direction == UP); //Le player n'a pas la bonne direction

    player_free(testing_player);

    Player *testing_player_start = player_start();
    assert(testing_player_start != NULL); //Le player n'est pas bien initialisé
    assert(testing_player_start->direction == UP); //Le player n'est pas bien initialisé

    player_free(testing_player_start);

    printf("La librairie player passe tout les tests\n");

    return 0;
}
