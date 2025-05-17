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

    Menu *all_menus = malloc(sizeof(Menu) * 10); // Allocation d'un tableau de 10 menus (on ne fera pas plus)
    // Création du menu d'accueil
    all_menus[0] = create_menu(0, 1);
    Button game_title = create_button(0, 482, 131, 0, 0, &menu_spritesheet, TITLE_ID);
    Button keyboard_controls = create_button(1, 625, 886, 1, 0, &menu_spritesheet, CONTROLS_ID);
    Button right_menu = create_button(2, 1761, 922, 0, 0, &menu_spritesheet, SETTINGS_ID);
    Button skins_menu = create_button(3, 17, 922, 0, 0, &menu_spritesheet, SKINS_ID);
    add_button_to_menu(&all_menus[0], game_title);
    add_button_to_menu(&all_menus[0], keyboard_controls);
    add_button_to_menu(&all_menus[0], right_menu);
    add_button_to_menu(&all_menus[0], skins_menu);

    // Création du menu de settings
    all_menus[1] = create_menu(1, 1);
    Button sound_button = create_button(4, 1761, 772, 0, 0, &menu_spritesheet, SOUND_ID);
    Button android_button = create_button(5, 1761, 622, 0, 0, &menu_spritesheet, ANDROID_ID);
    add_button_to_menu(&all_menus[1], sound_button);
    add_button_to_menu(&all_menus[1], android_button);

    // Création du menu de fin de partie
    all_menus[2] = create_menu(2, 0);
    Button play_button = create_button(7, 625, 886, 1, 0, &menu_spritesheet, PLAY_ID);
    add_button_to_menu(&all_menus[2], play_button);

    // Création du menu in game
    all_menus[3] = create_menu(3, 0);
    Button pause_button = create_button(8, 482, 131, 0, 0, &menu_spritesheet, PAUSE_ID);
    add_button_to_menu(&all_menus[3], pause_button);

    // Création du menu android
    all_menus[4] = create_menu(4, 0);
    Button return_button = create_button(9, 482, 131, 0, 0, &menu_spritesheet, BACK_ID);
    Button android_button2 = create_button(10, 625, 886, 0, 0, &menu_spritesheet, ANDROID_ID);
    add_button_to_menu(&all_menus[4], return_button);
    add_button_to_menu(&all_menus[4], android_button2);


    SDL_Event event;
    int running = 1;

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
                    click_button(x, y, all_menus, 1); // On ne gère pas les menus pour l'instant
                    printf("Button %d state: %d\n", all_menus[0].buttons[0].button_id, all_menus[0].buttons[0].state);
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

        render_menus(all_menus, all_menus->nb, renderer); // On ne gère pas les menus pour l'instant

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

