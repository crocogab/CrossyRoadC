#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdbool.h>


typedef struct _Obstacle {
    int type; //macro (voiture, camion, arbre etc)
    int variant;
    float h_position;
    float length;
} Obstacle;


typedef struct _couple {
    int a;
    int b;
} Couple;

Obstacle *obstacle_make(
    int type, 
    int variant,
    float h_position,
    float length
);

void obstacle_free(Obstacle *o);

void obstacle_update(Obstacle *o, float delta_t, float velocity);

Couple obstacle_hitbox(Obstacle *o);

bool obstacle_is_colliding(Obstacle *o, float player_pos);

#endif

