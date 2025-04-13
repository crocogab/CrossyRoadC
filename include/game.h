#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "gui.h"
#include "ttf.h"


typedef struct _Game {
    Board *board;
    int status; //macro
} Game;

Game game_make(int status);
void kill_player(Game *g);
void game_start(Game *g, Sprite_sheet *sprite_sheet);
void game_debug(Game *g, TTF_Font *font, SDL_Renderer *renderer, Camera cam, debugKit *debug_kit);

#endif
