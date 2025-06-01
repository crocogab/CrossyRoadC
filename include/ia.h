#ifndef IA_H
#define IA_H

#include <stdbool.h>
#include "board.h"

int ***hitmatrix_make(int max_deepness);
void hitmatrix_free(int ***hitmatrix, int max_deepness);
int ***hitmatrix_init(Board *, int max_deepness, float delta_t);
void hitmatrix_update(int ***hitmatrix, Board *, int max_deepness, float delta_t, int last_move);
void hitmatrix_fill(int ***hitmatrix, Board*, int max_deepness, float delta_t);

int **hitgrid_make(void);
void hitgrid_free(int **);
int **hitgrid_init(Ground **, float t, float dt);
void hitgrid_fill(int **hitgrid, Ground **, float t, float dt);

int pouleria_zero(Board *, float delta_t, int max_deepness);
bool pouleria_un(Board *, int***hitmatrix, float delta_t, int max_deepness, int *results);
bool pouleroti_un(Board *b, int max_deepness, int deepness, int v_index, float h_pos_pxl, int***hitmatrix, float dt_jump, int *results, char from);

#endif //IA_H
