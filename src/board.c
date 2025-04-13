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
Board* board_make(Sprite_sheet *sprite_sheet) {
    Board* b = (Board*)malloc(sizeof(Board));
    if (b == NULL) {
        return NULL; // En cas d'erreur d'allocation
    }
    b->grid_ground = grid_ground_make(sprite_sheet); // Fonction qui initialise le sol
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
void board_update(Board* b, float delta_t, Sprite_sheet *sprite_sheet) {
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
                    ground->obstacles[0]->h_position = -sprite_sheet->sprites[TRAIN_ID].lenght;
                    ground->velocity = TRAIN_MAX_SPEED;
                }
                else
                {
                    ground->obstacles[0]->h_position = MAP_WIDTH + sprite_sheet->sprites[TRAIN_ID].lenght;
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
 * @note tomber dans l'eau renvoie COLLIDE_DEADLY et marcher sur un rondin COLLIDE_NONE
 */
int check_future_collision(Board *b, int direction) {
    if (b == NULL) {
        return COLLIDE_ERROR;
    }
    
    // on change virtuellement la ligne et la position du joueur en fonction du coup donné
    int lig = V_POS;
    float h = b->player->h_position;

    if (h<LEFT_MAP_X ||(int) h>= RIGHT_MAP_X){
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
            h = h + DEFAULT_CELL_SIZE;
            break;
        case LEFT: 
            h = h - DEFAULT_CELL_SIZE;
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
void ground_move(Board* b, int direction, Sprite_sheet *sprite_sheet) {
    if (b == NULL || b->grid_ground == NULL) {
        return; // Si le plateau ou la grille du sol est NULL, on ne fait rien
    }

    switch (direction) {
        case DOWN: {
            Ground *first_ground = b->grid_ground[0]; 
            for (int i = 0; i < MAP_LEN - 1; i++) {
                b->grid_ground[i] = b->grid_ground[i + 1];
            }
            b->grid_ground[MAP_LEN - 1] = ground_generate(GROUND_GRASS, 0, 0, 0, sprite_sheet);
            ground_free(first_ground);
            break;
        }

        case UP: {
            Ground *last_ground = b->grid_ground[MAP_LEN - 1];
            for (int i = MAP_LEN - 1; i > 0; i--) {
                b->grid_ground[i] = b->grid_ground[i - 1];
            }
            b->grid_ground[0] = gen_next_ground(b, b->player->score, sprite_sheet); //Placeholder à changer avec la modification du champ score
            ground_free(last_ground);
            break;
        }

        case LEFT: 
            break;
        

        case RIGHT: 
            break;
        
    }
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


/**
 * Alloue et initialise un tableau 2D de pointeurs vers des structures Ground
 * Chaque ligne de la grille est initialisée à l'aide de la fonction `ground_generate`
 * avec des valeurs par défaut.
 *
 * @return Ground** Un pointeur vers la grille 2D allouée de structures Ground.
 *         L'appelant est responsable de libérer la mémoire allouée.
 */
Ground **grid_ground_make(Sprite_sheet *sprite_sheet) {
    Ground **grid = malloc(MAP_LEN * sizeof(Ground *));
    for (int i = 0; i < MAP_LEN; i++) {
        grid[i] = ground_generate(GROUND_GRASS, 0, 0, 0, sprite_sheet); // Valeurs par défaut pour initialiser chaque ligne
    }
    return grid;
}

/**
 * Change les valeurs de la grille de sol pour le démarrage du jeu.
 * i.e, crée un spawn similaire à celui du jeu
 * 
 * @param[in] b Le plateau contenant la grille de sol
 */
void grid_ground_starter_set(Board *b, Sprite_sheet *sprite_sheet) {

    for (int i = 2; i <= min(MAP_LEN, START_SIZE); i++)
    {   
        
        ground_free(b->grid_ground[i]);
        b->grid_ground[i] = gen_next_ground(b,0,sprite_sheet);
        
    }
    ground_free(b->grid_ground[MAP_LEN - 1]);
    b->grid_ground[MAP_LEN-1] = ground_generate(GROUND_GRASS, 0, 0, 0, sprite_sheet);
}
    
/**
 * Libère la mémoire allouée pour une grille de type Ground.
 * @param g Un pointeur vers un tableau de pointeurs de type Ground à libérer.
 */
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
Ground *gen_water(int score, float previous_velo, Sprite_sheet *sprite_sheet)
{
    int s = random_int(1, 5);
    int mini, maxi;
    int type;
    
    if (s <= 1) // GROUND_WATER_LILY
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
        maxi = 4;
    }
    return ground_generate(type, previous_velo, mini, maxi, sprite_sheet);
}

/**
 * Fonction auxiliaire de gen_next_ground, sert à générer un sol de type route
 * @param score le score
 * @param previous_velo vélocité du sol précédent
 * 
 */
Ground *gen_road(int score, float previous_velo, Sprite_sheet *sprite_sheet)
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
    return ground_generate(type, previous_velo, mini, maxi, sprite_sheet);
}

/**
 * Génère un nouveau sol en fonction du score et des sols précédents.
 * La génération est simpliste pour l'instant.
 *
 * @param b Pointeur vers la structure Board contenant les informations du plateau de jeu.
 *          Doit contenir un tableau de sols (grid_ground) non nul.
 * @param score Le score actuel du joueur, utilisé pour ajuster la génération.
 * 
 * @return Un pointeur vers une structure Ground représentant le nouveau sol généré.
 *         Retourne NULL si le pointeur Board ou grid_ground est nul.
 */
Ground *gen_next_ground(Board *b, int score, Sprite_sheet *sprite_sheet) {
    if (b == NULL || b->grid_ground == NULL) {
        return NULL; // Juste au cas où
    }

    int s = random_int(1, 7);

    /**
     * L'automate suivi pour la génération sera :
     * Herbe -> (1/7 : herbe, 2/7 : eau, 4/7 : route)
     * Route -> (4/7 : route, 2/7 : herbe, 1/7 : eau)
     * Eau -> (2/7 : eau, 1/7 : route, 4/7 : herbe)
     */
    switch (b->grid_ground[0]->type) {
        case GROUND_GRASS:
            if (s == 1) {
                return ground_generate(GROUND_GRASS, 0, 1, 4, sprite_sheet);
            } else if (s <= 3) {
                return gen_water(score, b->grid_ground[0]->velocity, sprite_sheet);
            } else {
                return gen_road(score, b->grid_ground[0]->velocity, sprite_sheet);
            }

        case GROUND_ROAD_CAR:
        case GROUND_ROAD_TRUCK:
        case GROUND_TRAIN:
            if (s <= 4) {
                return gen_road(score, b->grid_ground[0]->velocity, sprite_sheet);
            } else if (s <= 6) {
                return ground_generate(GROUND_GRASS, 0, 1, 4, sprite_sheet);
            } else {
                return gen_water(score, b->grid_ground[0]->velocity, sprite_sheet);
            }

        case GROUND_WATER_LOG:            
            if (s <= 1) {
                return gen_water(score, b->grid_ground[0]->velocity, sprite_sheet);
            } else if (s == 1) {
                return gen_road(score, b->grid_ground[0]->velocity, sprite_sheet);
            } else {
                return ground_generate(GROUND_GRASS, 0, 1, 4, sprite_sheet);
            }

        case GROUND_WATER_LILY:
            if (s <= 1) {
                return ground_generate(GROUND_WATER_LOG, b->grid_ground[0]->velocity, 2, 4 ,sprite_sheet);
            } else if (s == 1) {
                return gen_road(score, b->grid_ground[0]->velocity, sprite_sheet);
            } else {
                return ground_generate(GROUND_GRASS, 0, 1, 4, sprite_sheet);
            }
            
        default:
            return ground_generate(GROUND_GRASS, 0, 0, 0, sprite_sheet);
    }
}

/**
 * Fonction qui dessine le plateau de jeu
 * @param b le plateau de jeu
 * @param cam la caméra
 * @param display les informations d'affichage
 * @param colors les couleurs
 * @param renderer le renderer
 * 
 */
void draw_board(Board *b, Camera cam, Display_informations display, Colors colors, SDL_Renderer *renderer, Sprite_sheet *sprite_sheet)
{
    // On dessine le sol
    for (int i = 0; i < display.board_length; i++)
    {
        //Si on a une route avant on dessine une ROAD_BORDER
        if (i > 0 && (b->grid_ground[i]->type == GROUND_ROAD_CAR || b->grid_ground[i]->type == GROUND_ROAD_TRUCK) && (b->grid_ground[i-1]->type == GROUND_ROAD_CAR || b->grid_ground[i-1]->type == GROUND_ROAD_TRUCK))
        {
            draw_board_line(i, GROUND_ROAD_BORDER, cam, display, colors, renderer, sprite_sheet);
        }
        else
        {
            draw_board_line(i, b->grid_ground[i]->type, cam, display, colors, renderer, sprite_sheet);
        }
    }
}

/**
 * Fonction qui dessine les obstacles
 * @param b le plateau de jeu
 * @param cam la caméra
 * @param display les informations d'affichage
 * @param colors les couleurs
 * @param renderer le renderer
 * 
 */
void draw_entities(Board *b, Camera cam, Display_informations display, Colors colors, SDL_Renderer *renderer, Sprite_sheet *sprite_sheet)
{
    if (b == NULL || b->grid_ground == NULL) {
        return; // Si le plateau ou la grille du sol est NULL, on ne fait rien
    }

    // On a un flag pour savoir si le joueur a déjà été affiché
    bool displayed = false;

    for (int i = 0; i < MAP_LEN; i++)
    {
        Ground *ground = b->grid_ground[i];
        for (int j = 0; j < ground->nb_obstacles; j++)
        {
            // Il ne faut pas oublier de dessiner le joueur
            if (!displayed && i == V_POS && !(ground->type == GROUND_WATER_LILY || ground->type == GROUND_WATER_LOG))
            {
                displayed = true;
                draw_chicken(b->player, sprite_sheet, renderer, cam, display);
            }
        
            switch (ground->obstacles[j]->type)
            {
            case CAR_TYPE:
            case TRUCK_TYPE:
                if (ground->velocity > 0)
                {
                    draw_sprite_from_grid(ground->obstacles[j]->h_position, i, type_var_to_id(ground->obstacles[j]->type, ground->obstacles[j]->variant), 1, sprite_sheet, renderer, cam, display);
                }
                else
                {
                    draw_sprite_from_grid(ground->obstacles[j]->h_position, i, type_var_to_id(ground->obstacles[j]->type, ground->obstacles[j]->variant), 0, sprite_sheet, renderer, cam, display);
                }                
                break;

            case TREE_TYPE:
                draw_sprite_from_grid(ground->obstacles[j]->h_position, i, type_var_to_id(ground->obstacles[j]->type, ground->obstacles[j]->variant), ground->obstacles[j]->variant, sprite_sheet, renderer, cam, display);
                break;

            case WATER_LILY_TYPE:
                draw_sprite_from_grid(ground->obstacles[j]->h_position, i, type_var_to_id(ground->obstacles[j]->type, ground->obstacles[j]->variant), ground->obstacles[j]->variant, sprite_sheet, renderer, cam, display);
                break;

            default:
                draw_sprite_from_grid(ground->obstacles[j]->h_position, i, type_var_to_id(ground->obstacles[j]->type, ground->obstacles[j]->variant), 0, sprite_sheet, renderer, cam, display);
                break;
            }

        }
        if (!displayed && i == V_POS)
        {
            draw_chicken(b->player, sprite_sheet, renderer, cam, display);
        }
        
    }
}
