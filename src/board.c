#include "board.h"
#include "ground.h"
#include "obstacle.h"
#include "stdlib.h"

/**
 * Crée un nouveau plateau.
 * 
 * @return Un pointeur vers le plateau créé
 */
Board* board_make() {
    Board* b = (Board*)malloc(sizeof(Board));
    if (b == NULL) {
        return NULL; // En cas d'erreur d'allocation
    }
    b->grid_ground = grid_ground_make(); // Fonction qui initialise le sol
    b->grid_obstacle = grid_obstacle_make(); // Initialiser à NULL ou appeler une fonction pour créer les obstacles
    b->player = NULL; // Initialiser à NULL ou ajouter un joueur si nécessaire
    return b;
}

/**
 * Libère la mémoire associée à un plateau.
 * 
 * @param[in] b Le plateau à libérer
 */
void board_free(Board* b) {
    if (b != NULL) {
        if (b->grid_ground != NULL) {
            grid_ground_free(b->grid_ground); // Fonction qui libère le sol
        }
        if (b->grid_obstacle != NULL) {
            grid_obstacle_free(b->grid_obstacle); // Libérer les obstacles si nécessaire
        }
        if (b->player != NULL) {
            player_free(b->player);
        }
        free(b);
    }
}

/**
 * Récupère la grille de terrain associée au plateau.
 * 
 * @param[in] b Le plateau concerné
 * @return La grille de terrain
 */
Ground* board_get_ground(Board* b) {
    return b != NULL ? b->grid_ground : NULL;
}

/**
 * Change la grille de terrain associée au plateau.
 * 
 * @param[in] b Le plateau concerné
 * @param[in] ground La nouvelle grille de terrain
 */
void board_set_ground(Board* b, Ground* ground) {
    if (b != NULL) {
        if (b->grid_ground != NULL) {
            grid_ground_free(b->grid_ground);
        }
        b->grid_ground = ground;
    }
}

/**
 * Récupère le joueur associé au plateau.
 * 
 * @param[in] b Le plateau concerné
 * @return Le joueur
 */
Player* board_get_player(Board* b) {
    return b != NULL ? b->player : NULL;
}

/**
 * Change le joueur associé au plateau.
 * 
 * @param[in] b Le plateau concerné
 * @param[in] player Le nouveau joueur
 */
void board_set_player(Board* b, Player* player) {
    if (b != NULL) {
        if (b->player != NULL) {
            player_free(b->player);
        }
        b->player = player;
    }
}

/**
 * Fait avancer le temps dans la grille (fait bouger les obstacles)
 * 
 * @param[in] b Le plateau à mettre à jour
 * @param[in] delta_t le temps qui s'est écoulé
 */
/*parler ordre des arguments, update puis on voit l'erreur ou autre sens ?*/
void board_update(Board* b, float delta_t) {
    if (b == NULL || b->player == NULL) {
        return; // Si le plateau ou le joueur est NULL, on ne fait rien
    }
    Obstacle **grid = b->grid_obstacle;
    bool stop;

    for (int lig = 0; lig < MAP_LEN_GUI; lig++) {
        stop = false;
        for (int col = 0; col < MAP_WIDTH_GUI; col++) {
            
            if (grid[lig][col].type != TYPE_VIDE && !stop) {
                stop = true;
                obstacle_update(grid[lig]+col, delta_t);
            } else {    
                stop = false;
            }
        }
    }
}

/**
 * Vérifie s'il y a une collision entre le joueur et un obstacle.
 * 
 * @param[in] player Le joueur à vérifier
 * @param[in] direction La direction dans laquelle le joueur se déplace
 * @param[in] grid_obstacle La grille des obstacles sur le plateau
 * @return true s'il y a une collision, false sinon
 */
/*comment check la collision, changement puis regarde si pb ou détection après, comment marche la grille*/
bool check_collision(Player* player, int direction, Obstacle** grid_obstacle) {
    if (player == NULL || grid_obstacle == NULL) {
        return false; // Pas de collision si le joueur ou la grille d'obstacles est NULL
    }

    // Calculer la position future du joueur en fonction de la direction
    int future_position = player->h_position + direction;

    return false; // Pas de collision
}
