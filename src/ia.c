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
 * Remplit un hitmatrix (applique hitgrid_fill à chaque profondeur)
 */
void hitmatrix_fill(int ***hm, Board *b, int d, float dt) {
    for (int i = 0; i < d; i++) {
        hitgrid_fill(hm[i], b->grid_ground, i * dt, dt);
    }
}

/**
 * Crée et initialise une matrice 3D de collision à partir de l'état du jeu au moment de l'appel
 */
int ***hitmatrix_init(Board *b, int deepness, float dt) {
    int ***grid = hitmatrix_make(deepness);
    hitmatrix_fill(grid, b, deepness, dt);
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
                int *lig = hm[i][MAP_LEN-1];
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
                hm[i][MAP_LEN-1] =  lig;
                
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
bool pouleroti_un(Board *b, int mdeep, int deep, int v, float h_pxl, int***hm, float dt, int *res, char from) {
    int h = cased_h(h_pxl);
    // TODO ajouter le décalage dû au rondins

    if (h < 0 || MAP_WIDTH <= h || V_POS - v >= MEMORISATION) {
        // hors de la map
        printf("hors de la map\n");
        return false;
    }
    
    
    if (deep != 0) {
        printf("looked at %d %d | deepness %d from %c", v,h, deep - 1, from);
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

    else if (pouleroti_un(b, mdeep, deep+1, v - 1, h_pxl, hm, dt, res, '^')) {
        // on essaye up
        res[deep] = UP;
        return true;
    }
    else if (h < MAP_WIDTH/2 && pouleroti_un(b, mdeep, 1+ deep, v, h_pxl + DEFAULT_CELL_SIZE, hm, dt, res, '>')) {
        // si right est prio on essaye
        res[deep] = RIGHT;
        return true;
    }
    else if (pouleroti_un(b, mdeep, 1+deep, v, h_pxl - DEFAULT_CELL_SIZE, hm, dt, res, '<')) {
        // on essaye left
        res[deep] = LEFT;
        return true; 
    } 
    else if (h >= MAP_WIDTH/2 && pouleroti_un(b, mdeep, 1 + deep, v, h_pxl + DEFAULT_CELL_SIZE, hm, dt, res, '>')) {
        // on réessaye right si pas prio
        res[deep] = RIGHT;
        return true;
    }
    else if (pouleroti_un(b, mdeep, 1+deep, v, h_pxl, hm, dt, res, 'o')) {
        // on essaye neutral
        res[deep] = NEUTRAL;
        return true;        
    } 
    else if (pouleroti_un(b, mdeep, 1 + deep, v+1, h_pxl, hm, dt, res, 'v')){
        // on essaye down
        res[deep] = DOWN;
        return true;
    } 

    return false;
    
}


/**
 * Fonction récursive auxiliaire pour l'IA avec détection de boucles
 * 
 * @param[in] b Board du jeu
 * @param[in] mdeep profondeur maximale
 * @param[in] deep profondeur actuelle
 * @param[in] v position verticale
 * @param[in] h_pxl position horizontale en pixels
 * @param[in] hm hitmatrix 3D
 * @param[in] dt delta temps
 * @param[out] res tableau des mouvements résultants
 * @param[in] from caractère de debug
 * @param[in] visited_positions tableau des positions visitées
 * @param[in] visit_count nombre de positions visitées
 * 
 * @return true si un chemin sans boucle a été trouvé
 */
bool pouleroti_un_no_loop(Board *b, int mdeep, int deep, int v, float h_pxl, int***hm, 
                          float dt, int *res, char from, int **visited_positions, int visit_count) {
    int h = cased_h(h_pxl);
    
    // Vérifications de base
    if (h < 0 || MAP_WIDTH <= h || V_POS - v >= MEMORISATION) {
        printf("hors de la map (v=%d, h=%d)\n", v, h);
        return false;
    }
    
    // Vérifier si cette position a déjà été visitée dans ce chemin
    for (int i = 0; i < visit_count; i++) {
        if (visited_positions[i][0] == v && visited_positions[i][1] == h) {
            printf("Position (%d,%d) déjà visitée - éviter la boucle\n", v, h);
            return false;
        }
    }
    
    // Ajouter la position actuelle aux positions visitées
    if (visit_count < mdeep) {
        visited_positions[visit_count][0] = v;
        visited_positions[visit_count][1] = h;
        visit_count++;
    }
    
    // Vérification de collision
    if (deep != 0) {
        printf("looked at %d %d | deepness %d from %c", v, h, deep - 1, from);
        if (hm[deep-1][v][h] != COLLIDE_NONE) {
            printf(" -> nope (collision)\n");
            return false;
        } else {
            printf(" -> ok\n");
        }
    }
    
    // Condition d'arrêt
    if (deep == mdeep) {
        printf("Profondeur max atteinte - chemin trouvé!\n");
        return true;
    }
    
    // Définir les mouvements avec priorités améliorées
    typedef struct {
        int move;
        int delta_v;
        float delta_h;
        int base_priority;
        char debug_char;
        const char* name;
    } Move;
    
    Move moves[] = {
        {UP,      -1, 0,                    10, '^', "UP"},      // Priorité pour avancer
        {LEFT,     0, -DEFAULT_CELL_SIZE,    5, '<', "LEFT"},
        {RIGHT,    0, DEFAULT_CELL_SIZE,     5, '>', "RIGHT"},
        {NEUTRAL,  0, 0,                     2, 'o', "NEUTRAL"}, // Éviter de rester immobile
        {DOWN,     1, 0,                     1, 'v', "DOWN"}     // Éviter de reculer
    };
    
    // Adapter les priorités pour éviter les oscillations
    if (deep > 0) {
        // Regarder le mouvement précédent pour éviter l'aller-retour immédiat
        int prev_move = (deep > 0) ? res[deep-1] : NEUTRAL;
        
        switch (prev_move) {
            case LEFT:
                // Si on vient de LEFT, pénaliser RIGHT
                for (int i = 0; i < 5; i++) {
                    if (moves[i].move == RIGHT) {
                        moves[i].base_priority -= 3;
                    }
                }
                break;
            case RIGHT:
                // Si on vient de RIGHT, pénaliser LEFT
                for (int i = 0; i < 5; i++) {
                    if (moves[i].move == LEFT) {
                        moves[i].base_priority -= 3;
                    }
                }
                break;
            case UP:
                // Si on vient de UP, pénaliser DOWN
                for (int i = 0; i < 5; i++) {
                    if (moves[i].move == DOWN) {
                        moves[i].base_priority -= 5;
                    }
                }
                break;
            case DOWN:
                // Si on vient de DOWN, favoriser UP
                for (int i = 0; i < 5; i++) {
                    if (moves[i].move == UP) {
                        moves[i].base_priority += 3;
                    }
                }
                break;
        }
    }
    
    // CORRECTION: Calculer les priorités en fonction de la position par rapport au centre
    int center = MAP_WIDTH / 2;
    int distance_from_center = abs(h - center);
    
    // CORRECTION: Ajuster les priorités pour privilégier le centre
    for (int i = 0; i < 5; i++) {
        if (moves[i].move == LEFT || moves[i].move == RIGHT) {
            int new_h_after_move = h;
            if (moves[i].move == LEFT) new_h_after_move = h - 1;
            if (moves[i].move == RIGHT) new_h_after_move = h + 1;
            
            int new_distance_from_center = abs(new_h_after_move - center);
            
            // Bonus si le mouvement rapproche du centre
            if (new_distance_from_center < distance_from_center) {
                moves[i].base_priority += 3; // Bonus pour se rapprocher du centre
                printf("Bonus centrage pour %s: +3\n", moves[i].name);
            }
            // Pénalité si le mouvement éloigne du centre
            else if (new_distance_from_center > distance_from_center) {
                moves[i].base_priority -= 2; // Pénalité pour s'éloigner du centre
                printf("Pénalité centrage pour %s: -2\n", moves[i].name);
            }
            
            // Bonus supplémentaire si on est déjà loin du centre
            if (distance_from_center > 2 && new_distance_from_center < distance_from_center) {
                moves[i].base_priority += 2; // Bonus supplémentaire quand on est loin
                printf("Bonus supplémentaire pour %s: +2\n", moves[i].name);
            }
        }
    }
    
    // CORRECTION: Tri par priorité ET par préférence (en cas d'égalité, préférer le centrage)
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 5; j++) {
            bool should_swap = false;
            
            if (moves[i].base_priority < moves[j].base_priority) {
                should_swap = true;
            } else if (moves[i].base_priority == moves[j].base_priority) {
                // En cas d'égalité, préférer le mouvement qui centre
                if ((moves[j].move == LEFT || moves[j].move == RIGHT) && 
                    (moves[i].move != LEFT && moves[i].move != RIGHT)) {
                    
                    int h_after_j = h;
                    if (moves[j].move == LEFT) h_after_j = h - 1;
                    if (moves[j].move == RIGHT) h_after_j = h + 1;
                    
                    if (abs(h_after_j - center) < distance_from_center) {
                        should_swap = true;
                    }
                }
            }
            
            if (should_swap) {
                Move temp = moves[i];
                moves[i] = moves[j];
                moves[j] = temp;
            }
        }
    }
    
    // Affichage des priorités pour debug
    printf("Priorités ajustées (position h=%d, centre=%d):\n", h, center);
    for (int i = 0; i < 5; i++) {
        printf("  %s: priorité %d\n", moves[i].name, moves[i].base_priority);
    }
    
    // Explorer chaque mouvement
    for (int i = 0; i < 5; i++) {
        Move current_move = moves[i];
        int new_v = v + current_move.delta_v;
        float new_h = h_pxl + current_move.delta_h;
        int new_h_cell = cased_h(new_h);
        
        // Vérifications de limites
        bool move_valid = true;
        switch (current_move.move) {
            case LEFT:
                if (new_h_cell < 0) move_valid = false;
                break;
            case RIGHT:
                if (new_h_cell >= MAP_WIDTH) move_valid = false;
                break;
            case UP:
                if (new_v < 0) move_valid = false;
                break;
            case DOWN:
                if (new_v >= MAP_LEN) move_valid = false;
                break;
        }
        
        if (!move_valid) {
            printf("Mouvement %s invalide (limites)\n", current_move.name);
            continue;
        }
        
        // Vérification préalable de boucle
        bool would_create_loop = false;
        for (int j = 0; j < visit_count; j++) {
            if (visited_positions[j][0] == new_v && visited_positions[j][1] == new_h_cell) {
                would_create_loop = true;
                break;
            }
        }
        
        if (would_create_loop) {
            printf("Mouvement %s évité (créerait une boucle)\n", current_move.name);
            continue;
        }
        
        printf("Tentative mouvement %s vers (%d,%d)\n", current_move.name, new_v, new_h_cell);
        
        // Exploration récursive avec copie des positions visitées
        int **new_visited = malloc(mdeep * sizeof(int *));
        for (int j = 0; j < mdeep; j++) {
            new_visited[j] = malloc(2 * sizeof(int));
            if (j < visit_count) {
                new_visited[j][0] = visited_positions[j][0];
                new_visited[j][1] = visited_positions[j][1];
            }
        }
        
        if (pouleroti_un_no_loop(b, mdeep, deep + 1, new_v, new_h, hm, dt, res, 
                                current_move.debug_char, new_visited, visit_count)) {
            // Chemin trouvé ! Enregistrer le mouvement
            res[deep] = current_move.move;
            printf("Mouvement %s choisi à profondeur %d\n", current_move.name, deep);
            
            // Libérer la mémoire temporaire
            for (int j = 0; j < mdeep; j++) {
                free(new_visited[j]);
            }
            free(new_visited);
            
            return true;
        }
        
        // Libérer la mémoire si le chemin n'a pas fonctionné
        for (int j = 0; j < mdeep; j++) {
            free(new_visited[j]);
        }
        free(new_visited);
    }
    
    printf("Aucun chemin viable depuis (%d,%d) à profondeur %d\n", v, h, deep);
    return false;
}

