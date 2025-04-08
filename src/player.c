#include <stdlib.h>
#include <stdio.h>
#include "macro.h"
#include "player.h"


/**
 * Crée un pointeur vers un nouveau joueur
 * initialisé avec les paramètres donnés.
 * Cette structure représente le personnage
 * contrôlé par le joueur
 * La structure est allouée sur le tas.
 * 
 * @author : Raphaël
 * @pre : 0 <= h_position < fin_de_la_map
 */
Player *player_make(float grid_cell_size, float h_position, int direction, bool alive, bool isJumping)
{
    Player *ans = malloc(sizeof(Player));
    ans->grid_cell_width = grid_cell_size;
    ans->h_position = h_position;
    ans->direction = direction;
    ans->alive = alive;
    ans->is_jumping = isJumping;
    return ans;
}

/**
 * Crée un nouveau personnage au joueur initialisé
 * avec les paramètres nécessaires au début de la partie.
 * 
 * @author : Raphaël
 */
Player *player_start()
{
    return player_make(DEFAULT_CELL_SIZE, (float)(MAP_WIDTH/2), UP, true, false);
}

/**
 * Libère la mémoire associée à l'entité player.
 * 
 * @param[in] player L'entité dont on veut libérer la mémoire
 * 
 * @author : Raphaël
 */
void player_free(Player *player)
{
    free(player);
}

/**
 * Change les attributs du personnage en fonction du mouvement.
 * On suppose que le mouvement est valide. Le comportement peut-
 * être partiellement indéfini.
 * 
 * @param[in] direction Direction du mouvement
 * @param[in] player l'entité concernée par le mouvement valide
 * 
 * @author : Raphaël
 */
void move_player(int direction, Player *player)
{
    switch (direction)
    {
    case LEFT:
        if (player->h_position - player->grid_cell_width < LEFT_MAP_X)
        {
            printf("Invalid move: direction %s\n", "left");
        }
        else
        {
            player->h_position -= player->grid_cell_width;
        }
        player->direction = LEFT;
        break;

    case RIGHT:
        if (player->h_position + player->grid_cell_width > RIGHT_MAP_X)
        {
            printf("Invalid move: direction %s\n", "right");
        }
        else
        {
            player->h_position += player->grid_cell_width;
        }
        player->direction = RIGHT;
        break;

    case UP:
        player->direction = UP;
        break;

    case DOWN:
        player->direction = DOWN;
        break;
    
    default:
        printf("Invalid input : direction invalide : %d\n", direction);
    }
}
