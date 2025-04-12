#include "obstacle.h"
#include "macro.h"
#include <stdlib.h>
#include <stdbool.h>
#include <gui.h>


/**
 * Crée un nouvel obstacle.
 * 
 * @param[in] type Le type d'obstacle
 * @param[in] variant La variante de l'obstacle
 * @param[in] h_position la position horizontale de l'avant de l'obstacle
 * @param[in] length La longueur de l'obstacle
 * 
 * @return Un pointeur vers le nouvel obstacle créé.
 */

Obstacle *obstacle_make(int type, int variant, float h_position, float lenght) {
    // crée et initialise un obstacle à partir des paramètres donnés
    Obstacle *obst = malloc(sizeof(Obstacle));
    obst->type = type;
    obst->variant = variant;
    obst->h_position = h_position;
    obst->length = lenght;
    return obst;
}


/**
 * Libère l'espace mémoire alloué à un espace
 * 
 * @param[in] o Un poiteur vers un obstacle à détruire
 */
void obstacle_free(Obstacle *o) {
    // libère la mémoire de l'obstacle donné
    if (o!=NULL) {free(o);}
}



/**
 * Met à jour la position d'un obstacle.
 * 
 * @param[in] o poiteur vers un obstacle à mettre à jour
 * @param[in] delta_t le temps écoulé depuis le dernier appel de cette fonction
 * 
 */
void obstacle_update(Obstacle *o, float delta_t, float velocity) {
    // met a jour la position de l'obstacle (fonction appelée à chaque boucle)
    o->h_position = o->h_position + delta_t * velocity;

    if (o->type != TRAIN_TYPE)
    {
        // modulo à la main parce que math.h::fmod est bizarre
        if (o->h_position < 0) {
            o->h_position = o->h_position + MAP_WIDTH;
        } else if (o->h_position >= MAP_WIDTH) {
            o->h_position = o->h_position - MAP_WIDTH;
        }
    }
}

/**
 * Calcule la hitbox d'un obstacle, ie les indices horizontaux des cases qu'il occupe
 * 
 * @param[in] o Un pointeur vers un obstacle
 * 
 * @return Le `Couple` des indices min et max occupés par l'obstacle dans la grille 
 */
Couple obstacle_hitbox(Obstacle *o) {
    // renvoie les h_indices min et max INCLUS des cases où l'obstacle se trouve 
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


/**
 * Vérifie si un obstacle donné est en collision avec la position du joueur
 * 
 * @param[in] o un obstacle
 * @param[in] player_pos la position du joueur (qui a un hitbox ponctuelle ici)
 * 
 * @return `true` ssi il y a collision
 */
bool obstacle_is_colliding(Obstacle *o, float player_pos) {
    Couple hb = obstacle_hitbox(o);
    bool avant = hb.a <= player_pos && player_pos <= hb.b;
    bool apres = hb.a <= player_pos - MAP_WIDTH && player_pos - MAP_WIDTH <= hb.b;
    return avant || apres;
}

