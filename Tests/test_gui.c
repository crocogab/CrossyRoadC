#include "macro.h"
#include "gui.h"
#include "board.h"
#include "ground.h"
#include "obstacle.h"
#include "random_custom.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main() {
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);
    //Il faudra aussi inclure les autres bibliotheques SDL

    // Creation de la fenetre
    int const width = 1400;
    int const height = 1100;

    // Variables de la caméra
    Camera cam = {1.25f, 150, -65, 0.25f};

    // Variables de la grille
    Display_informations display = {121, 14, 50, 1, 25};

    // Couleurs de Crossy_road
    Colors colors = {
        // Les sols
        .GRASS_COLOR_DARK = {181, 236, 93, SDL_ALPHA_OPAQUE},
        .GRASS_COLOR_BRIGHT = {189, 244, 101, SDL_ALPHA_OPAQUE},
        .GRASS_BORDER_COLOR = {86, 110, 50, SDL_ALPHA_OPAQUE},
        .DIRT_COLOR = {100, 56, 71, SDL_ALPHA_OPAQUE},
        .WATER_COLOR = {130, 244, 255, SDL_ALPHA_OPAQUE},
        .ROAD_COLOR = {82, 88, 100, SDL_ALPHA_OPAQUE},
        .ROAD_BORDER_COLOR = {38, 40, 53, SDL_ALPHA_OPAQUE},
        .ROAD_LINE_COLOR = {110, 118, 141, SDL_ALPHA_OPAQUE}
    };


    SDL_Window *window;
    window = SDL_CreateWindow("SDL2 Window",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                width, height,
                                SDL_WINDOW_SHOWN);
    
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window: %s", SDL_GetError());
        exit(-1);
    }

    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create renderer: %s", SDL_GetError());
        exit(-1);
    }

    // On charge la sprite_sheet
    Sprite_sheet sprite_sheet = load_spritesheet("assets/spritesheet_coord.json", "assets/spritesheet.png", renderer, cam);

    SDL_Event event;
    int running = 1;

    // On genere un board de test
    Board *test_board = board_make();
    // On genere un joueur de test
    Player *p = player_start();
    test_board->player = p;

    while (running)
    {
        if(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;

            case SDL_KEYDOWN:
            // On utilisera les flèches pour changer l'orientation de la caméra
                if (event.key.keysym.sym == SDLK_s)
                {
                    cam.orientation += 0.05f;
                }
                else if (event.key.keysym.sym == SDLK_z)
                {
                    cam.orientation -= 0.05f;
                }
                else if (event.key.keysym.sym == SDLK_q)
                {
                    cam.rotation += 0.05f;
                }
                else if (event.key.keysym.sym == SDLK_d)
                {
                    cam.rotation -= 0.05f;
                }
                else if (event.key.keysym.sym == SDLK_UP)
                {
                    cam.cam_y += 10;
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    cam.cam_y -= 10;
                }
                else if (event.key.keysym.sym == SDLK_LEFT)
                {
                    cam.cam_x -= 10;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    cam.cam_x += 10;
                }
                else if (event.key.keysym.sym == SDLK_f)
                {
                    display.tile_size += 1;
                }
                else if (event.key.keysym.sym == SDLK_r)
                {
                    display.tile_size -= 1;
                }
                else if (event.key.keysym.sym == SDLK_g)
                {
                    display.border_gap += 1;
                }
                else if (event.key.keysym.sym == SDLK_t)
                {
                    display.border_gap -= 1;
                }

                else if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = 0;
                }
                
                break;
            
            default:
                break;
            }
        }

        // On efface l'écran
        SDL_Color const BACKGROUND_COLOR = {.r = 0xD0, .g = 0xD0, .b = 0xD0, .a = SDL_ALPHA_OPAQUE};
        if (SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to set render draw color: %s", SDL_GetError());
            exit(-1);
        }

        if (SDL_RenderClear(renderer))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to clear renderer: %s", SDL_GetError());
            exit(-1);
        }

        /*
        // On affiche une grille de test avec tout les types de sol et tout les sprites possibles
        int sprite_reading_grid[15] = {BLUE_CAR_NB, BLUE_TRUCK_NB, CHICKEN_NB, COIN_NB, GAZ_TRUCK_NB, LILYPAD_NB, LOG_NB, ORANGE_CAR_NB, PURPLE_CAR_NB, RAIL_NB, RED_TRUCK_NB, ROCK_NB, TRAIN_NB, TRAIN_POLE_NB, TREE_NB};



        draw_board_line(0, GROUND_GRASS, cam, display, colors, renderer);
        draw_board_line(1, GROUND_WATER_LILY, cam, display, colors, renderer);
        draw_board_line(2, GROUND_ROAD_CAR, cam, display, colors, renderer);

        for (int i = 0; i < 15; i++)
        {
            draw_board_line(i*2+3, GROUND_ROAD_BORDER, cam, display, colors, renderer);
            draw_board_line(i*2+4, GROUND_ROAD_BORDER, cam, display, colors, renderer);
            for (int j = 0; j < sprite_reading_grid[i]; j++)
            {
                draw_sprite_from_grid(j*4*display.tile_size, i*2+3, i, j, &sprite_sheet, renderer, cam, display);
            }
        }
        */

        draw_board(test_board, cam, display, colors, renderer);
        draw_chicken(test_board->player, &sprite_sheet, renderer, cam, display);

        // Switch framebuffer
        SDL_RenderPresent(renderer);

        // Delay to control frame rate
        SDL_Delay(8); // ~60 FPS   
    }

  IMG_Quit();

  board_free(test_board);

  unload_spritesheet(sprite_sheet);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

}
