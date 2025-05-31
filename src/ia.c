// #include "board.h"
// #include "macro.h"
#include "ia.h"
#include "macro.h"

#include <stdbool.h>
#include <stdlib.h>

#include <stdio.h> // debug

//MARK: hitmatrix
/**
 * Alloue de la mémoire pour une grille 3D de collision.
 * 
 * @param[in] max_deepness profondeur max d'exploration de l'ia qui va se servir de ce int***, donc profondeur de la matrice
 */
int ***hitmatrix_make(int deepness) {
    int ***matrix = malloc(deepness * sizeof(int **));
    for (int i=0; i<deepness;i++) {
        matrix[i] = hitgrid_make();
    }
    return matrix;
}


/**
 * Libère la mémoire allouée à une grille de collision 3D.
 */
void hitmatrix_free(int ***hitmatrix, int max_deepness) {
    for (int i = 0; i < max_deepness; i++) {
        hitgrid_free(hitmatrix[i]);
        hitmatrix[i] = NULL;
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
    // printf("0\n");
    int **deprecated_grid = hm[0];
    
    // printf("1\n");
    // Faire glisser toutes les grilles d'un cran (avancée dans le temps)
    for (int i = 1; i < deep; i++) {
        hm[i-1] = hm[i];
    }
    // printf("2\n");
    // Créer une nouvelle grille pour la dernière position
    // hm[deep-1] = hitgrid_init(b->grid_ground, (deep-1) * dt, dt);
    hitgrid_fill(deprecated_grid, b->grid_ground, (deep-1) * dt, dt);
    hm[deep-1] = deprecated_grid;
    // printf("3\n");
    // Modifier les grilles en fonction du coup joué
    switch (coup) {
        case UP:
            // printf("up\n");
            // Ajuster les prédictions en tenant compte que le joueur avance
            // on ajuste pas celle qu'on vient de créer parce qu'elle se base sur board up to date
            for (int i = 0; i < deep - 1; i++) {
                // printf("i\n");
                int *lig = hm[i][deep-1];
                // on déplace toutes les lignes vers le haut
                for (int j = 0; j < MAP_LEN-1; j++) {
                    hm[i][j+1] = hm[i][j];
                }
                hm[i][0] = lig;
                
                // pour la ligne en bas, on est pas censé y accéder donc whatever 
            }
            break;
        
        case DOWN:
            // printf("down\n");
            for (int i = 0; i < deep - 1; i++) {
                // on déplace toutes les lignes vers le bas
                int *lig =hm[i][0];
                for (int j = 0; j < MAP_LEN-1; j++) {
                    hm[i][j] = hm[i][j+1];
                }
                hm[i][deep-1] =  lig;
                
                // pour la ligne en haut, on est pas censé y accéder donc whatever 
                // on fait cycler pour pouvoir free sans se poser de questions
            }
            break;
            
        case LEFT:
        case RIGHT:
        case NEUTRAL:
            // Pas besoin d'ajustement spécial pour ces coups
            // printf("neutral\n");
            break;
        
        default:
            // Coup non reconnu, ne rien faire de spécial
            printf("coup default dans hm update\n");
            break;
    }
}
//MARK: hitgrid
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
        if (hitgrid[i] != NULL) {

            free(hitgrid[i]); 
            hitgrid[i] = NULL;
        }
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

//MARK: ia 0
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

//MARK: ia 1

void print_hm(int ***hm, int mdeep, int v, int h) {

    for (int i = 0; i < mdeep; i++) {
        printf("POULET: %d %d %d ---\n", i, v, h);
        for (int j = v-2; j < v +1 ; j++) {
            for (int k = h - 4; k < h + 5; k++) {
                
                if (h == k && j == v) {
                    printf("@");
                } 
                else if (hm[i][j][k] == COLLIDE_NONE) {
                    printf("_");
                }
                else if (hm[i][j][k] == COLLIDE_DEADLY) {
                    printf("+");
                } 
                else {
                    printf("#");
                }
            }
            
            printf("\n");
        }
    }
    printf("~~~~~~~~~~~~~~~\n");
}

int cased_h(float h_pxl) {
    return (h_pxl - DEFAULT_CELL_SIZE/4 + .00001) / DEFAULT_CELL_SIZE;
}

void print_moves(int *moves, int deepness){
    printf("MOVES: ");
    for (int i = 0; i < deepness; i++) {
        switch (moves[i]) {
            case UP: 
                printf("^ ");
            break;
            case DOWN: 
                printf("v ");
            case RIGHT: printf("> ");
            break;
            case LEFT: 
                printf("< ");
            break;
            case NEUTRAL: printf("o ");
            break;
            
        }
    }
    printf("\n");
}

void reset_moves(int *moves, int d) {
    // pour être sur qu'on a pas des mouvements rémanents
    for (int i = 0; i < d; i++) {
        moves[i] = 0;
    }
}

/**
 * fonction recursive auxiliaire pour l'ia I
 * 
 * @param[in] deep profondeur à laquelle on regarde
 * 
 * @return true si le coup ne condamne pas le poulet
 * 
 */
bool pouleroti_un(Board *b, int mdeep, int deep, int v, float h_pxl, int***hm, float dt, int *res) {
    int h = cased_h(h_pxl);
    // TODO ajouter le décalage dû au rondins

    if (h < 0 || MAP_WIDTH <= h || V_POS - v >= MEMORISATION) {
        // hors de la map
        printf("hors de la map\n");
        return false;
    }
    
    
    if (deep != 0) {
        printf("looked at %d %d | deepness %d", v,h, deep);
        if (hm[deep-1][v][h] != COLLIDE_NONE) {
            printf(" -> nope\n");
            return false;
        } else {
            printf(" -> ok\n");
        }
    }
    if (deep == mdeep) {
        // condition d'arrêt
        return true;
    }

    else if (pouleroti_un(b, mdeep, deep+1, v - 1, h_pxl, hm, dt, res)) {
        // on essaye up
        res[deep] = UP;
        return true;
    }
    else if (h < MAP_WIDTH/2 && pouleroti_un(b, mdeep, 1+ deep, v, h_pxl + DEFAULT_CELL_SIZE, hm, dt, res)) {
        // si right est prio on essaye
        res[deep] = RIGHT;
        return true;
    }
    else if (pouleroti_un(b, mdeep, 1+deep, v, h_pxl - DEFAULT_CELL_SIZE, hm, dt, res)) {
        // on essaye left
        res[deep] = LEFT;
        return true; 
    } 
    else if (h >= MAP_WIDTH/2 && pouleroti_un(b, mdeep, 1 + deep, v, h_pxl + DEFAULT_CELL_SIZE, hm, dt, res)) {
        // on réessaye right si pas prio
        res[deep] = RIGHT;
        return true;
    }
    else if (pouleroti_un(b, mdeep, 1+deep, v, h_pxl, hm, dt, res)) {
        // on essaye neutral
        res[deep] = NEUTRAL;
        return true;        
    } 
    else if (pouleroti_un(b, mdeep, 1 + deep, v+1, h_pxl, hm, dt, res)){
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
 * `true` si un chemin a été trouvé
 */
bool pouleria_un(Board *b, int*** hm, float dt, int mdeep, int *moves) {
    reset_moves(moves, mdeep);
    bool res = pouleroti_un(b, mdeep, 0, V_POS, b->player->h_position, hm, dt, moves);
    print_hm(hm, mdeep, V_POS, cased_h(b->player->h_position));
    print_moves(moves, mdeep);
    return res;
}
