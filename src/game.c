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
void game_start(Game *g, Sprite_sheet *sprite_sheet){
    Board *b= board_make(sprite_sheet);
    g->board=b;
    g->status=PLAYING;
}

void game_debug(Game *g, TTF_Font *font, SDL_Renderer *renderer)
{
    SDL_Color white = {255, 255, 255, SDL_ALPHA_OPAQUE};
    // Première zone : affichage des informations joueurs
    write_text("Player : ", 5, 5, -1, 0, 2, white, renderer, font);
    char player_info[256];
    snprintf(player_info, sizeof(player_info), "Position: (%d, %d) | Alive: %s | Score: %d",
            g->board->player->h_position, V_POS,
            g->board->player->alive ? "Yes" : "No",
            g->board->player->score);
    write_text(player_info, 5, 25, -1, 0, 2, white, renderer, font);
}
