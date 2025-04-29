#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "player.h"
#include "macro.h"
#include "ground.h"

#define GRID_CELL_SIZE 1.0


int main() {

    // ajoute un ground factice pour passer en paramètre
    Ground *dummy_ground = ground_generate(GROUND_GRASS, 0, 0, 0, NULL);

    Player *testing_player = player_make(GRID_CELL_SIZE, 5.0, UP, true, false, 0);
    assert(testing_player != NULL); //Le player n'est pas bien initialisé
    assert(testing_player->alive == true); //Le player n'est pas bien initialisé

    move_player(LEFT, testing_player, dummy_ground);
    assert(testing_player->h_position == 5.0); //Le player a bougé 
    assert(testing_player->direction == LEFT); //Le player n'a pas la bonne direction

    move_player(RIGHT, testing_player, dummy_ground);
    assert(testing_player->direction == RIGHT); //Le player n'a pas la bonne direction
    

    move_player(UP, testing_player, dummy_ground);
    assert(testing_player->direction == UP); //Le player n'a pas la bonne direction

    player_free(testing_player);
    ground_free(dummy_ground);

    Player *testing_player_start = player_start();
    assert(testing_player_start != NULL); //Le player n'est pas bien initialisé
    assert(testing_player_start->direction == UP); //Le player n'est pas bien initialisé

    player_free(testing_player_start);

    printf("La librairie player passe tout les tests\n");

    return 0;
}
