#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "random_custom.h"

int main() {
    srand(1);

    //Random_int
    for (int i = 0; i < 10; i++)
    {
        assert(random_int(0, 10) >= 0 && random_int(0, 10) <= 10); // Un des nombres int n'est pas dans l'intervalle
    }
    assert(random_int(3, 3) == 3); //random_int ne renvoie pas le bon nombre

    //Random_float
    for (int i = 0; i < 10; i++)
    {
        assert(random_float(0, 10) >= 0 && random_float(0, 10) < 10); // Un des nombres float n'est pas dans l'intervalle
    }

    //Random_int_array
    int *array = random_int_array(0, 20, 5);
    assert(array != NULL); //Le tableau n'a pas été alloué correctement
    for (int i = 0; i < 5; i++)
    {
        assert(array[i] >= 0 && array[i] <= 20); // Un des nombres n'est pas dans l'intervalle
    }
    for (int i = 0; i < 4; i++)
    {
        assert(array[i] != array[i+1]); // Le tableau contient des doublons
        assert(array[i] < array[i+1]); // Le tableau n'est pas trié
    }
    free(array);
    printf("La librairie random passe tout les tests\n");
    
}
