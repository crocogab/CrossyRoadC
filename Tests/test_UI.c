#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "macro.h"
#include "gui.h"
#include "UI.h"

int main() {

    save_high_score("ABC" , 200);
    save_high_score("DEF" , 100);
    save_high_score("ABC" , 150);
    
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

    Sprite_sheet menu_spritesheet = load_ui_spritesheet("assets/ui_spritesheet_coord.json", "assets/ui_spritesheet.png", renderer, (Camera){0, 0, 0, 0});

    Button menu_button = create_button(0, 200, 200, 0, 0, &menu_spritesheet, SETTINGS_ID);

    SDL_Event event;
    int running = 1;

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
                if (event.key.keysym.sym == SDLK_ESCAPE)
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

        render_button(&menu_button, renderer);

        // Switch framebuffer
        SDL_RenderPresent(renderer);

        // Delay to control frame rate
        SDL_Delay(8); // ~60 FPS   
    }
    
    IMG_Quit();

    unload_ui_spritesheet(menu_spritesheet);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);    

}

