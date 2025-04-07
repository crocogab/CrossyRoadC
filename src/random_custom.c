#include <stdlib.h>
#include "random_custom.h"

/**
 *  Renvoie un entier aléatoire entre min et max (min < max)
 * 
 *  @param min : valeur minimale (incluse)
 *  @param max : valeur maximale (incluse)
 * 
 *  @return un entier aléatoire entre min et max
 */
int random_int(int min, int max)
{
    return (rand()%(max - min + 1)) + min;
}


/**
 * Renvoie un float aléatoire entre min et max (min < max)
 * 
 *  @param min valeur minimale (incluse)
 *  @param max valeur maximale (excluse)
 *  
 *  @return un float aléatoire entre min et max
 */
float random_float(float min, float max)
{
    return ((float)rand()/(float)(RAND_MAX)) * (max - min) + min;
}

/**
 * Renvoie un tableau d'entiers aléatoires entre min et max (min < max)
 * 
 *  @param min valeur minimale (incluse)
 *  @param max valeur maximale (incluse)
 *  @param size taille du tableau
 *  
 *  @return un tableau d'entiers aléatoires entre min et max 
 *  où les valeurs sont triées
 */
int *random_int_array(int min, int max, int size)
{
    int *array = malloc(size * sizeof(int));
    if (array == NULL || size <= 0) {
        return NULL; //Le tableau n'a pas été alloué correctement ou la taille est invalide
    }

    // Choix de valeurs à mettre dans le tableau (sans doublons)
    for (int i = 0; i < size; i++) {
        array[i] = random_int(min, max);
        for (int j = 0; j < i; j++) {
            if (array[i] == array[j]) {
                i--; //On retire un nombre s'il est deja présent
                break;
            }
        }
    }
    //Tri à bulle du tableau (pas besoin d'être très efficace et on veut un tri en place)
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
   
    
    return array;
}
