#ifndef BOARD_H
#define BOARD_H

#include "obstacle.h"
#include "player.h"
#include "ground.h"


typedef struct _Board {
    int **grid_obstacle; // les obstacles sont caractérisés par leurs types
    Player *player; // pointeur vers le joueur
    Ground *grid_ground;  
} Board;

Board* board_make();
void board_free(Board* b);
Ground* board_get_ground(Board* b);
void board_set_ground(Board* b, Ground* ground);
Player* board_get_player(Board* b);
void board_set_player(Board* b, Player* player);
void board_update(Board* b, float delta_t);
int check_collision(Board* b);
void player_move (Player* p, int direction, float delta_t);
void ground_move (Board* b, int direction);

int **grid_obstacle_make(void);
Ground *grid_ground_make(void);
void grid_ground_free(Ground *);
void grid_obstacle_free(int **);

#endif

