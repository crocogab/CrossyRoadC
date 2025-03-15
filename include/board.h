#ifndef BOARD_H
#define BOARD_H

#include "obstacle.h"
#include "player.h"
#include "ground.h"

typedef struct _Board {
    Obstacle **grid_obstacle;
    Player *player; // * ou pas * ?
    Ground *grid_ground;  
} Board;


#endif
