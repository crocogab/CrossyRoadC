#ifndef BOARD_H
#define BOARD_H

#include "obstacle.h"
#include "player.h"
#include "ground.h"

#define MAP_WIDTH 19 //il n'y a que 9 cases atteignables au spawn mais il faut en afficher plus
#define MAP_LEN 13 // cela dépend de comment en compte (certaines ne sont pas entières)  



typedef struct _Board {
    Obstacle **grid_obstacle;
    Player *player; // * ou pas * ?
    Ground *grid_ground;  
} Board;


#endif
