#include "game.h"
#include "board.h"

Game game_make(){
    // crée la game 
    Game g;
    g.score=0;
    g.status=0; // à changer quand on aura les macros
    return g;
}

void kill_player(Game g){
    g.status = 0;
    g.board->player->alive = false;
}

Game game_start(Game g){
    Board * b= board_make();
    g.board=b;
    g.score=0;
    g.status=1; // à changer avec les macros
}
