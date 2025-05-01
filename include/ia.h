#include "game.h"

int *poulerIA_zero(Game *, float delta_t, int max_deepness);

int ***hitmatrix_make(int max_deepness);
void hitmatrix_free(int ***hitmatrix, int max_deepness);
int ***hitmatrix_init(Board *, int max_deepness, float delta_t);

int **hitgrid_make(void);
void hitgrid_free(int **);
int **hitgrid_init(Ground **, float t);
void hitgrid_fill(int **hitgrid, Ground **, float t);

