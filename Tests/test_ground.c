#include "ground.h"
#include "macro.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "gui.h"

#define EPS 1e-6

int main(void) {
    srand(1);

    SDL_Window *window; // création de la fenêtre
    window = SDL_CreateWindow("SDL2 Window",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                100, 100,
                                SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    Camera cam = {1.25f, -300, -765, 0.25f};
    Sprite_sheet sprite_sheet = load_spritesheet("assets/spritesheet_coord.json", "assets/spritesheet.png", renderer, cam);

    // ground_generate signature updated: pass NULL as sprite_sheet
    Ground *test_ground = ground_generate(GROUND_ROAD_CAR, 0, 2, 2, &sprite_sheet);
    assert(test_ground != NULL);
    // type and special attribute for road cars
    assert(test_ground->type == GROUND_ROAD_CAR);
    assert(test_ground->special_attr == 0);
    // nb_obstacles must be exactly the fixed range [2,2]
    assert(test_ground->nb_obstacles == 2);
    // velocity must be within the car speed bounds
    assert(test_ground->velocity >= CAR_MIN_SPEED - EPS);
    assert(test_ground->velocity <= CAR_MAX_SPEED + EPS);

    // each obstacle must be a car, within the map bounds and valid variant
    for (int i = 0; i < test_ground->nb_obstacles; i++) {
        Obstacle *obs = test_ground->obstacles[i];
        assert(obs->type == CAR_TYPE);
        assert(obs->variant >= 0 && obs->variant < 7);
        assert(obs->h_position >= 0.0f);
        assert(obs->h_position < MAP_WIDTH * DEFAULT_CELL_SIZE);
    }

    ground_free(test_ground);


    printf("La librairie ground passe tous les tests\n");

    unload_spritesheet(sprite_sheet);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return 0;
}