#include "board.h"
#include "ground.h"
#include "obstacle.h"
#include "random_custom.h"
#include <stdlib.h>
#include <math.h>
#include "macro.h"

/**
 * Crée un nouveau plateau.
 * 
 * @return Un pointeur vers le plateau créé
 */
Board* board_make(void) {
    Board* b = (Board*)malloc(sizeof(Board));
    if (b == NULL) {
        return NULL; // En cas d'erreur d'allocation
    }
    b->grid_ground = grid_ground_make(); // Fonction qui initialise le sol
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
        if (b->player != NULL) {
            player_free(b->player);
        }
        free(b);
    }
}


/**
 * Change la grille de terrain associée au plateau.
 * 
 * @param[in] b Le plateau concerné
 * @param[in] ground La nouvelle grille de terrain
 */
void board_set_ground(Board* b, Ground** ground) {
    if (b != NULL) {
        if (b->grid_ground != NULL) {
            grid_ground_free(b->grid_ground);
        }
        b->grid_ground = ground;
    }
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
    
    Ground *ground;
    Obstacle *obst;

    for (int lig = 0; lig < MAP_LEN; lig++) {
        ground = b->grid_ground[lig];

        for (int i = 0; i < ground->nb_obstacles; i++) {
            // update des obstacles
            obst = ground->obstacles[i]; // obst ou *obst ?????
            obstacle_update(obst, delta_t, ground->velocity);
        }
    }
}






/**
 * Vérifie s'il y a collision entre le joueur et un obstacle si le coup donné est joué.
 * 
 * @param[in] b un plateau de jeu
 * @param[in] direction une direction de mouvement, def par une macro
 * 
 * @return COLLIDE_NONE | COLLIDE_DEADLY | COLLIDE_HARMLESS
 * 
 * tomber dans l'eau renvoie COLLIDE_DEADLY et marcher sur un rondin COLLIDE_NONE
 */
int check_future_collision(Board *b, int direction) {
    if (b == NULL) {
        return COLLIDE_ERROR;
    }

    // on change virtuellement la ligne et la position du joueur en fonction du coup donné
    int lig = V_POS;
    float h = b->player->h_position;
    switch (direction) {
        case UP:
            lig++; 
            break;
        case DOWN: 
            lig--;
            break;
        case RIGHT:
            h = h + 1;
            break;
        case LEFT: 
            h = h - 1;
            break;
    }
    Ground *g = b->grid_ground[lig];
    Couple hb;
    
    if (g->type == GROUND_GRASS) {
        for (int i = 0; i<g->nb_obstacles; i++) {
            hb = obstacle_hitbox(g->obstacles[i]);
            if (hb.a <= i && i <= hb.b) {
                return COLLIDE_HARMLESS;
            }
            return COLLIDE_NONE;
        }
    }
    else if (g->type == GROUND_ROAD_CAR || g->type == GROUND_ROAD_TRUCKS || g->type == GROUND_TRAIN) {
        for (int i = 0; i<g->nb_obstacles; i++) {
            hb = obstacle_hitbox(g->obstacles[i]);
            if (hb.a <= i && i <= hb.b) {
                return COLLIDE_DEADLY;
            }
            return COLLIDE_NONE;
        }      
    }
    else if (g->type == GROUND_WATER_LOGS || g->type == GROUND_WATER_LILY) {
        for (int i = 0; i<g->nb_obstacles; i++) {
            hb = obstacle_hitbox(g->obstacles[i]);
            if (hb.a <= i && i <= hb.b) {
                return COLLIDE_NONE;
            }
            return COLLIDE_DEADLY;
        }       
    }
    return COLLIDE_ERROR;
}





/**
 * Fait avancer la grille de sol dans la direction du joueur donnée.
 * 
 * @param[in] b Le plateau contenant la grille de sol
 * @param[in] direction La direction du mouvement du joueur
 */
void ground_move(Board* b, int direction) {
    if (b == NULL || b->grid_ground == NULL) {
        return; // Si le plateau ou la grille du sol est NULL, on ne fait rien
    }

    switch (direction) {
        case DOWN: {
            Ground *first_ground = b->grid_ground[0]; 
            for (int i = 0; i < MAP_LEN - 1; i++) {
                b->grid_ground[i] = b->grid_ground[i + 1];
            }
            b->grid_ground[MAP_LEN - 1] = ground_generate(GROUND_GRASS, 0, 0, 0);
            ground_free(first_ground);
            break;
        }

        case UP: {
            Ground *last_ground = b->grid_ground[MAP_LEN - 1];
            for (int i = MAP_LEN - 1; i > 0; i--) {
                b->grid_ground[i] = b->grid_ground[i - 1];
            }
            b->grid_ground[0] = ground_generate(GROUND_GRASS, 0, 0, 0);
            ground_free(last_ground);
            break;
        }

        case LEFT: 
            break;
        

        case RIGHT: 
            break;
        
    }
}


char **grid_tui_make(Board *b) {
    char **grid = malloc(MAP_LEN * sizeof(char *));
    Ground *g;
    Couple hb;
    for (int lig = 0; lig < MAP_LEN; lig++) {
        grid[lig] = malloc(MAP_WIDTH * sizeof(char));
        g = b->grid_ground[lig];
        // initialisation du sol
        for (int col = 0; col < MAP_WIDTH; col++) {
            grid[lig][col] = g->model;
        }
        // ajout des abstacles
        for (int i = 0; i < g->nb_obstacles; i++) {
            hb = obstacle_hitbox(g->obstacles[i]);
            for (int j = hb.a; j<=hb.b; j++) {
                grid[lig][j] = g->obstacles[i]->model;
            }
        }
    }
    // ajout du poulet
    grid[V_POS][(int) b->player->h_position] = MODEL_CHICKEN;
    return grid;
}

Ground **grid_ground_make(void) {
    Ground **grid = malloc(MAP_LEN * sizeof(Ground *));
    for (int i = 0; i < MAP_LEN; i++) {
        grid[i] = ground_generate(GROUND_GRASS, 0, 0, 0); // Placeholder pour initialiser chaque ligne
    }
    return grid;
}

void grid_ground_free(Ground ** g){
    for (int i = 0; i<MAP_LEN; i++){
        ground_free(g[i]);
    }
    free(g);
}
