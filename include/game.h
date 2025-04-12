#ifndef GAME_H
#define GAME_H

#include "board.h"


typedef struct _Game {
    Board *board;
    int status; //macro
} Game;

Game game_make(int status);
void kill_player(Game *g);
void game_start(Game *g);

#endif
