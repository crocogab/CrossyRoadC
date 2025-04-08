#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

typedef struct _Player {
    // Skin *skin;
    bool alive;
    int direction; //macro
    float h_position;
    bool is_jumping;
    float grid_cell_width;
    int score;

} Player;

Player *player_make(float grid_cell_size, float h_position, int direction, bool alive, bool isJumping, int score);

Player *player_start();

void player_free(Player *player);

void move_player(int direction, Player *player);

#endif
