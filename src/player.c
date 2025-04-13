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
    return player_make(DEFAULT_CELL_SIZE, (float)(MAP_WIDTH*DEFAULT_CELL_SIZE/2)+(DEFAULT_CELL_SIZE/4), UP, true, false, 0);
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

void move_player(int direction, Player *player,Ground * next_ground)
{
    int current_cell_x;
    int new_cell_x;
    int wrapped_cell_x;
    switch (direction)
    {
    case LEFT:
        if (player->h_position - player->grid_cell_width < LEFT_MAP_X)
        {
            printf("Invalid move: direction %s\n", "left");
        }
        else
        {
            // POUR ETRE TOUJOURS SUR LA BONNE PARTIE DE LA CASE -> EVITE d ETRE AU MILIEU DE 2 CASES
            current_cell_x = player->h_position / DEFAULT_CELL_SIZE;
            new_cell_x = current_cell_x - 1;
            
            wrapped_cell_x = (new_cell_x % MAP_WIDTH + MAP_WIDTH) % MAP_WIDTH;
            
            player->h_position = wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);

        }
        player->direction = LEFT;
        player->previous_direction = LEFT;
        player->is_jumping = 1;
        break;

    case RIGHT:
        if (player->h_position + player->grid_cell_width > RIGHT_MAP_X)
        {
            printf("Invalid move: direction %s\n", "right");
        }
        else
        {
            current_cell_x = player->h_position / DEFAULT_CELL_SIZE;
            new_cell_x = current_cell_x + 1;
            
            wrapped_cell_x = (new_cell_x % MAP_WIDTH + MAP_WIDTH) % MAP_WIDTH;
            
            player->h_position = wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
        }
        player->direction = RIGHT;
        player->previous_direction = RIGHT;
        player->is_jumping = 1;
        break;

    case UP:
        
        current_cell_x = player->h_position / DEFAULT_CELL_SIZE;
                    
        wrapped_cell_x = (current_cell_x % MAP_WIDTH + MAP_WIDTH) % MAP_WIDTH;

        if (next_ground->type==WATER_LILY_TYPE || next_ground->type==LOG_TYPE){
            for (int i=0;i<=next_ground->nb_obstacles;i++ ){
                if (obstacle_is_colliding(next_ground->obstacles[i],wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4))){
                    player->h_position = wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
                }
                if (obstacle_is_colliding(next_ground->obstacles[i],(wrapped_cell_x-1) * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4))){
                    player->h_position = (wrapped_cell_x-1) * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
                }else{
                    player->h_position = wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
                }

            }

        }else{
            player->h_position = wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
        }
            
        
        
        player->direction = UP;
        player->previous_direction = UP;
        player->is_jumping = 1;
        break;

    case DOWN:
        current_cell_x = player->h_position / DEFAULT_CELL_SIZE;
                    
        wrapped_cell_x = (current_cell_x % MAP_WIDTH + MAP_WIDTH) % MAP_WIDTH;
            
        if (next_ground->type==WATER_LILY_TYPE || next_ground->type==LOG_TYPE){
            for (int i=0;i<=next_ground->nb_obstacles;i++ ){
                if (obstacle_is_colliding(next_ground->obstacles[i],wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4))){
                    player->h_position = wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
                }
                if (obstacle_is_colliding(next_ground->obstacles[i],(wrapped_cell_x-1) * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4))){
                    player->h_position = (wrapped_cell_x-1) * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
                }else{
                    player->h_position = wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
                }

            }

        }else{
            player->h_position = wrapped_cell_x * DEFAULT_CELL_SIZE + (DEFAULT_CELL_SIZE/4);
        }
        player->direction = DOWN;
        player->previous_direction = DOWN;
        player->is_jumping = 1;
        break;
    
    default:
        printf("Invalid input : direction invalide : %d\n", direction);
    }
}

void draw_chicken(Player *p, Sprite_sheet *sprite_sheet, SDL_Renderer *renderer, Camera cam, Display_informations display, debugKit *debug_kit)
{
    int sprite_index = 0;
    switch (p->previous_direction)
    {
    case LEFT:
        sprite_index = 2;
        break;
    case RIGHT:
        sprite_index = 0;
        break;
    case UP:
        sprite_index = 3;
        break;
    case DOWN:
        sprite_index = 1;
        break;
    
    default:
        break;
    } 
    draw_sprite_from_grid(p->h_position, V_POS, 0, CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
}

int draw_chicken_anim(Player *p, float anim_time, Animation anim_x, Animation anim_z, Sprite_sheet *sprite_sheet, SDL_Renderer *renderer, Camera cam, Display_informations display, debugKit *debug_kit)
{
    int sprite_index = 0;
    printf("anim_calc_z : %f\n", animation_calc(anim_z, anim_time)/DEFAULT_CELL_SIZE);
    switch (p->previous_direction)
    {
    case LEFT:
        sprite_index = 2;
        if (anim_time == 0.0 || anim_time > anim_x.duration)
        {
            draw_sprite_from_grid(p->h_position, V_POS, 0, CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
        }
        else
        {
            draw_sprite_from_grid(p->h_position - animation_calc(anim_x, anim_time), V_POS, animation_calc(anim_z, anim_time), CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
        }
        break;
    case RIGHT:
        sprite_index = 0;
        if (anim_time == 0.0 || anim_time > anim_x.duration)
        {
            draw_sprite_from_grid(p->h_position, V_POS, 0, CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
        }
        else
        {
            draw_sprite_from_grid(p->h_position + animation_calc(anim_x, anim_time), V_POS, animation_calc(anim_z, anim_time), CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
        }
        break;
    case UP:
        sprite_index = 3;
        if (anim_time == 0.0 || anim_time > anim_x.duration)
        {
            draw_sprite_from_grid(p->h_position, (float)V_POS, 0, CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
        }
        else
        {
            draw_sprite_from_grid(p->h_position, (float)V_POS, animation_calc(anim_z, anim_time), CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
        }
        break;
    case DOWN:
        sprite_index = 1;
        if (anim_time == 0.0 || anim_time > anim_x.duration)
        {
            draw_sprite_from_grid(p->h_position, (float)V_POS, 0, CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
        }
        else
        {
            draw_sprite_from_grid(p->h_position, (float)V_POS, animation_calc(anim_z, anim_time), CHICKEN_ID, sprite_index, sprite_sheet, renderer, cam, display, debug_kit); 
        }
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
