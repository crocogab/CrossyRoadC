#include "game.h"
#include "macro.h"
#include "board.h"


Game game_make(int status,int score,Board * b){
    Game g;
    g.score=score;
    g.status=status;
    return g;
}

void kill_player(Game g){
    g.status = DEAD;
    g.board->player->alive = false;
}

Game game_start(Game g){
    Board * b= board_make();
    g.board=b;
    g.score=0;
    g.status=PLAYING;
}
