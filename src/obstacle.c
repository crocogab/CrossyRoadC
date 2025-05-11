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
 * 
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
 * @param[in] velocity la vitesse de l'obstacle
 * 
 */
void obstacle_update(Obstacle *o, float delta_t, float velocity) {

    // On fait accélerer les rondins s'ils sont en dehors de la zone jouable
    if (o->type == LOG_TYPE && (o->h_position  < LEFT_MAP_X - DEFAULT_CELL_SIZE || o->h_position > RIGHT_MAP_X + DEFAULT_CELL_SIZE)) 
    {
        o->h_position += delta_t * velocity;
    }
    if (o->type==TRAIN_POLE_TYPE ){
        o->h_position -=delta_t * velocity;
    }


    // met a jour la position de l'obstacle (fonction appelée à chaque boucle)
    o->h_position = o->h_position + delta_t * velocity;

    if (o->type != TRAIN_TYPE)
    {
        // modulo à la main parce que math.h::fmod est bizarre
        if (o->h_position < 0) {
            o->h_position = o->h_position + MAP_WIDTH*DEFAULT_CELL_SIZE;
        } else if (o->h_position >= MAP_WIDTH*DEFAULT_CELL_SIZE) {
            o->h_position = o->h_position - MAP_WIDTH*DEFAULT_CELL_SIZE;
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

Couple obstacle_simulated_hitbox(Obstacle *o, float x, float dx) {

    float h = o->h_position + x; // position à t

     
    if (o->type != TRAIN_TYPE)
    {
        // modulo à la main parce que math.h::fmod est bizarre
        // if (h < 0) {
        //     h = h + MAP_WIDTH*DEFAULT_CELL_SIZE;
        // } else if (h >= MAP_WIDTH*DEFAULT_CELL_SIZE) {
        //     h = h - MAP_WIDTH*DEFAULT_CELL_SIZE;
        // }
        float i = 0;
        if (dx < 0) {i = i + dx;} else {i = i - dx;}
        if (o->length < 0) {i = i + o->length;} else {i = i - o->length;}

        while (h <= i) {
            h = h + MAP_WIDTH * DEFAULT_CELL_SIZE;
        }
        if (h + dx < 0 || h + dx + o->length < 0) {printf("en gros c se fout de ma gueule %f %f %f %f\n", h, dx, o->length,i);}
    }

    // intbox à t
    int a = (int) h;
    int b = (int) (h + o->length);
    // intbox à t + dt
    int c = (int) (h + dx);
    int d = (int) (h + dx + o->length);


    Couple ab;

    if (o->type == TRAIN_POLE_TYPE) {
        ab.a = 1; ab.b = 0;
    } else if ((o->length < 0) ^ (o->type == LOG_TYPE) ) {
        ab.a = b < d ? b : d;
        ab.b = a < c ? c : a;
    } else {
        ab.a = a < c ? a : c;
        ab.b = b < d ? d : b;
    }

    if (o->type != TRAIN_TYPE && ab.a < 0) {printf("OBSTACLE %i %i %i %i \n%i %i \n%f %f %f\n", a,b,c,d, ab.a,ab.b, h, dx, o->length);}
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
    
    // Vérification standard pour les obstacles dans le champ de vision
    bool avant = hb.a <= player_pos && player_pos <= hb.b;
    
    // Pour les obstacles qui peuvent faire le tour de l'écran (comme les voitures, camions)
    // mais pas pour les obstacles d'eau, qui ne devraient pas être détectés de l'autre côté
    bool apres = false;
    if (o->type != WATER_LILY_TYPE && o->type != LOG_TYPE && o->type!=TRAIN_TYPE) {
        apres = hb.a <= player_pos - MAP_WIDTH*DEFAULT_CELL_SIZE && 
                player_pos - MAP_WIDTH*DEFAULT_CELL_SIZE <= hb.b;
    }
    
    return avant || apres;
}
 
