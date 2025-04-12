#include "macro.h"
#include "gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game.h"
#include "player.h"
#include <time.h>

Board *g_board;

int main() {
    srand(time(NULL)); // nouvelle graine
    
    
    Game g = game_make(TO_LAUNCH);
    Player *p = player_start();
    Board *b;
    g_board = NULL;

    
    // initialisation du plateau

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
    int const width = 1500;
    int const height = 1000;

    // Variables de la caméra
    Camera cam = {1.25f, 150-400, -350-65, 0.25f};

    // Variables de la grille
    Display_informations display = {121, 14, 50, 1, 25};
    
    p->grid_cell_width=display.tile_size;
    // Couleurs de Crossy_road
    Colors colors = {
        // Les sols
        .GRASS_COLOR_DARK = {181, 236, 93, SDL_ALPHA_OPAQUE},
        .GRASS_COLOR_BRIGHT = {189, 244, 101, SDL_ALPHA_OPAQUE},
        .GRASS_COLOR_SHADOW = {134, 185, 58, SDL_ALPHA_OPAQUE},
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

    game_start(&g, &sprite_sheet);
    b = g.board;
    g_board = b;

    p->h_position=1000;
    board_set_player(b, p);
    

    SDL_Event event;
    
    
    while (g.status==PLAYING){
        
        if (!p->alive) {
            g.status = DEAD;
            break; // Exit the game loop immediately when player is dead
        }

        int direction = NEUTRAL;
        // 1. Action du joueur           
        if (SDL_PollEvent(&event)){ 
            switch (event.type){
                
                case SDL_QUIT:
                    g.status=DEAD;
                    break;
                case SDL_KEYDOWN: // touche pressée
                    if (event.key.keysym.sym==SDLK_q){
                        g.status=DEAD;
                    }
                    if (event.key.keysym.sym==SDLK_RIGHT){
                        direction = RIGHT;

                    }
                    if (event.key.keysym.sym==SDLK_LEFT){
                        direction = LEFT;
                       
                    }
                    if (event.key.keysym.sym==SDLK_UP){
                        direction = UP;
                    }
                    if (event.key.keysym.sym==SDLK_DOWN){
                        if (jump_back < 3){
                            direction = DOWN;
                            //ground_move(b,DOWN, &sprite_sheet);

                        }
                        
                    }

                default:
                    break;
            }
        }
        // 2. Traiter action
        int collision_type = check_future_collision(b, direction);
        //printf("COLLISION UP : %d\n",collision_type);
        switch (collision_type) {
            case COLLIDE_NONE:
                //mouvement autorise
                if (direction== UP) {
                    score_actu++;
                    if (jump_back > 0) {
                        jump_back--;
                    }
                } else if (direction == DOWN) {
                    score_actu--;
                    jump_back++;
                }
            
            
                if (direction != NEUTRAL) {
                    move_player(direction, p);
                    ground_move(b, direction,&sprite_sheet);
                }
                break;
            
            case COLLIDE_DEADLY:
                // Collision mortelle
                p->alive = false;
                g.status = DEAD;
                break;
            
            case COLLIDE_HARMLESS:
                // Collision sans effet, on ne fait rien
                break;
        }




        if (!p->alive) {
            // update si on est mort
        } else {
            
            board_update(b, 1, &sprite_sheet);
        }

        //printf("Position du joueur : h_float = %f \n",p->h_position);
        
        // PROBLEME DE TRAIN A ECRAN
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

        //4. MAJ du score
        if (score_actu > score_maxi) {
            score_maxi = score_actu;
        }
        //5. Maj de etat graphique
        
        draw_board(b,cam,display,colors,renderer,&sprite_sheet);
        
        draw_entities(b,cam,display,colors,renderer,&sprite_sheet);


    
        //draw_chicken(p,&sprite_sheet,renderer,cam,display);
        
        //6. Affichage
        SDL_RenderPresent(renderer);
        SDL_Delay(8); // ~60 FPS  
    }
    IMG_Quit();
    unload_spritesheet(sprite_sheet);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    board_free(b);
}
