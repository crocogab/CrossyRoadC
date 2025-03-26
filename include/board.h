#ifndef BOARD_H
#define BOARD_H

#include "obstacle.h"
#include "player.h"
#include "ground.h"

#define MAP_WIDTH_GUI 19 
#define MAP_WIDTH_TXT 9
#define MAP_LEN_GUI 25 // cela dépend de comment en compte (certaines ne sont pas entières)  
#define MAP_LEN_TXT 13
#define V_POS 15 // position du joueur sur la board



typedef struct _Board {
    Obstacle **grid_obstacle;
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
bool check_collision(Board* b);
void player_move (Player* p, int direction, float delta_t);
void ground_move (Board* b, int direction);

#endif