/**
 * Version améliorée de l'IA qui évite les boucles
 */
bool pouleria_un(Board *b, int*** hm, float dt, int mdeep, int *moves) {
    reset_moves(moves, mdeep);
    
    // Allocation pour tracker les positions visitées
    int **visited_positions = malloc(mdeep * sizeof(int *));
    for (int i = 0; i < mdeep; i++) {
        visited_positions[i] = malloc(2 * sizeof(int)); // [v, h]
    }
    
    printf("=== IA v1 améliorée - Sans boucles ===\n");
    
    bool res = pouleroti_un_no_loop(b, mdeep, 0, V_POS, b->player->h_position, 
                                   hm, dt, moves, '.', visited_positions, 0);
    
    // Libérer la mémoire
    for (int i = 0; i < mdeep; i++) {
        free(visited_positions[i]);
    }
    free(visited_positions);
    
    if (res) {
        printf("Chemin sans boucle trouvé:\n");
    } else {
        printf("Aucun chemin sans boucle trouvé - stratégie de secours\n");
        // Stratégie simple : essayer d'avancer puis se déplacer latéralement
        for (int i = 0; i < mdeep; i++) {
            if (i % 3 == 0) {
                moves[i] = UP;    // Essayer d'avancer
            } else if (i % 3 == 1) {
                moves[i] = LEFT;  // Puis aller à gauche
            } else {
                moves[i] = RIGHT; // Puis aller à droite
            }
        }
    }
    
    print_hm(hm, mdeep, V_POS, cased_h(b->player->h_position));
    print_moves(moves, mdeep);
    return res;
}
