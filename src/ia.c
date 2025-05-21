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
            for (int i = 0; i < deep - 1; i++) {
                int *lig = hm[i][MAP_LEN-1];
                // on déplace toutes les lignes vers le haut
                for (int j = 0; j < MAP_LEN-1; j++) {
                    hm[i][j+1] = hm[i][j];
                }
                
                // pour la ligne en bas, on est pas censé y accéder donc whatever 
            }
            break;
        
        case DOWN:
            for (int i = 0; i < deep - 1; i++) {
                int *lig = hm[i][MAP_LEN-1];
                // on déplace toutes les lignes vers le bas
                for (int j = 0; j < MAP_LEN-1; j++) {
                    hm[i][j] = hm[i][j+1];
                }
                
                // pour la ligne en haut, on est pas censé y accéder donc whatever 
            }
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
        int deb, fin;
        
        for (int k = 0; k < g.nb_obstacles; k++) {
            
            //printf("OBSTACLE : %d  type : %d \n",k,g.obstacles[k]->type);
            hb = obstacle_simulated_hitbox(
                g.obstacles[k], 
                g.velocity, 
                t, 
                dt,
                g.special_attr
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
                for (int j = deb; j <= fin; j++) {
                        if (j >= 0 && j < MAP_WIDTH ) { //
                            // On met à jour la grille avec l'information de collision
                            hitgrid[i][j % MAP_WIDTH] = collide_obstacle;
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
    int player_h_index = (int)((b->player->h_position) / DEFAULT_CELL_SIZE);
    
   
    if (player_h_index < 0 || player_h_index >= MAP_WIDTH) {
        printf("Position invalide\n");
        return NEUTRAL; // hors limite
    }
    
    
    int **hitgrid = hitgrid_init(b->grid_ground, 0, dt);
    
    // recupère la prochaine collision
    int collision = hitgrid[V_POS-1][player_h_index];
    
    
    
    
    
    if (collision == COLLIDE_NONE) {
        return UP; 
    } else {
    
        int left_index = player_h_index - 1;
        int right_index = player_h_index + 1;
    
    
        bool can_go_left = (left_index >= 0);
        bool can_go_right = (right_index < MAP_WIDTH);
    
    
        int left_collision = can_go_left ? hitgrid[V_POS][left_index] : COLLIDE_DEADLY;
        int right_collision = can_go_right ? hitgrid[V_POS][right_index] : COLLIDE_DEADLY;
    
    
        if (left_collision == COLLIDE_NONE && right_collision == COLLIDE_NONE) {
        
            return (rand() % 2 == 0) ? LEFT : RIGHT;
        } else if (left_collision == COLLIDE_NONE) {
            return LEFT;
        } else if (right_collision == COLLIDE_NONE) {
            return RIGHT;
        } else {
        
            return NEUTRAL;
        }
    }
    hitgrid_free(hitgrid);
}

/**
 * fonction recursive auxiliaire pour l'ia I
 * 
 * @return true si le je peux 
 * 
 */
bool pouleroti_un(Board *b, int deep, int v, float h_pxl, int***hm, float dt, int *res) {
    if (deep == 0) {
        // condition d'arrêt
        return true;
    }

    int h = h_pxl / DEFAULT_CELL_SIZE;

    if (h < 0 || MAP_WIDTH <= h || V_POS - v >= MEMORISATION) {
        // hors de la map
        return false;
    }
    else if (hm[deep][v][h] != COLLIDE_NONE) {
        // case bloquée
        return false;
    }
    else if (pouleroti_un(b, deep - 1, v - 1, h, hm, dt, res)) {
        // on essaye up
        res[deep] = UP;
        return true;
    }
    else if (h < MAP_WIDTH/2 && pouleroti_un(b, deep-1, v, h+1, hm, dt, res)) {
        // si right est prio on essaye
        res[deep] = RIGHT;
        return true;
    }
    else if (pouleroti_un(b, deep-1, v, h-1, hm, dt, res)) {
        // on essaye left
        res[deep] = LEFT;
        return true; 
    } 
    else if (h >= MAP_WIDTH/2 && pouleroti_un(b, deep-1, v, h+1, hm, dt, res)) {
        // on réessaye right si pas prio
        res[deep] = RIGHT;
        return true;
    }
    else if (pouleroti_un(b, deep-1, v, h, hm, dt, res)) {
        // on essaye neutral
        res[deep] = NEUTRAL;
        return true;        
    } 
    else if (pouleroti_un(b, deep-1, v+1, h, hm, dt, res)){
        // on essaye down
        res[deep] = DOWN;
        return true;
    } 
    return false;
}


/**
 * V1 de l'ia.
 * Simulation de la trajectoire des obstacles et parcours en profondeur avec priorité au score.
 * 
 * Dès qu'un coup a été trouvé il est renvoyé.
 * 
 * NB une profondeur de 0 ça n'existe pas.
 * 
 * @param[in] board le plateau de jeu
 * @param[in] dt l'unité de temps, correspond au temps d'animation d'un jump
 * @param[in] max_deepness profondeur de simulation (>= 1)
 * 
 * @returns
 * Un pointeur vers une séquence de coups (tableau de taille deepness).
 */
int *pouleria_un(Board *b, int*** hm, float dt, int mdeep) {
    
    int *res = malloc(mdeep * sizeof(int));

    pouleroti_un(b, mdeep, V_POS, b->player->h_position, hm, dt, res);

    return res;
}
