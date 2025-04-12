#include <stdlib.h>
#include "ground.h"
#include "obstacle.h"
#include "macro.h"
#include "random_custom.h"

/**
 * Crée un objet Ground avec les paramètres spécifiés.
 * 
 * @param obstacles Tableau de pointeurs vers les obstacles associés au sol.
 * @param velocity Vitesse des obstacles sur la ligne.
 * @param type Type de sol (défini dans les macros).
 * @param nb_obstacles Nombre d'obstacles sur la ligne.
 * @param special_attr Attribut spécial associé au sol (par exemple, temps pour un train).
 * @return Un pointeur vers un objet Ground alloué dynamiquement.
 */
Ground *ground_make(Obstacle **obstacles, float velocity, int type, int nb_obstacles, int special_attr) {
    Ground *g = malloc(sizeof(Ground));
    g->obstacles = obstacles; 
    //Initialisation des vitesses des obstacles sur la ligne
    g->velocity = velocity;
    g->type = type;
    g->nb_obstacles = nb_obstacles;
    g->special_attr = special_attr;
    return g;
}

/**
 * @brief Libère la mémoire associée à un objet Ground.
 * 
 * @param g Pointeur vers l'objet Ground à libérer.
 */
void ground_free(Ground *g) {
    for (int i = 0; i < g->nb_obstacles; i++)
    {
        obstacle_free(g->obstacles[i]);
    }
    free(g->obstacles);
    free(g);
}

/**
 * Renvoie l'id (macro gui) d'un objet en fonction de son type
 */
int type_var_to_id(int type, int variant)
{
    switch (type)
    {
    case CAR_TYPE:
        if (variant == 0)
        {
            return BLUE_CAR_ID;
        }
        else if (variant == 1)
        {
            return ORANGE_CAR_ID;
        }
        else
        {
            return PURPLE_CAR_ID;
        }     
        
        break;
    case TRUCK_TYPE:
        if (variant == 0)
        {
            return BLUE_TRUCK_ID;
        }
        else if (variant == 1)
        {
            return GAZ_TRUCK_ID;
        }
        else
        {
            return RED_TRUCK_ID;
        }
        break;
    case TRAIN_TYPE:
        return TRAIN_ID;
    case WATER_LILY_TYPE:
        return LILYPAD_ID;
    case LOG_TYPE:
        if (variant == 0)
        {
            return LOG_LONG_ID;
        }
        else if (variant == 1)
        {
            return LOG_MEDIUM_ID;
        }
        else
        {
            return LOG_SMALL_ID;
        }
    case TREE_TYPE:
        return TREE_ID;
    case ROCK_TYPE:
        return ROCK_ID;  
        
    
    default:
        return 0;
        break;
    }
}


/**
 * Génère un sol d'un type donné avec des obstacles et des paramètres spécifiques.
 * 
 * @param type Type de sol à générer (défini dans les macros).
 * @param previous_velo Vitesse du sol précédent pour éviter les conflits.
 * @param min_nb Nombre minimum d'obstacles à générer.
 * @param max_nb Nombre maximum d'obstacles à générer.
 * @return Un pointeur vers un objet Ground alloué dynamiquement.
 */
