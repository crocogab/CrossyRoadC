#include <stdlib.h>
#include <stdio.h>
#include "macro.h"
#include "player.h"
#include "ground.h"
#include "obstacle.h"


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
 *         Retourne NULL si l'allocation mémoire échoue.
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
Player *player_start(void)
{
    return player_make(DEFAULT_CELL_SIZE, (float)(MAP_WIDTH*DEFAULT_CELL_SIZE/2)+(DEFAULT_CELL_SIZE/4), UP, true, false, 0);
}

void player_reset(Player *player)
{
    player->h_position = (float)(MAP_WIDTH*DEFAULT_CELL_SIZE/2)+(DEFAULT_CELL_SIZE/4);
    player->direction = UP;
    player->previous_direction = UP;
    player->alive = true;
    player->is_jumping = false;
    player->score = 0;
}

/**
 * Libère la mémoire allouée pour une structure Player.
 *
 * @param player Pointeur vers la structure Player à libérer.
 * 
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
 * @param[in] next_ground le sol sur lequel le joueur se déplace
 * 
 */
void move_player(int direction, Player *player, Ground *next_ground, Ground *ground_actu)
{
    int current_cell_x;
    int new_cell_x;
    int wrapped_cell_x;
    float target_position = player->h_position; // Position par défaut si aucun mouvement
    bool found_safe_spot = false;
    
    switch (direction)
    {
    case LEFT:
        if (player->h_position - player->grid_cell_width < LEFT_MAP_X)
        {
            printf("Invalid move: direction %s\n", "left");
            return;
        }
        
        current_cell_x = player->h_position / DEFAULT_CELL_SIZE;
        new_cell_x = current_cell_x - 1;
        wrapped_cell_x = (new_cell_x % MAP_WIDTH + MAP_WIDTH) % MAP_WIDTH;
        target_position = wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
        
        break;
        
    case RIGHT:
        if (player->h_position + player->grid_cell_width > RIGHT_MAP_X)
        {
            printf("Invalid move: direction %s\n", "right");
            return;
        }
        
        current_cell_x = player->h_position / DEFAULT_CELL_SIZE;
        new_cell_x = current_cell_x + 1;
        wrapped_cell_x = (new_cell_x % MAP_WIDTH + MAP_WIDTH) % MAP_WIDTH;
        target_position = wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
        
        break;
        
    case UP:
        current_cell_x = player->h_position / DEFAULT_CELL_SIZE;
        wrapped_cell_x = (current_cell_x % MAP_WIDTH + MAP_WIDTH) % MAP_WIDTH;
        target_position = wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
        
        break;
        
    case DOWN:
        current_cell_x = player->h_position / DEFAULT_CELL_SIZE;
        wrapped_cell_x = (current_cell_x % MAP_WIDTH + MAP_WIDTH) % MAP_WIDTH;
        target_position = wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
        
        break;
        
    default:
        printf("Invalid input : direction invalide : %d\n", direction);
        return;
    }
    
    // traitement special pour eau
    if (next_ground && (next_ground->type == GROUND_WATER_LILY || next_ground->type == GROUND_WATER_LOG))
    {
        
        found_safe_spot = false;
        float best_position = target_position;
        float min_distance = 999999.0f;
        
        // trouve le plus proche
        for (int i = 0; i < next_ground->nb_obstacles; i++)
        {
            float obstacle_center = next_ground->obstacles[i]->h_position + (next_ground->obstacles[i]->length / 2);
            float distance = fabsf(obstacle_center - target_position);
            
            if (obstacle_is_colliding(next_ground->obstacles[i], target_position) && distance < min_distance)
            {
                min_distance = distance;
                best_position = target_position;
                found_safe_spot = true;
            }
        }
        
        if (found_safe_spot)
        {
            
            player->h_position = best_position;
        }
        else
        {
            
            printf("Player fell in water!\n");
            player->h_position = target_position;
            
        }
    }
    else
    {
        
        player->h_position = target_position;
    }
    
    
    player->direction = direction;
    player->previous_direction = direction;
    player->is_jumping = 1;
}

/**
 * Dessine le personnage sur la carte avec animation
 * 
 * @param p Le joueur à dessiner
 * @param anim_time Le temps d'animation
 * @param anim_x L'animation sur l'axe horizontal
 * @param anim_z L'animation sur l'axe vertical
 * @param sprite_sheet La sprite sheet
 * @param renderer Le renderer
 * @param cam La caméra
 * @param display Les informations d'affichage
 * @param debug_kit Le kit de débug
 * 
 * @return 0 si l'animation est finie, 1 sinon
 */
int draw_chicken_anim(Player *p, float anim_time, Animation anim_x, Animation anim_z, Sprite_sheet *sprite_sheet, SDL_Renderer *renderer, Camera cam, Display_informations display, debugKit *debug_kit)
{
    int sprite_index = 0;
    // On calcule le deplacement induit par l'animation en cours
    float delta_anim_x = 0.0;
    float delta_anim_z = 0.0;
    if (anim_time > 0.0 && anim_time <= anim_x.duration)
    {
        delta_anim_x = animation_calc(anim_x, anim_time);
        delta_anim_z = animation_calc(anim_z, anim_time);
    }

    switch (p->previous_direction)
    {
    case LEFT:
        sprite_index = 2;
        draw_sprite_from_grid(p->h_position - delta_anim_x, V_POS, delta_anim_z, CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
        break;
    case RIGHT:
        sprite_index = 0;
        draw_sprite_from_grid(p->h_position + delta_anim_x, V_POS, delta_anim_z, CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
        break;
    case UP:
        sprite_index = 3;
        draw_sprite_from_grid(p->h_position, (float)V_POS, delta_anim_z, CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
        break;
    case DOWN:
        sprite_index = 1;
        draw_sprite_from_grid(p->h_position, (float)V_POS, delta_anim_z, CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
        break;
    
    default:
        break;
    }

    // On renvoie 0 si l'animation est finie
    if (anim_time > anim_x.duration || anim_time == 0)
    {
        return 0;
    }
    return 1;
}
