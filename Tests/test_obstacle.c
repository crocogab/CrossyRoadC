#include "obstacle.h"
#include "macro.h"
#include <assert.h>

int main (void) {

    Obstacle *voiture = obstacle_make(TYPE_VOITURE, 'i', 0, 2, 2.3);

    for (int i = 0; i<100; i++) {obstacle_update(voiture, 0.02);}

    assert (voiture->type == TYPE_VOITURE);

    
    obstacle_free(voiture);

    return 0;
}