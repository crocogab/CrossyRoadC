#include "board.h"
#include "ground.h"
#include "obstacle.h"
#include <stdlib.h>
#include <math.h>

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
 * @param[in] b Le plateau de jeu
 * @return true s'il y a une collision, false sinon
 */
/*comment check la collision, changement puis regarde si pb ou détection après, comment marche la grille*/
bool check_collision(Board* b) {
    if (b == NULL|| b->grid_obstacle == NULL ){
        return; //Si le plateau et la grille d'obstacle est NULL, on ne fait rien
    }
    return(((b->grid_obstacle)[V_POS][(int)(b->player->h_position)].type)!=TYPE_VIDE);
    /* comment on gère la hitbox car position flotante*/
        
}

/**
 * Déplace le joueur en mettant à jour sa position horizontale.
 * 
 * @param[in] p Le joueur à déplacer
 * @param[in] direction La direction du mouvement ()
 * @param[in] delta_t Le temps écoulé depuis la dernière mise à jour
 */
void player_move(Player* p, int direction, float delta_t) {
    if (p == NULL) {
        return; // Si le joueur est NULL, on ne fait rien
    }
    if ((direction == 1)||(direction == -1)){
        p->h_position += direction * delta_t; // Met à jour la position du joueur
    }    
}

/**
 * Fait avancer le sol d'une case dans la direction donnée.
 * 
 * @param[in] b Le plateau contenant la grille de sol
 * @param[in] direction La direction dans laquelle le sol doit avancer (1 pour droite, -1 pour gauche)
 */
void ground_move(Board* b, int direction) {
    if (b == NULL || b->grid_ground == NULL) {
        return; // Si le plateau ou la grille du sol est NULL, on ne fait rien
    }

    // Décale les cases de la grille de sol dans la direction donnée
    if (direction == 2) { // Déplacement vers le haut
        for(int i = MAP_LEN_GUI-1 ; i>0 ; i++){
            b->grid_ground[i] = b->grid_ground[i-1];
            //Que fait on du b->ground[0] ?
        }

    } else if (direction == -2) { // Déplacement vers le bas
        for(int i = 0 ; i<MAP_LEN_GUI-1; i++){
            b->grid_ground[i] = b->grid_ground[i+1];
            //Que fait on du b->ground[MAP_LEN_GUI-1] ?
        } 
    }
}