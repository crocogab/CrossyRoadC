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

        /*Si le sol est un train, on vérifie si le special_attr est <= 0, 
        si oui, on bouge le train et on repasse l'attr à un nb aléatoire
        si non, on change l'attr et le train ne bouge pas*/
        if (ground->type == GROUND_TRAIN && ground->special_attr <= 0)
        {
            if (ground->type == GROUND_TRAIN && (ground->obstacles[0]->h_position >= MAP_WIDTH || ground->obstacles[0]->h_position < 0))
            {
                int s = random_int(0, 1);
                if (s == 0)
                {
                    ground->obstacles[0]->h_position = -TRAIN_LEN;
                    ground->velocity = TRAIN_MAX_SPEED;
                }
                else
                {
                    ground->obstacles[0]->h_position = MAP_WIDTH + TRAIN_LEN;
                    ground->velocity = -TRAIN_MAX_SPEED;
                }
                ground->special_attr = random_int(TRAIN_MIN_TIME, TRAIN_MAX_TIME);
            }
        }
        else if (ground->type == GROUND_TRAIN && ground->special_attr > 0)
        {
            
            ground->special_attr -= 1;
        }

        for (int i = 0; i < ground->nb_obstacles; i++) 
        {
        // update des obstacles
            obst = ground->obstacles[i]; // obst ou *obst ?????
            obstacle_update(obst, delta_t, ground->velocity);
        }
    }

    if (b->grid_ground[V_POS]->type == GROUND_WATER_LOG) { 
        // on suit le mouvement des rondins (on suppose qu'on y est sinon le joueur meurt)
        b->player->h_position =b->player->h_position + b->grid_ground[V_POS]->velocity * delta_t;
    } else {
        // hors rondin on reste sur la grille
        b->player->h_position = round(b->player->h_position);
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

    if (h<0 ||(int) h>= MAP_WIDTH){
        return COLLIDE_DEADLY; // hors de la map
    }

    switch (direction) {
        case UP:
            lig--; 
            break;
        case DOWN: 
            lig++;
            break;
        case RIGHT:
            h = h + 1;
            break;
        case LEFT: 
            h = h - 1;
            break;
    }
    Ground *g = b->grid_ground[lig];
    
    
    if (g->type == GROUND_GRASS) {
        for (int i = 0; i < g->nb_obstacles; i++) {
            // hb = obstacle_hitbox(g->obstacles[i]);
            // if (hb.a <= h && h <= hb.b) {
            if (obstacle_is_colliding(g->obstacles[i],(int) h)) {
                return COLLIDE_HARMLESS;
            }
        }
        return COLLIDE_NONE;
    }
    else if (g->type == GROUND_ROAD_CAR || g->type == GROUND_ROAD_TRUCK || g->type == GROUND_TRAIN) {
        for (int i = 0; i < g->nb_obstacles; i++) {
            // hb = obstacle_hitbox(g->obstacles[i]);
            // if (hb.a <= h && h <= hb.b) {
                if (obstacle_is_colliding(g->obstacles[i],(int) h)) {
                return COLLIDE_DEADLY;
            }
        }    
        return COLLIDE_NONE;  
    }
    else if (g->type == GROUND_WATER_LOG || g->type == GROUND_WATER_LILY) {
        for (int i = 0; i < g->nb_obstacles; i++) {
            // hb = obstacle_hitbox(g->obstacles[i]);
            // if (hb.a <= h && h <= hb.b) {
            if (obstacle_is_colliding(g->obstacles[i], (int)h)) {
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
            
            if (g->obstacles[i]->type == TRAIN_TYPE)
            {
                // Pour les trains, on affiche uniquement les parties du train qui sont dans la carte
                for (int j = hb.a; j <= hb.b; j++) {
                    if (j >= 0 && j < MAP_WIDTH) {
                        grid[lig][j] = g->obstacles[i]->model;
                    }
                }
                if (g->special_attr <= 200 && g->special_attr>0){
                    for (int j = 0;j < MAP_WIDTH;j++) {
                        grid[lig][j] = MODEL_INCOMING_TRAIN;
                    }
                    
                }
            }
            else
            {
                if (g->obstacles[i]->type != LOG_TYPE){
                    for (int j = hb.a; j<=hb.b; j++) {
                        k = j%MAP_WIDTH;
                        if (k<0) {
                            k = k + MAP_WIDTH;
                        }
                    grid[lig][k] = g->obstacles[i]->model;
                    }
                }else{
                    for (int j = hb.a; j<hb.b; j++) {
                        k = j%MAP_WIDTH;
                        if (k<0) {
                            k = k + MAP_WIDTH;
                        }
                        grid[lig][k] = g->obstacles[i]->model;
                }
                }
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
 * Fonction auxiliaire de gen_next_ground, sert à générer un sol de type eau
 * @param score le score
 * @param previous_velo vélocité du sol précédent
 * 
 */
Ground *gen_water(int score, float previous_velo)
{
    int s = random_int(1, 3);
    int mini, maxi;
    int type;
    
    if (s <= 2) // GROUND_WATER_LILY
    {
        type = GROUND_WATER_LILY;
        if (score < DIFF_EASY) {
            mini = 4;
            maxi = 7;
        } else if (score < DIFF_NORMAL) {
            mini = 3;
            maxi = 6;
        } else if (score < DIFF_MEDIUM) {
            mini = 3;
            maxi = 5;
        } else if (score < DIFF_HARD) {
            mini = 2;
            maxi = 4;
        } else {
            mini = 2;
            maxi = 3;
        }
    }
    else // GROUND_WATER_LOG
    {
        type = GROUND_WATER_LOG;
        mini = 2;
        maxi = 5;
    }
    return ground_generate(type, previous_velo, mini, maxi);
}

/**
 * Fonction auxiliaire de gen_next_ground, sert à générer un sol de type route
 * @param score le score
 * @param previous_velo vélocité du sol précédent
 * 
 */
Ground *gen_road(int score, float previous_velo)
{
    int s = random_int(1, 4);
    int mini, maxi;
    int type;
    
    if (s <= 2) //GROUND_ROAD_CAR
    {
        type = GROUND_ROAD_CAR;
        if (score < DIFF_EASY) {
            mini = 1;
            maxi = 3;
        } else if (score < DIFF_NORMAL) {
            mini = 2;
            maxi = 4;
        } else if (score < DIFF_MEDIUM) {
            mini = 2;
            maxi = 5;
        } else if (score < DIFF_HARD) {
            mini = 2;
            maxi = 6;
        } else {
            mini = 3;
            maxi = 7;
        }
    }
    else if (s <= 3)// GROUND_ROAD_TRUCK
    {
        type = GROUND_ROAD_TRUCK;
        if (score < DIFF_EASY) {
            mini = 1;
            maxi = 3;
        } else if (score < DIFF_NORMAL) {
            mini = 2;
            maxi = 3;
        } else if (score < DIFF_MEDIUM) {
            mini = 2;
            maxi = 4;
        } else if (score < DIFF_HARD) {
            mini = 2;
            maxi = 4;
        } else {
            mini = 3;
            maxi = 4;
        }
    }
    else //GROUND_TRAIN
    {
        type = GROUND_TRAIN;
        mini = 1;
        maxi = 1;
    }
    return ground_generate(type, previous_velo, mini, maxi);
}

/**
 * Génère un nouveau sol en fonction du score et des sols précédents.
 * La génération est simpliste pour l'instant.
 */
Ground *gen_next_ground(Board *b, int score) {
    if (b == NULL || b->grid_ground == NULL) {
        return NULL; // Juste au cas où
    }

    int s = random_int(1, 7);

    /**
     * L'automate suivi pour la génération sera :
     * Herbe -> (1/7 : herbe, 2/7 : eau, 4/7 : route)
     * Route -> (4/7 : route, 2/7 : herbe, 1/7 : eau)
     * Eau -> (3/7 : eau, 1/7 : route, 3/7 : herbe)
     */
    switch (b->grid_ground[0]->type) {
        case GROUND_GRASS:
            if (s == 1) {
                return ground_generate(GROUND_GRASS, 0, 1, 4);
            } else if (s <= 3) {
                return gen_water(score, b->grid_ground[0]->velocity);
            } else {
                return gen_road(score, b->grid_ground[0]->velocity);
            }

        case GROUND_ROAD_CAR:
        case GROUND_ROAD_TRUCK:
        case GROUND_TRAIN:
            if (s <= 4) {
                return gen_road(score, b->grid_ground[0]->velocity);
            } else if (s <= 6) {
                return ground_generate(GROUND_GRASS, 0, 1, 4);
            } else {
                return gen_water(score, b->grid_ground[0]->velocity);
            }

        case GROUND_WATER_LOG:
        case GROUND_WATER_LILY:
            if (s <= 3) {
                return gen_water(score, b->grid_ground[0]->velocity);
            } else if (s == 4) {
                return gen_road(score, b->grid_ground[0]->velocity);
            } else {
                return ground_generate(GROUND_GRASS, 0, 1, 4);
            }
            
        default:
            return ground_generate(GROUND_GRASS, 0, 0, 0);
    }
}
