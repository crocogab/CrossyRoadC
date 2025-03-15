#include "game.h"

Game game_make(int status,int score,Board * b){
    // crée la game 
    Game g;
    g.board=b;
    g.score=score;
    g.status=status;
    return g;
}

void game_change_status(Game g,int status){
    // change le status de la game pour le nouveau status
    g.status = status;
}

int game_get_status(Game g){
    // renvoie le status de la partie
    return g.status;
}

void game_change_score(Game g,int score){
    // change le score de la game pour le nouveau score
    g.score = score;
}

int game_get_score(Game g){
    // renvoie le score de la partie
    return g.score;
}

Board* game_get_board(Game g){
    // renvoie le board associé à la partie
    return g.board;
}

void game_change_board(Game g, Board * b){
    // change le board associé à la game
    g.board=b;
}
