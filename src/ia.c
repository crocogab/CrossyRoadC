// #include "board.h"
// #include "macro.h"
#include "ia.h"
#include "macro.h"

#include <stdbool.h>
#include <stdlib.h>

#include <stdio.h> // debug


/**
 * Alloue de la mémoire pour une grille 3D de collision.
 * 
 * @param[in] max_deepness profondeur max d'exploration de l'ia qui va se servir de ce int***, donc profondeur de la matrice
 */
int ***hitmatrix_make(int deepness) {
    int ***matrix = malloc(deepness * sizeof(int **));
    for (int i=0; i<deepness;i++) {
        matrix[i] = malloc(MAP_LEN*sizeof(int *));
        for (int j=0; j<MAP_LEN; j++) {
            matrix[i][j] = malloc(MAP_WIDTH * sizeof(int));
        }
    }
    return matrix;
}

/**
 * Libère la mémoire allouée à une grille de collision 3D.
 */
void hitmatrix_free(int ***hitmatrix, int max_deepness) {
    for (int i = 0; i < max_deepness; i++) {
        for (int j = 0; j < MAP_LEN; j++) {
            free(hitmatrix[i][j]);
        }
        free(hitmatrix[i]);
    }
    free(hitmatrix);
}

/**
 * Crée et initialise une matrice 3D de collision à partir de l'état du jeu au moment de l'appel
 */
int ***hitmatrix_init(Board *b, int deepness, float dt) {
    int ***grid = hitmatrix_make(deepness);
    for (int i = 0; i < deepness; i++) {
        hitgrid_fill(grid[i], b->grid_ground, i * dt, dt);
    }
    return grid;
}

void hitmatrix_update(int ***hm, Board *b, int deep, float dt, int coup) {
    // Libérer la mémoire de la première grille
    if (hm[0] != NULL) {
        hitgrid_free(hm[0]);
    }
    
    // Faire glisser toutes les grilles d'un cran
    for (int i = 1; i < deep; i++) {
        hm[i-1] = hm[i];
    }

    // Créer une nouvelle grille pour la dernière position
    hm[deep-1] = hitgrid_init(b->grid_ground, (deep-1) * dt, dt);

    // Modifier les grilles en fonction du coup joué
    switch (coup) {
        case UP:
            // Ajuster les prédictions en tenant compte que le joueur avance
            // (Si nécessaire, implémenter la logique pour simuler l'avancement du joueur)
            break;
        
        case DOWN:
            // Ajuster les prédictions en tenant compte que le joueur recule
            // (Si nécessaire, implémenter la logique pour simuler le recul du joueur)
            break;
            
        case LEFT:
        case RIGHT:
        case NEUTRAL:
            // Pas besoin d'ajustement spécial pour ces coups
            break;
            
        default:
            // Coup non reconnu, ne rien faire de spécial
            break;
    }
}

/**
 * Alloue de la mémoire pour une grille de collision.
 */
int **hitgrid_make(void) {
    int **grid = malloc(MAP_LEN * sizeof(int *)); 
    for (int i = 0; i < MAP_LEN; i++) {
        grid[i] = malloc(MAP_WIDTH * sizeof(int));
    }
    // printf("hg make ok\n");
    return grid;
}

/**
 * Libère la mémoire allouée à une grille de collision.
 */
void hitgrid_free(int **hitgrid) {
    for (int i = 0; i < MAP_LEN; i++) {
        free(hitgrid[i]);
    }
    free(hitgrid);
}

/**
 * Crée et initialise une grille de collision à un temps donné (relatif au moment de l'appel).
 */
int **hitgrid_init(Ground **grid_ground, float t, float dt) {
    int **hitgrid = hitgrid_make();
    hitgrid_fill(hitgrid, grid_ground, t, dt);
    // printf("hg init ok\n");
    return hitgrid;
}

/**
 * Remplit une grille de collision déjà créée, à un temps donné.
 */
