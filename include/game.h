#ifndef GAME_H
#define GAME_H

#include "board.h"


typedef struct _Game {
    Board *board; // * ou pas * ?
    int score;
    int status; //macro
} Game;

Game game_make(int status,int score,Board * b);

void kill_player(Game g);

void game_change_status(Game g,int status);

int game_get_status(Game g);

void game_change_score(Game g,int score);

int game_get_score(Game g);

Board* game_get_board(Game g);

void game_change_board(Game g, Board * b);

#endif
