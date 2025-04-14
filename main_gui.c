#include "macro.h"
#include "gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "player.h"
#include <time.h>
#include "ground.h"
#include "ttf.h"
#include "debugKit.h"

Board *g_board;

debugKit debug;

int main() {
    srand(time(NULL)); // nouvelle graine
    
    debug.display_grid_lines=0;
    debug.game_speed=1;
    debug.god_mode=0;
    debug.display_sprites=0;
    debug.display_information=0;
    debug.display_information_sprites=0;
    
    Game g = game_make(TO_LAUNCH);
    Player *p = player_start();
    Board *b;
    g_board = NULL;

    
    // initialisation du plateau

    int jump_back = 0;    // Limitation des retours en arrière
    int score_actu = 0;   // Score actuel
    int quit_game = 0;    // Signal pour quitter le jeu

    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);

    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Creation de la fenetre
    int const width = 1500;
    int const height = 1000;

    // Variables de la caméra
    Camera cam = {1.25f, 100-400, -700-65, 0.25f};

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

    TTF_Font *debug_font = font_load("assets/editundo.ttf", 20);
    TTF_Font *score_fond = font_load("assets/editundo.ttf", 100);

    SDL_Color white = {255, 255, 255, SDL_ALPHA_OPAQUE};

    // On charge la sprite_sheet
    Sprite_sheet sprite_sheet = load_spritesheet("assets/spritesheet_coord.json", "assets/spritesheet.png", renderer, cam);

    game_start(&g, &sprite_sheet);
    b = g.board;
    grid_ground_starter_set(b,&sprite_sheet);
    g_board = b;

   
    board_set_player(b, p);

    float duration = 15.0;
    float jump_height = 0.5;

    float b_x = 1.0/duration;

    float a_z = -(4*jump_height)/(duration*duration);
    float b_z = (4*jump_height)/duration;

    Animation anim_jump_x = {.duration = duration, .a = 0, .b = b_x, .c = -1};

    Animation anim_jump_z = {.duration = duration, .a = a_z, .b = b_z, .c = 0};

    float anim_time = 0;
    

    SDL_Event event;
    
    
    while (g.status==PLAYING){
        
        if (!p->alive) {
            g.status = DEAD;
            break; // Exit the game loop immediately when player is dead
        }

        int direction = NEUTRAL;
        // 1. Action du joueur           
        if (SDL_PollEvent(&event)){ 
            if (!(p->is_jumping))
            {
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
                        // DEBUG MODE
                        if (event.key.keysym.sym==SDLK_F1){
                            debug.god_mode=debug.god_mode ? 0 : 1;
                        }

                        // ralentir temps = f5 et accelerer = f6
                        if (event.key.keysym.sym==SDLK_F5){
                            debug.game_speed = debug.game_speed/2;
                        }
                        if (event.key.keysym.sym==SDLK_F6){
                            debug.game_speed = debug.game_speed*2;
                        }

                        if (event.key.keysym.sym==SDLK_F2){ // ACTIVE OU PAS LES LIGNES
                            debug.display_grid_lines= debug.display_grid_lines ? 0 : 1;
                        }

                        if (event.key.keysym.sym==SDLK_F4){ // ACTIVE OU PAS LES sprites
                            debug.display_sprites= debug.display_sprites ? 0 : 1;
                        }
                        if (event.key.keysym.sym==SDLK_F3){ // ACTIVE OU PAS le debug
                            debug.display_information= debug.display_information ? 0 : 1;
                        }
                        if (event.key.keysym.sym==SDLK_F7){
                            debug.display_information_sprites = debug.display_information_sprites ? 0 : 1;
                        }


                    default:
                        break;
                }
            }
        }
        // 2. Traiter action
        int collision_type;
        if (!debug.god_mode){
            collision_type= check_future_collision(b, direction);
        }else{
            collision_type=COLLIDE_NONE;
        }
        
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
                    // On déplace le joueur
                    move_player(direction, p,b->grid_ground[V_POS-1]);
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
            
            board_update(b, debug.game_speed, &sprite_sheet,debug);
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
        if (score_actu > p->score) {
            p->score = score_actu;
        }
        //5. Maj de etat graphique
        
        draw_board(b,anim_time, anim_jump_x,cam,display,colors,renderer,&sprite_sheet, &debug,score_actu);
        
        // Gestion des animations
        printf("anim_time : %f\n", anim_time);
        if (p->is_jumping)
        {
            printf("is_juming = true\n");
            printf("anim_cal : %f | anim.b = %f\n", animation_calc(anim_jump_x, anim_time), anim_jump_x.b);
        }

        if (p->is_jumping)
        {
            anim_time += debug.game_speed;
        }
        
        if (draw_entities(b, anim_time, anim_jump_x, anim_jump_z, cam,display,colors,renderer,&sprite_sheet, &debug) == 0)
        {
            anim_time = 0;
            p->is_jumping = 0;
        }
        else
        {
            p->is_jumping = 1;
            anim_time += 0.01;
        }
        
        if (debug.display_information || debug.display_information_sprites){
            game_debug(&g, debug_font, renderer, cam, &debug);
        }else{
            char score_text[32]; // score pas censé depasser 32 chiffres
            sprintf(score_text, "%d", p->score);
            write_text(score_text, 50,30 , -1, 0, 10, white, renderer, score_fond);
        }
        
    
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

    SDL_Quit();
}
