#include <stdlib.h>
#include "ground.h"
#include "obstacle.h"
#include "macro.h"
#include "random_custom.h"

Ground *ground_make(Obstacle **obstacles, float velocity, int type, int nb_obstacles, char model) {
    Ground *g = malloc(sizeof(Ground));
    g->obstacles = obstacles; 
    //Initialisation des vitesses des obstacles sur la ligne
    g->velocity = velocity;
    g->type = type;
    g->nb_obstacles = nb_obstacles;
    g->model = model;
    return g;
}

void ground_free(Ground *g) {
    for (int i = 0; i < g->nb_obstacles; i++)
    {
        obstacle_free(g->obstacles[i]);
    }
    free(g->obstacles);
    free(g);
}

/**
 * Génére un sol d'un type donné en fonction du type donné (cf macro.h).
 * La mémoire est allouée par cette fonction
 * Ne fonctionne que pour les voitures et les arbres
 * 
 * @param [type] le type de sol (macro.h)
 * @param [previous_velo] la velocité du sol précédent pour éviter les conflits (sans vitesse = 0)
 * 
 * @return : un pointeur vers un sol alloué sur le tas
 */
Ground *ground_generate(int type, float previous_velo, int min_nb, int max_nb)
{   
    float velo = 0.0;
    int nb = 0;

    //On peut avoir au maximum autant d'obstacles que la map est large
    Obstacle **obs = malloc(sizeof(Obstacle *)*MAP_WIDTH);
    
    //On génére les obstacles sur la ligne
    switch (type)
    {
    case GROUND_GRASS:

        //On choisit une vitesse pour les obstacles
        velo = 0;

        nb = random_int(min_nb, max_nb); //On tire un nombre d'obstacles entre min_nb et max_nb

        //On va maintenant générer autant d'obstacles sur la ligne
        if (nb > 0) {
            int *obs_h_pos_array = random_int_array(0, MAP_WIDTH-1, nb);
            for (int i = 0; i < nb; i++)
            {
                obs[i] = obstacle_make(TREE_TYPE, TREE_MODEL, (int)obs_h_pos_array[i], TREE_LEN);
            }
            free(obs_h_pos_array);
        }

        break;
    
    case GROUND_ROAD_CAR:

        //On choisit une vitesse pour les voitures, simpliste pour l'instant car on ne choisit que d'alterner entre les vitesses + et -
        //On ne change pas non plus l'espacement entre les voitures
        if (previous_velo <= 0)
        {
            velo = random_float((float)CAR_MIN_SPEED, (float)CAR_MAX_SPEED);
        }
        else
        {
            velo = - random_float((float)CAR_MIN_SPEED, (float)CAR_MAX_SPEED);
        }

        //On prends des précautions sur le nombre de voitures maximal
        if (max_nb >= MAP_WIDTH/INTER_CAR_MIN)
        {
            max_nb = MAP_WIDTH/INTER_CAR_MIN; //On ne peut pas avoir plus de voitures que la map est large
        }
        nb = random_int(min_nb, max_nb); //On tire au maximum des voitures espacées de INTER_CAR_MIN ou max_nb
        for (int i = 0; i < nb; i++)
        {
            obs[i] = obstacle_make(CAR_TYPE, CAR_MODEL, i*INTER_CAR_MIN, CAR_LEN);
        }

        break;


    default:
        break;
    }

    Ground *ans = ground_make(obs, velo, type, nb, ground_model_of_type(type));
    return ans;
}

char ground_model_of_type(int type) {
    switch (type) {
        case GROUND_GRASS:
            return MODEL_GRASS;
        case GROUND_ROAD_CAR:
            return MODEL_ROAD;
        case GROUND_ROAD_TRUCKS:
            return MODEL_ROAD;
        case GROUND_WATER_LILY:
            return MODEL_WATER;
        case GROUND_WATER_LOGS:
            return MODEL_WATER;
        default:
            return 'E';
    } 
}
