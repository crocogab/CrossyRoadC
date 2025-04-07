#include "ground.h"
#include "macro.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main() {

    srand(1);
    Ground *test_ground = ground_generate(GROUND_ROAD_CAR, 0, 2, 2);
    assert(test_ground->velocity < CAR_MAX_SPEED);
    assert(test_ground->velocity >= CAR_MIN_SPEED);
    assert(test_ground->nb_obstacles <= MAP_WIDTH/INTER_CAR_MIN);
    for (int i = 0; i < test_ground->nb_obstacles; i++)
    {
        //printf("Car nb %d  |  h_pos : %f\n", i, test_ground->obstacles[i]->h_position);
        assert(test_ground->obstacles[i]->h_position >= 0);
        assert(test_ground->obstacles[i]->h_position < MAP_WIDTH);
    }

    ground_free(test_ground);
    

    return 0;
}
