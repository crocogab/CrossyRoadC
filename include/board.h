#ifndef BOARD_H
#define BOARD_H

#include "obstacle.h"
#include "player.h"
#include "ground.h"


typedef struct _Board {
    Player *player; // pointeur vers le joueur
    Ground **grid_ground;  
} Board;

Board* board_make(void);
void board_free(Board* b);
void board_set_ground(Board* b, Ground** ground);
void board_set_player(Board* b, Player* player);
void board_update(Board* b, float delta_t);
int check_future_collision(Board *, int direction);
void player_move (Player* p, int direction, float delta_t);
void ground_move (Board* b, int direction);

Ground **grid_ground_make(void);
void grid_ground_free(Ground **);

char **grid_tui_make(Board *);

#endif

