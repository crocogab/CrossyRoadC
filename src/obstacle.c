#include "obstacle.h"
#include <stdlib.h>

Obstacle *obstacle_make(char type, char model, float h_position, float velocity, float length) {
    // crée et initialise un obstacle à partir des paramètres donnés
    Obstacle *obst = malloc(sizeof(Obstacle));
    obst->type = type;
    obst->model = model;
    obst->h_position = h_position;
    obst->velocity = velocity;
    obst->length = length;
    return obst;
}

void obstacle_free(Obstacle *o) {
    // libère la mémoire de l'obstacle donné
    if (o!=NULL) {free(o);}
}




void obstacle_update(Obstacle *o, float delta_t) {
    // met a jour la position de l'obstacle (fonction appelée à chaque boucle)
    o->h_position = o->h_position + delta_t * o->velocity;

}


Couple obstacle_hitbox(Obstacle *o) {
    // renvoie les h_indices min et max des cases où l'obstacle se trouve 
    int a = (int) o->h_position;
    int b = (int) o->h_position + o->length;
    
    if (a > b) {
        int c = a;
        a = b;
        b = c;
    }
    Couple ab;
    ab.a = a;
    ab.b = b;

    return ab;
}

