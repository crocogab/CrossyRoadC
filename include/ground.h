#ifndef GROUND_H
#define GROUND_H

#include "obstacle.h"

typedef struct _Ground {
    Obstacle **obstacles; //Le tableau contiendra tout les obstacles présents sur la ligne
    float velocity; //Vélocité algébrique pour le déplacement des obstacles
    int type;
    int nb_obstacles; //Le nombre d'obstacles sur la ligne
} Ground;

Ground *ground_make(Obstacle *obscacles, float velocity, int type, int nb_obstacles);

void ground_free(Ground *g);

Ground *ground_generate(int type, float previous_velo);

#endif
