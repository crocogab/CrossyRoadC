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
    snprintf(player_info, sizeof(player_info), "Position: (%.2f, %d) | Alive: %s | Score: %d",
            g->board->player->h_position, V_POS,
            g->board->player->alive ? "Yes" : "No",
            g->board->player->score);
    write_text(player_info, 5, 25, -1, 0, 2, white, renderer, font);

    // Deuxième zone : affichage des informations de la ligne suivante
    write_text("Next line : ", 5, 45, -1, 0, 2, white, renderer, font);
    char next_line_info[256];
    snprintf(next_line_info, sizeof(next_line_info), "Nb. Obsta.: %d | Type: %d | Special: %d | Velocity: %f",
            g->board->grid_ground[V_POS - 1]->nb_obstacles,
            g->board->grid_ground[V_POS - 1]->type,
            g->board->grid_ground[V_POS - 1]->special_attr,
            g->board->grid_ground[V_POS - 1]->velocity);
    write_text(next_line_info, 5, 65, -1, 0, 2, white, renderer, font);

}
