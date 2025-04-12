#include "macro.h"
#include "gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.h"
#include "player.h"

Board *g_board;

int main() {

    Game g = game_make(TO_LAUNCH);
    Player *p = player_start();
    Board *b;
    g_board = NULL;


    // initialisation du plateau

    game_start(&g);
    b = g.board;
    g_board = b;

    board_set_player(b, p);


    int jump_back = 0;    // Limitation des retours en arrière
    int score_actu = 0;   // Score actuel
    int score_maxi = 0;   // Score maximum
    int quit_game = 0;    // Signal pour quitter le jeu

    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);
    //Il faudra aussi inclure les autres bibliotheques SDL

    // Creation de la fenetre
    int const width = 1000;
    int const height = 800;

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

    SDL_Window *window; // création de la fenêtre
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
    } // render de SDL


    // On charge la sprite_sheet
    Sprite_sheet sprite_sheet = load_spritesheet("assets/spritesheet_coord.json", "assets/spritesheet.png", renderer, cam);

    SDL_Event event;
    int running = 1;
    while (running){
        if (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    running=0;
                    break;
                case SDL_KEYDOWN: // touche pressée
                    draw_board_line(0,GROUND_GRASS,cam,display,colors,renderer);

            }
        }

    }
}
