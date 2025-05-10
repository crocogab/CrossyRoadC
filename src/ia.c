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

void hitmatrix_update(int***hm, Board *b, int deep, float dt, int coup) {
    
    // moving hitgrids
    hitgrid_free(hm[0]);
    for (int i = 1; i < deep; i++) {
        hm[i-1] = hm[i];
    }

    // generating the last grid
    hm[deep-1] = hitgrid_init(b->grid_ground, (deep-1) * dt, dt);

    // updating hitgrids
    switch (coup) {
        case UP:
            // 
            
            break ;
        
        case DOWN:

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
    // printf("init fill ok\n");
    for (int i = 0; i < MAP_LEN; i++) {
        g = *(grid_ground[i]);

        if (g.type == GROUND_WATER_LILY || g.type == GROUND_WATER_LOG) {
            collide_neutral = COLLIDE_DEADLY;
            collide_obstacle = COLLIDE_NONE;
        } else {
            collide_neutral = COLLIDE_NONE;
            if (g.type == GROUND_GRASS) {
                collide_obstacle = COLLIDE_HARMLESS;
            } else {
                collide_obstacle = COLLIDE_DEADLY;
            }
        }
        // printf("i = %i ok\n", i);
        // printf("j = ");
        // préremplissage par défaut en fonction du type de sol
        for (int j = 0; j < MAP_WIDTH; j++) {
            hitgrid[i][j] = collide_neutral;
            // printf("%i ", j);
        }
        // printf("ok\nk = ");
    
        // gestion des obstacles 
        float t0xcell = t * DEFAULT_CELL_SIZE;
        float t1xcell = (t + dt) * DEFAULT_CELL_SIZE; 
        for (int k = 0; k < g.nb_obstacles; k++) {
            hb = obstacle_simulated_hitbox(
                g.obstacles[k], 
                grid_ground[i]->velocity * t0xcell, 
                grid_ground[i]->velocity * t1xcell
            );

            for (int j = hb.a / DEFAULT_CELL_SIZE; j <= hb.b / DEFAULT_CELL_SIZE; j++) {
                hitgrid[i][j%MAP_WIDTH] = collide_obstacle; // pas sûr du modulo
            }
            // printf("%i ", k);
        }
        // printf("ok\n");
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
    int **hitgrid = hitgrid_init(b->grid_ground, 0, dt);
    int coll = hitgrid[V_POS][((int) b->player->h_position / DEFAULT_CELL_SIZE)] ;
    
    hitgrid_free(hitgrid);
    if (coll == COLLIDE_NONE) {
        return UP;
    } else {
        return NEUTRAL;
    }
}

