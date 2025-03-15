#ifndef GAME_H
#define GAME_H

#include "board.h"


typedef struct _Game {
    Board *board; // * ou pas * ?
    int score;
    int status; //macro
} Game;

#endif