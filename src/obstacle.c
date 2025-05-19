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
    if (o->type == LOG_TYPE && (o->h_position< LEFT_MAP_X - DEFAULT_CELL_SIZE || o->h_position > RIGHT_MAP_X + DEFAULT_CELL_SIZE)) 
    {
        o->h_position += delta_t * velocity;
    }
    if (o->type==TRAIN_POLE_TYPE ){
        o->h_position -= delta_t * velocity;
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

Couple obstacle_simulated_hitbox(Obstacle *o, float v, float t, float dt, float spe) {

    Couple ab;
    if (o->type == TRAIN_POLE_TYPE) {
        // pour les poteaux -> pas de hitbox
        ab.a = 1; 
        ab.b = 0;
        return ab;
    }

    float h;
    if (o->type == TRAIN_TYPE) {
        t = t - spe;
        t = t < 0 ? 0 : t;
        h = v < 0 ? - o->length : MAP_WIDTH * DEFAULT_CELL_SIZE;
    } 

    float x = v * t;
    float dx = v * dt;

    h = o->h_position + x; // position à t
    
     
    if (o->type != TRAIN_TYPE) {
        
        float map_width_pixels = MAP_WIDTH * DEFAULT_CELL_SIZE;
        
        // ajuster h pour qu'il soit dans les limites
        while (h < 0) {
            h += map_width_pixels;
        }
        while (h >= map_width_pixels) {
            h -= map_width_pixels;
        }
    }

    // intbox à t
    int a = (int) h;
    int b = (int) (h + o->length);
    // intbox à t + dt
    int c = (int) (h + dx);
    int d = (int) (h + dx + o->length);

    if (o->type == LOG_TYPE) {
        ab.a = a ;
        ab.b = b;
        if (ab.a > ab.b) {
            // on inverse pour a <= b
            int temp = ab.a;
            ab.a = ab.b;
            ab.b = temp;
        }
        ab.a = a + DEFAULT_CELL_SIZE;
        ab.b = b - DEFAULT_CELL_SIZE;

    } else if (o->length < 0) {
        
        ab.a = b < d ? b : d;
        ab.b = a > c ? a : c;
        
        

    } else {
        
        ab.a = a < c ? a : c;
        ab.b = b > d ? b : d;
    }

    if (o->type != TRAIN_TYPE) {
        
        if (ab.a < 0) {
            
            
            printf("Attention: hitbox hors limites (a=%d) pour obstacle type %d\n", ab.a, o->type);
            
            
            ab.a = (ab.a % (MAP_WIDTH * DEFAULT_CELL_SIZE) + MAP_WIDTH * DEFAULT_CELL_SIZE) % (MAP_WIDTH * DEFAULT_CELL_SIZE);
            printf("Nouvelle position : %d ",ab.a);
        }
        if (ab.b < 0) {
            ab.b = (ab.b % (MAP_WIDTH * DEFAULT_CELL_SIZE) + MAP_WIDTH * DEFAULT_CELL_SIZE) % (MAP_WIDTH * DEFAULT_CELL_SIZE);
        }
    }
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
 
