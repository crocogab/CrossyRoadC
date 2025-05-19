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
    int const width = 1920;
    int const height = 1080;

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

    Menu *all_menus = init_menus(&menu_spritesheet); // Allocation d'un tableau de 10 menus (on ne fera pas plus)

    // Ouverture du menu initial
    toggle_menu_active(&all_menus[3]);

    SDL_Event event;
    int running = 1;
    int game_state = TO_LAUNCH;
    int game_click_state = -1;

    while (running)
    {
        if(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    printf("Mouse clicked at (%d, %d)\n", x, y);
                    game_click_state = click_button(x, y, all_menus, 6); // On ne gère pas les menus pour l'instant
                    if (game_click_state != -1)
                    {
                        game_state = game_click_state;
                    }
                    printf("Game state: %d\n", game_state);
                }
                break;
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

        render_menus(all_menus, 6, renderer); // On ne gère pas les menus pour l'instant

        // Switch framebuffer
        SDL_RenderPresent(renderer);

        // Delay to control frame rate
        SDL_Delay(8); // ~60 FPS   
    }
    
    IMG_Quit();

    unload_ui_spritesheet(menu_spritesheet);
    destroy_menus(all_menus, 6);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);    

}

