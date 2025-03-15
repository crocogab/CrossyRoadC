#ifndef OBSTACLE_H
#define OBSTACLE_H

typedef struct _Obstacle {
    char type; //macro
    char model;
    float h_position;
    float velocity;
    float length;
} Obstacle;

#endif