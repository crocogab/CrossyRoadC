#include "obstacle.h"
#include "macro.h"
#include <assert.h>
#include <math.h>


int main (void) {

    Obstacle *voiture = obstacle_make(TYPE_VOITURE, 'i', 0, 2);

    assert (voiture->type == TYPE_VOITURE); // obstacle make initialise mal le type
    for (int i = 0; i<100; i++) {obstacle_update(voiture, 1.0 / 50 , 2);}

    assert(fabsf(voiture->h_position - 4) < 0.1); // pas le bon resultat apres quelques obstacle_update
    assert(fabsf(voiture->h_position - 4) < 0.00001); // erreur de float avec obstacle_update répété
    
    obstacle_free(voiture);

    return 0;
}