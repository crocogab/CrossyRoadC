#ifndef OBSTACLE_H
#define OBSTACLE_H


typedef struct _Obstacle {
    char type; //macro
    char model;
    float h_position;
    float velocity;
    float length;
} Obstacle;

typedef struct _couple {
    int a;
    int b;
} Couple;

Obstacle *obstacle_make(
    char type, 
    char model, 
    float h_position, 
    float velocity, 
    float length
);

void obstacle_free(Obstacle *o);

void obstacle_update(Obstacle *o, float delta_t);

Couple obstacle_hitbox(Obstacle *o);


#endif