Ground *ground_generate(int type, float previous_velo, int min_nb, int max_nb, Sprite_sheet *sprite_sheet)
{   
    float velo = 0.0;
    int nb = 0;
    int choice = 0;
    int special_attr = 0;
    int variant = 0;
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
                choice = random_int(0, 1); // choix entre un arbre et un rocher
                if (choice == 0)
                {
                    obs[i] = obstacle_make(TREE_TYPE, variant, (int)obs_h_pos_array[i], sprite_sheet->sprites[type_var_to_id(TREE_TYPE, variant)].lenght);
                }
                else
                {
                    obs[i] = obstacle_make(ROCK_TYPE, variant, (int)obs_h_pos_array[i], sprite_sheet->sprites[type_var_to_id(ROCK_TYPE, variant)].lenght);
                }
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
        variant = random_int(0,2);
        for (int i = 0; i < nb; i++)
        {
            variant = random_int(0,2);
            obs[i] = obstacle_make(CAR_TYPE, variant, i*INTER_CAR_MIN, sprite_sheet->sprites[type_var_to_id(CAR_TYPE, variant)].lenght);
        }

        break;

    case GROUND_ROAD_TRUCK :
        
        //On choisit une vitesse pour les camions, simpliste pour l'instant car on ne choisit que d'alterner entre les vitesses + et -
        //On ne change pas non plus l'espacement entre les voitures
        variant = random_int(0,2);
        if (previous_velo <= 0)
        {
            velo = random_float((float)TRUCK_MIN_SPEED, (float)TRUCK_MAX_SPEED);
        }
        else
        {
            velo = - random_float((float)TRUCK_MIN_SPEED, (float)TRUCK_MAX_SPEED);
        }

        //On prends des précautions sur le nombre de voitures maximal
        if (max_nb >= MAP_WIDTH/INTER_TRUCK_MIN)
        {
            max_nb = MAP_WIDTH/INTER_TRUCK_MIN; //On ne peut pas avoir plus de voitures que la map est large
        }
        nb = random_int(min_nb, max_nb); //On tire au maximum des voitures espacées de INTER_CAR_MIN ou max_nb
        for (int i = 0; i < nb; i++)
        {
            variant = random_int(0, 2);
            obs[i] = obstacle_make(TRUCK_TYPE, variant, i*INTER_TRUCK_MIN, sprite_sheet->sprites[type_var_to_id(TRUCK_TYPE, variant)].lenght);
        }
        
        break;

    case GROUND_WATER_LILY :
        //On choisit une vitesse nulle pour les nénuphares (ils ne bougent pas)
        velo = 0;

        nb = random_int(min_nb, max_nb); //On tire un nombre d'obstacles entre min_nb et max_nb
        variant = random_int(0,2);
        //On va maintenant générer autant d'obstacles sur la ligne
        if (nb > 0) {
            int *obs_h_pos_array = random_int_array(0, MAP_WIDTH-1, nb);
            for (int i = 0; i < nb; i++)
            {
                obs[i] = obstacle_make(WATER_LILY_TYPE, variant, (int)obs_h_pos_array[i], sprite_sheet->sprites[type_var_to_id(WATER_LILY_TYPE, 0)].lenght);
            }
            free(obs_h_pos_array);
        }
        break;

    case GROUND_WATER_LOG:
        if (previous_velo <= 0)
        {
            velo = random_float((float)LOG_MIN_SPEED, (float)LOG_MAX_SPEED);
        }
        else
        {
            velo = - random_float((float)LOG_MIN_NUMBER, (float)LOG_MAX_SPEED);
        }

        int max_possible_nb = MAP_WIDTH / INTER_LOG_MIN;
        // S'assurer que max_nb ne dépasse pas le maximum possible
        if (max_nb > max_possible_nb) {
            max_nb = max_possible_nb;
        }
        // S'assurer que min_nb est valide (inférieur ou égal à max_nb)
        if (min_nb > max_nb) {
            min_nb = max_nb; // Ou gérer l'erreur autrement
        }

        nb = random_int(min_nb, max_nb);
        // variant = random_int(0,3); // Supprimer cette ligne redondante
        for (int i = 0; i < nb; i++) {
            // Générer la variante à l'intérieur de la boucle si chaque log peut avoir une variante différente
            variant = random_int(0, 2); // Utilise la plage correcte (0-2)
            // Corriger la faute de frappe: lenght -> length
            obs[i] = obstacle_make(LOG_TYPE, variant, i * INTER_LOG_MIN, sprite_sheet->sprites[type_var_to_id(LOG_TYPE, variant)].lenght);
        }
        
        break;

    case GROUND_TRAIN:
        if (previous_velo <= 0)
        {
            velo = random_float((float)TRAIN_MIN_SPEED, (float)TRAIN_MAX_SPEED);
        }
        else
        {
            velo = - random_float((float)TRAIN_MIN_SPEED, (float)TRAIN_MAX_SPEED);
        }

        special_attr = random_int(TRAIN_MIN_TIME, TRAIN_MAX_TIME);

        obs[0] = obstacle_make(TRAIN_TYPE, variant, -1, sprite_sheet->sprites[type_var_to_id(TRAIN_TYPE, 0)].lenght);

        nb = 1;

        break;
        

    default:
        break;
    }

    Ground *ans = ground_make(obs, velo, type, nb, special_attr);
    return ans;
}

