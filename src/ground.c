#include <stdlib.h>
#include "ground.h"
#include "obstacle.h"
#include "macro.h"

Ground *ground_make(Obstacle **obstacles, float velocity, int type, int nb_obstacles) {
    Ground *g = malloc(sizeof(Ground));
    g->obstacles = obstacles; 
    //Initialisation des vitesses des obstacles sur la ligne
    g->velocity = velocity;
    g->type = type;
    g->nb_obstacles = nb_obstacles;
    return g;
}

void ground_free(Ground *g) {
    free(g->obstacles);
    free(g);
}

/**
 * Génére un sol d'un type donné en fonction du type donné (cf macro.h).
 * La mémoire est allouée par cette fonction
 * Ne fonctionne que pour les voitures et les arbres
 * 
 * @param [in] : type, le type de sol (macro.h)
 * @param [in] : previous_velo, la velocité du sol précédent pour éviter les conflits (sans vitesse = 0)
 * 
 * @return : un pointeur vers un sol alloué sur le tas
 */
Ground *ground_generate(int type, float previous_velo)
{   
    float velo = 0.0;
    int nb = 0;

    //On peut avoir au maximum autant d'obstacles que la map est large
    Obstacle **obs = malloc(sizeof(Obstacle)*MAP_WIDTH);
    
    //On génére les obstacles sur la ligne
    switch (type)
    {
    case GROUND_GRASS:

        //On choisit une vitesse pour les obstacles
        velo = 0;

        nb = rand() * (MAP_WIDTH/3) / (RAND_MAX); //On tire au maximum MAP_WIDTH/3 arbuste par ligne
        
        //On va maintenant générer autant d'obstacles sur la ligne
        for (int i = 0; i < nb; i++)
        {
            obs[i] = obstacle_make(TREE_TYPE, TREE_MODEL, (rand() * MAP_WIDTH / RAND_MAX), TREE_LEN);
        }

        break;
    
    case GROUND_ROAD_CAR:

        //On choisit une vitesse pour les voitures, simpliste pour l'instant car on ne choisit que d'alterner entre les vitesses + et -
        if (previous_velo <= 0)
        {
            velo = (rand() * (CAR_MAX_SPEED-CAR_MIN_SPEED) / (RAND_MAX)) + CAR_MIN_SPEED;
        }
        else
        {
            velo = - ((rand() * (CAR_MAX_SPEED-CAR_MIN_SPEED) / (RAND_MAX)) + CAR_MIN_SPEED);
        }

        nb = rand() * (MAP_WIDTH/INTER_CAR_MIN) / (RAND_MAX); //On tire au maximum des voitures espacées de 6
        for (int i = 0; i < nb; i++)
        {
            obs[i] = obstacle_make(CAR_TYPE, CAR_MODEL, i*6, CAR_LEN);
        }

        break;


    default:
        break;
    }

    Ground *ans = ground_make(obs, velo, type, nb);
    return ans;
}
