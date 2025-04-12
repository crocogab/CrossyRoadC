#include "game.h"
#include "macro.h"
#include "board.h"

/**
 * Crée une nouvelle instance de jeu avec un statut donné.
 * @param status Le statut initial du jeu.
 * @return Une instance de Game initialisée avec le statut donné.
 */
Game game_make(int status){
    Game g;
    g.status=status;
    return g;
}

/**
 * Met fin à la vie du joueur dans le jeu.
 * @param g Un pointeur vers l'instance de Game à modifier.
 */
void kill_player(Game *g){
    g->status = DEAD;
    g->board->player->alive = false;
}

/**
 * Initialise le jeu en créant un plateau et en définissant le statut à PLAYING.
 * @param g Un pointeur vers l'instance de Game à initialiser.
 */
void game_start(Game *g){
    Board *b= board_make();
    g->board=b;
    g->status=PLAYING;
}
