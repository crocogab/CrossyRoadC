#ifndef OBSTACLE_H
#define OBSTACLE_H



typedef struct _Obstacle {
    int type; //macro
    char model;
    float h_position;
    float length;
} Obstacle;


typedef struct _couple {
    int a;
    int b;
} Couple;

Obstacle *obstacle_make(
    int type, 
    char model, 
    float h_position,
    float length
);

void obstacle_free(Obstacle *o);

void obstacle_update(Obstacle *o, float delta_t, float velocity);

Couple obstacle_hitbox(Obstacle *o);


#endif