void hitgrid_fill(int **hitgrid, Ground **grid_ground, float t, float dt) {
    Ground g;
    Couple hb;
    int collide_neutral;
    int collide_obstacle;
    float map_width_pixels = MAP_WIDTH * DEFAULT_CELL_SIZE;
    
    for (int i =  0; i<MAP_LEN; i++) {
        g = *(grid_ground[i]);
        
        //on itère à l'envers pour le debug
        // if (i==V_POS-1){
        //     printf("SOL num %d TYPE : %d RAND : %d\n",i,g.type,rand()%20);
        // }
        
        
        
        
        if (g.type == GROUND_WATER_LILY || g.type == GROUND_WATER_LOG) {
            collide_neutral = COLLIDE_DEADLY;  // eau mortelle
            collide_obstacle = COLLIDE_NONE;   // obstacles sont surs
        } else {
            collide_neutral = COLLIDE_NONE;    // sol est sur par default
            if (g.type == GROUND_GRASS ) { 
                collide_obstacle = COLLIDE_HARMLESS; //arbres = innofensifs
            } else {
                collide_obstacle = COLLIDE_DEADLY;   // Autres obstacles = mortels
            }
        }
        
        // par default on met à neutral
        for (int j = 0; j < MAP_WIDTH; j++) {
            hitgrid[i][j] = collide_neutral;
        }
    
        // Gestion des obstacles 
        float t0xcell = t * DEFAULT_CELL_SIZE;      // position temps 0
        float t1xcell = (t + dt) * DEFAULT_CELL_SIZE; // position au temps t+dt
        int deb, fin;
        
        for (int k = 0; k < g.nb_obstacles; k++) {
            
            //printf("OBSTACLE : %d  type : %d \n",k,g.obstacles[k]->type);
            hb = obstacle_simulated_hitbox(
                g.obstacles[k], 
                g.velocity * t0xcell, 
                g.velocity * (t1xcell - t0xcell)
            );
            

            deb = hb.a / DEFAULT_CELL_SIZE;
            fin = hb.b / DEFAULT_CELL_SIZE;  
            //printf("OBSTACLE %d , hitbox : (%d,%d )\n",k,deb,fin);

            if (g.type == GROUND_TRAIN) {
                
                // for (int j = 0; j < MAP_WIDTH; j++) {
                //     hitgrid[i][j] = COLLIDE_DEADLY;
                // }
                
                
                break;
            } else {
                if (g.obstacles[k]->type != LOG_TYPE ){
                    for (int j = deb; j <= fin; j++) {
                        if (j >= 0 && j < MAP_WIDTH ) { //
                            // On met à jour la grille avec l'information de collision
                            hitgrid[i][j % MAP_WIDTH] = collide_obstacle;
                        }
                    }

                }else{
                    for (int j = 0; j < MAP_WIDTH; j++) {
                        int wrapped_j = (j % MAP_WIDTH + MAP_WIDTH) % MAP_WIDTH;
                        int prev_j = ((j-1) % MAP_WIDTH + MAP_WIDTH) % MAP_WIDTH;
                        
                        
                        float test_offset = DEFAULT_CELL_SIZE/4;
                        
                        
                        bool current_collision = obstacle_is_colliding(g.obstacles[k], wrapped_j * DEFAULT_CELL_SIZE + test_offset);
                        
                        
                        bool prev_collision = obstacle_is_colliding(g.obstacles[k], prev_j * DEFAULT_CELL_SIZE + test_offset);
                        
                        if (current_collision) {
                            
                            hitgrid[i][wrapped_j] = collide_obstacle;
                        }
                        
                        if (prev_collision) {
                            
                            hitgrid[i][prev_j] = collide_obstacle;
                        } else if (current_collision) {
                            
                            hitgrid[i][wrapped_j] = collide_obstacle;
                        }
                    }
            
                }
                
            }
        }
        //printf("-------------------------- \n");
    }
}


/**
 * version zero de l'ia. Se contente de regarder devant et d'avancer si c'est possible. 
 * 
 * @param[in] board le plateau de  tel qu'il est au moment de l'appel
 * @param[in] delta_t l'unité de temps utlisée pour l'ia, correspond probablement à la durée de l'animation de temps
 * @param[in] max_deepness profondeur maximum de simulation, inutile ici.
 * 
 * @return un entier, une macro qui correspond à un des 5 coups possibles.
 */
int pouleria_zero(Board *b, float dt, int maxd) {
    
    if (b->player == NULL || b->player->h_position < 0) {
        printf("Position invalide\n");
        return NEUTRAL; // invalide : ne rien faire
    }
    
    // calcule indice du joueur
    int player_h_index = (int)(b->player->h_position) / DEFAULT_CELL_SIZE;
    
   
    if (player_h_index < 0 || player_h_index >= MAP_WIDTH) {
        printf("Position invalide\n");
        return NEUTRAL; // hors limite
    }
    
    
    int **hitgrid = hitgrid_init(b->grid_ground, 0, dt);
    
    // recupère la prochaine collision
    int collision = hitgrid[V_POS-1][player_h_index];
    
    
    hitgrid_free(hitgrid);
    
    
    if (collision == COLLIDE_NONE) {
        return UP; // si pas obstacle on avance
    } else {

        return NEUTRAL;
    }
}

