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
        }
        return COLLIDE_NONE;
    }
    else if (g->type == GROUND_ROAD_CAR || g->type == GROUND_ROAD_TRUCKS || g->type == GROUND_TRAIN) {
        for (int i = 0; i<g->nb_obstacles; i++) {
            hb = obstacle_hitbox(g->obstacles[i]);
            if (hb.a <= i && i <= hb.b) {
                return COLLIDE_DEADLY;
            }
        }    
        return COLLIDE_NONE;  
    }
    else if (g->type == GROUND_WATER_LOGS || g->type == GROUND_WATER_LILY) {
        for (int i = 0; i<g->nb_obstacles; i++) {
            hb = obstacle_hitbox(g->obstacles[i]);
            if (hb.a <= i && i <= hb.b) {
                return COLLIDE_NONE;
            }
        }    
        return COLLIDE_DEADLY;   
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
            b->grid_ground[0] = gen_next_ground(b, b->player->score); //Placeholder à changer avec la modification du champ score
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
    int k;
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
                k = j%MAP_WIDTH;
                if (k<0) {
                    k = k + MAP_WIDTH;
                }
                grid[lig][k] = g->obstacles[i]->model;
            }
        }
    }
    // ajout du poulet
    if (b->player != NULL)
    {
        grid[V_POS][(int) b->player->h_position] = MODEL_CHICKEN;
    }
    return grid;
}

/**
 * Libère la mémoire associée à la grille de char**. 
 * 
 * @param[in] g La grille à libérer
 */
void grid_tui_free(char **g) {
    for (int i = 0; i < MAP_LEN; i++) {
        free(g[i]);
    }
    free(g);
}

Ground **grid_ground_make(void) {
    Ground **grid = malloc(MAP_LEN * sizeof(Ground *));
    for (int i = 0; i < MAP_LEN; i++) {
        grid[i] = ground_generate(GROUND_GRASS, 0, 0, 0); // Placeholder pour initialiser chaque ligne
    }
    return grid;
}

/**
 * Change les valeurs de la grille de sol pour le démarrage du jeu.
 * i.e, crée un spawn similaire à celui du jeu
 * 
 * @param[in] b Le plateau contenant la grille de sol
 */
void grid_ground_starter_set(Board *b) {
    for (int i = 2; i <= min(MAP_LEN, START_SIZE); i++)
    {
        ground_free(b->grid_ground[MAP_LEN - i]);
        b->grid_ground[MAP_LEN - i] = ground_generate(GROUND_GRASS, 0, 0, 0);
    }
    ground_free(b->grid_ground[MAP_LEN - 1]);
    b->grid_ground[MAP_LEN-1] = ground_generate(GROUND_GRASS, 0, MAP_WIDTH, MAP_WIDTH);
}

void grid_ground_free(Ground ** g){
    for (int i = 0; i<MAP_LEN; i++){
        ground_free(g[i]);
    }
    free(g);
}

/**
 * Génère un nouveau sol en fonction du score et des sols précédents.
 * La génération est simpliste pour l'instant.
 * 
 * @param[in] b Le plateau contenant la grille de sol
 * @param[in] score Le score du joueur
 * 
 * @return Un pointeur vers le nouveau sol généré
 */
Ground *gen_next_ground(Board *b, int score)
{
    if (b == NULL) {
        return NULL; // Juste au cas où
    }

    switch (b->grid_ground[0]->type)
    {
    case GROUND_GRASS:
        if (score < DIFF_EASY) {
            return ground_generate(GROUND_ROAD_CAR, b->grid_ground[0]->velocity, 1, 3);
        }
        else if (score < DIFF_NORMAL) {
            return ground_generate(GROUND_ROAD_CAR, b->grid_ground[0]->velocity, 2, 4);
        }
        else if (score < DIFF_MEDIUM) {
            return ground_generate(GROUND_ROAD_CAR, b->grid_ground[0]->velocity, 2, 5);
        }
        else if (score < DIFF_HARD) {
            return ground_generate(GROUND_ROAD_CAR, b->grid_ground[0]->velocity, 2, 6);
        }
        else {
            return ground_generate(GROUND_ROAD_CAR, b->grid_ground[0]->velocity, 3, 7);
        }        
        break;

    case GROUND_ROAD_CAR:
        if (score < DIFF_EASY) {
            if (random_int(1, 10) < 5) {
                return ground_generate(GROUND_GRASS, b->grid_ground[0]->velocity, 1, 3);
            }
            else {
                return ground_generate(GROUND_ROAD_CAR, b->grid_ground[0]->velocity, 1, 3);
            }
        }
        else if (score < DIFF_NORMAL) {
            if (random_int(1, 10) < 4)
            {
                return ground_generate(GROUND_GRASS, b->grid_ground[0]->velocity, 1, 3);
            }
            else {
                return ground_generate(GROUND_ROAD_CAR, b->grid_ground[0]->velocity, 2, 4);
            }
        }
        else if (score < DIFF_MEDIUM) {
            if (random_int(1, 10) < 3)
            {
                return ground_generate(GROUND_GRASS, b->grid_ground[0]->velocity, 1, 3);
            }
            else {
                return ground_generate(GROUND_ROAD_CAR, b->grid_ground[0]->velocity, 2, 5);
            }
        }
        else if (score < DIFF_HARD) {
            if (random_int(1, 10) < 2)
            {
                return ground_generate(GROUND_GRASS, b->grid_ground[0]->velocity, 1, 3);
            }
            else {
                return ground_generate(GROUND_ROAD_CAR, b->grid_ground[0]->velocity, 2, 6);
            }
        }
        else
        {
            if (random_int(1, 10) < 1)
            {
                return ground_generate(GROUND_GRASS, b->grid_ground[0]->velocity, 1, 3);
            }
            else {
                return ground_generate(GROUND_ROAD_CAR, b->grid_ground[0]->velocity, 3, 7);
            }
        }
        break;            
    
    default:
        break;
    }
    
    
    
    return ground_generate(GROUND_GRASS, 0, 0, 0);
}
