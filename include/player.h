#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

#define DEFAULT_CELL_SIZE 10.0
#define LEFT_MAP_X 0.0
#define RIGHT_MAP_X 100.0

typedef struct _Player {
    // Skin *skin;
    bool alive;
    int direction; //macro
    float h_position;
    bool is_jumping;
    float grid_cell_width;

} Player;

Player *player_make(float grid_cell_size, float h_position, int direction, bool alive, bool isJumping);

Player *player_start();

void player_free(Player *player);

void move_player(int direction, Player *player);

#endif
