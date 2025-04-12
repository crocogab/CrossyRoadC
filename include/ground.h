#ifndef GROUND_H
#define GROUND_H

#include "obstacle.h"

typedef struct _Ground {
    Obstacle **obstacles; //Le tableau contiendra tout les obstacles présents sur la ligne
    float velocity; //Vélocité algébrique pour le déplacement des obstacles
    int type;
    int nb_obstacles; //Le nombre d'obstacles sur la ligne
    int special_attr; //Un attribut spécial pour certains types d'obstacles (les trains :/)
} Ground;

Ground *ground_make(Obstacle **obscacles, float velocity, int type, int nb_obstacles, int special_attr);

void ground_free(Ground *g);

Ground *ground_generate(int type, float previous_velo, int min_nb, int max_nb);

char ground_model_of_type(int type);

#endif
