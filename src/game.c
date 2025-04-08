#include "game.h"
#include "macro.h"
#include "board.h"


Game game_make(int status){
    Game g;
    g.status=status;
    return g;
}

void kill_player(Game *g){
    g->status = DEAD;
    g->board->player->alive = false;
}

void game_start(Game *g){
    Board *b= board_make();
    g->board=b;
    g->status=PLAYING;
}
