#include <stdlib.h>
#include <stdio.h>
#include "macro.h"
#include "player.h"


/**
 * Crée un pointeur vers un nouveau joueur initialisé avec les paramètres donnés.
 * 
 * Cette fonction alloue dynamiquement une structure Player sur le tas et
 * l'initialise avec les valeurs fournies. Cette structure représente le 
 * personnage contrôlé par le joueur dans le jeu.
 * 
 * @param grid_cell_size La taille (largeur) d'une cellule de la grille, utilisée pour le joueur.
 * @param h_position La position horizontale initiale du joueur sur la grille.
 * @param direction La direction initiale du joueur (par exemple, 1 pour droite, -1 pour gauche).
 * @param alive L'état initial de vie du joueur (true si vivant, false sinon).
 * @param isJumping L'état initial de saut du joueur (true si en train de sauter, false sinon).
 * @param score Le score initial du joueur.
 * 
 * @return Player* Un pointeur vers la structure Player nouvellement créée et initialisée. 
 *         Retourne NULL si l'allocation mémoire échoue. (Note: le code actuel ne gère pas l'échec de malloc).
 * 
 * @pre 0 <= h_position < fin_de_la_map (La position horizontale doit être dans les limites de la carte).
 */
Player *player_make(float grid_cell_size, float h_position, int direction, bool alive, bool isJumping, int score)
{
    Player *ans = malloc(sizeof(Player));
    ans->grid_cell_width = grid_cell_size;
    ans->h_position = h_position;
    ans->direction = direction;
    ans->previous_direction = direction;
    ans->alive = alive;
    ans->is_jumping = isJumping;
    ans->score = score;
    return ans;
}

/**
 * @brief Crée un nouveau personnage joueur initialisé pour le début de partie.
 *
 * Initialise un joueur avec les paramètres par défaut nécessaires
 * au commencement d'une partie (taille, position centrale, direction UP, etc.).
 *
 * @return Player* Un pointeur vers la structure Player nouvellement créée et initialisée.
 */
Player *player_start()
{
    return player_make(DEFAULT_CELL_SIZE, (float)(MAP_WIDTH/2), UP, true, false, 0);
}

/**
 * Libère la mémoire allouée pour une structure Player.
 *
 * @param player Pointeur vers la structure Player à libérer.
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
        player->previous_direction = LEFT;
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
        player->previous_direction = RIGHT;
        break;

    case UP:
        player->direction = UP;
        player->previous_direction = UP;
        break;

    case DOWN:
        player->direction = DOWN;
        player->previous_direction = DOWN;
        break;
    
    default:
        printf("Invalid input : direction invalide : %d\n", direction);
    }
}