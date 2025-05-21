#include "game.h"
#include "macro.h"
#include "board.h"

/**
 * Crée une nouvelle instance de jeu avec un statut donné.
 * 
 * @param status Le statut initial du jeu.
 * 
 * @return Une instance de Game initialisée avec le statut donné.
 */
Game game_make(int status){
    Game g;
    g.status=status;
    return g;
}

/**
 * Met fin à la vie du joueur dans le jeu.
 * 
 * @param g Un pointeur vers l'instance de Game à modifier.
 * 
 */
void kill_player(Game *g){
    g->status = DEAD;
    g->board->player->alive = false;
}

/**
 * Initialise le jeu en créant un plateau et en définissant le statut à PLAYING.
 * 
 * @param g Un pointeur vers l'instance de Game à initialiser.
 * @param sprite_sheet La feuille de sprites.
 * 
 */
void game_start(Game *g, Sprite_sheet *sprite_sheet){
    Board *b= board_make(sprite_sheet);
    g->board=b;
    g->status=TO_LAUNCH;
}

/**
 * Affiche les informations de débogage du jeu
 * 
 * @param g Un pointeur vers l'instance de la partie
 * @param font La police de caractères utilisée pour afficher le texte
 * @param renderer Le renderer
 * @param cam La caméra
 * @param debug_kit Le kit de débogage
 * 
 */
void game_debug(Game *g, TTF_Font *font, SDL_Renderer *renderer, Camera cam, debugKit *debug_kit)
{
    SDL_Color white = {255, 255, 255, SDL_ALPHA_OPAQUE};
    Obstacle *o;
    write_text("Controls: F1 = god mode | F2 = lines | F3 = informations | F4 = sprites | F5 = slow | F6 = speed | F7= sprites_info", 5, 25, -1, 0, 2, white, renderer, font);
    write_text("Controls: F8 = Toggle IA | F9 = Hitboxes | Q = quit", 5, 45, -1, 0, 2, white, renderer, font);
    if (debug_kit->display_information)
    {
        // Première zone : affichage des informations joueurs
        write_text("Player : ", 5, 65, -1, 0, 2, white, renderer, font);
        char player_info[256];
        snprintf(player_info, sizeof(player_info), "Position: (%.2f, %d) | Alive: %s | Score: %d",
                g->board->player->h_position, V_POS,
                g->board->player->alive ? "Yes" : "No",
                g->board->player->score);
        write_text(player_info, 115, 65, -1, 0, 2, white, renderer, font);

        // Deuxième zone : affichage des informations de la ligne suivante
        write_text("Next line : ", 5, 85, -1, 0, 2, white, renderer, font);
        char next_line_info[256];
        snprintf(next_line_info, sizeof(next_line_info), "Nb. Obsta.: %d | Type: %d | Special: %f | Velocity: %f",
                g->board->grid_ground[V_POS - 1]->nb_obstacles,
                g->board->grid_ground[V_POS - 1]->type,
                g->board->grid_ground[V_POS - 1]->special_attr,
                g->board->grid_ground[V_POS - 1]->velocity);
        write_text(next_line_info, 155, 85, -1, 0, 2, white, renderer, font);
        
        // Troisième zone : affichage des informations des obstacles de la ligne suivante
        for (int i = 0; i < g->board->grid_ground[V_POS-1]->nb_obstacles; i++)
        {
            o = g->board->grid_ground[V_POS-1]->obstacles[i];
            if ((o->type == TREE_TYPE || o->type == ROCK_TYPE) && o->h_position > LEFT_MAP_X && o->h_position < RIGHT_MAP_X)
            {
                char obstacle_info[256];
                snprintf(obstacle_info, sizeof(obstacle_info), "Obstacle num. %d: Type: %d | Position: (%.2f, %d) | Length: %.2f",
                        i, o->type, o->h_position, V_POS-1, o->length);
                write_text(obstacle_info, 5, 105 + i*20, -1, 0, 2, white, renderer, font);
            }
            else if (!(o->type == TREE_TYPE || o->type == ROCK_TYPE))
            {
                char obstacle_info[256];
                snprintf(obstacle_info, sizeof(obstacle_info), "Obstacle num. %d: Type: %d | Position: (%.2f, %d) | Length: %.2f",
                        i, o->type, o->h_position, V_POS-1, o->length);
                write_text(obstacle_info, 5, 105 + i*20, -1, 0, 2, white, renderer, font);
            }
            
        }
    }

    if (debug_kit->display_information_sprites)
    {
        for (int j = 0; j < MAP_LEN; j++)
        {
            // Quatrième zone : on affiche les informations des sprites sous les sprites
            for (int i = 0; i < g->board->grid_ground[j]->nb_obstacles; i++)
            {
                o = g->board->grid_ground[j]->obstacles[i];
                if ((o->type == TREE_TYPE || o->type == ROCK_TYPE) && o->h_position > LEFT_MAP_X && o->h_position < RIGHT_MAP_X)
                {
                    char obstacle_info[256];
                    snprintf(obstacle_info, sizeof(obstacle_info), "Pos.: (%.2f, %d) | Len: %.2f",
                            o->h_position, j, o->length);
                    // On calcule les points où afficher le texte
                    Point2d p = d3_to_2d(o->h_position, (j)*DEFAULT_CELL_SIZE, -10, cam);
                    write_text(obstacle_info, p.x, p.y, -1, 0, 2, white, renderer, font);
                }
                else if (!(o->type == TREE_TYPE || o->type == ROCK_TYPE))
                {
                    char obstacle_info[256];
                    snprintf(obstacle_info, sizeof(obstacle_info), "Pos.: (%.2f, %d) | Len: %.2f",
                            o->h_position, j, o->length);
                    // On calcule les points où afficher le texte
                    Point2d p = d3_to_2d(o->h_position, (j)*DEFAULT_CELL_SIZE, -10, cam);
                    write_text(obstacle_info, p.x, p.y, -1, 0, 2, white, renderer, font);
                }
            }
        }
    }
}

/**
 * Réinitialise la partie, libère la mémoire des board et player,
 * les objets de la partie sont remis à zéro
 * @param g la partie a réinitialiser
 * 
 */
void game_reset(Game *g, Sprite_sheet *sprite_sheet)
{
    if (g->board != NULL)
    {
        board_free(g->board);
        g->board = NULL;
    }
    g->status = TO_LAUNCH;
    g->board = board_make(sprite_sheet);
    grid_ground_starter_set(g->board, sprite_sheet);
    board_set_player(g->board, player_start());
}
