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
#include "ia.h"
#include "UI.h" 


debugKit debug;

int main() {
    srand(time(NULL)); // nouvelle graine
    
    //Initialisation des informations pour le debug
    debug.display_grid_lines=0;
    debug.game_speed=1;
    debug.god_mode=0;
    debug.display_sprites=0;
    debug.display_information=0;
    debug.display_information_sprites=0;
    debug.pouleria = 0;
    debug.deepness_ia = 5;
    
    //Initialisation des objets
    Game g = game_make(TO_LAUNCH);
    Player *p = player_start();
    Board *b;
   
    
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

    // Inisitialisation de la partie texte de SDL
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Creation de la fenetre
    int const width = 1500;
    int const height = 1000;

    // Variables de la caméra
    Camera cam = {1.25f, -300, -765, 0.25f};

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
        .WATER_SHADOW_COLOR = {86, 191, 255, SDL_ALPHA_OPAQUE},
        .ROAD_COLOR = {82, 88, 100, SDL_ALPHA_OPAQUE},
        .ROAD_BORDER_COLOR = {38, 40, 53, SDL_ALPHA_OPAQUE},
        .ROAD_LINE_COLOR = {110, 118, 141, SDL_ALPHA_OPAQUE}
    };


    SDL_Window *window; // MARK: création de la fenêtre
    window = SDL_CreateWindow("SDL2 Window",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                width, height,
                                SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window: %s", SDL_GetError());
        exit(-1);
    }
    
    // Création du renderer
    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create renderer: %s", SDL_GetError());
        exit(-1);
    }

    // Initialisation des polices d'écriture du jeu
    TTF_Font *debug_font = font_load("assets/editundo.ttf", 20);
    TTF_Font *score_fond = font_load("assets/editundo.ttf", 100);

    SDL_Color white = {255, 255, 255, SDL_ALPHA_OPAQUE};

    // On charge la sprite_sheet
    Sprite_sheet sprite_sheet = load_spritesheet("assets/spritesheet_coord.json", "assets/spritesheet.png", renderer, cam);

    // On change certains aspects pour le début de la partie
    game_start(&g, &sprite_sheet);
    b = g.board;
    grid_ground_starter_set(b,&sprite_sheet);
    
    board_set_player(b, p);

    // Paramétrage des différentes animations de saut (interpolation de polynômes du second degré)
    float duration = 13.0; // Durée d'un saut
    float jump_height = 0.5; // Hauteur d'un saut

    float b_x = 1.0/duration;

    float a_z = -(4*jump_height)/(duration*duration);
    float b_z = (4*jump_height)/duration;

    Animation anim_jump_x = {.duration = duration, .a = 0, .b = b_x, .c = -1}; // Le déplacement au sol est linéaire

    Animation anim_jump_z = {.duration = duration, .a = a_z, .b = b_z, .c = 0}; // La courbe de saut est quadratique
    // Initialisation du compteur pour les frames des animations
    float anim_time = 0;
    
    // Initialisation de l'écoute des événements
    SDL_Event event;
    
    // Début de la boucle d'action
    while (g.status==PLAYING){
        
        if (!p->alive) {
            g.status = DEAD;
            break; // Fin de la partie si le joueur meurt
        }

        
        //MARK: Action du joueur  
        int direction = NEUTRAL;
        int ai_direction = NEUTRAL;
        int player_direction = NEUTRAL;



        if (SDL_PollEvent(&event)){ 
            {
                switch (event.type){
                    
                    case SDL_QUIT:
                        g.status=DEAD;
                        break;

                    case SDL_KEYUP: //touche relachée pour les mouvements
            
                        if (!(p->is_jumping)) {
                            if (event.key.keysym.sym == SDLK_RIGHT) {
                                player_direction = RIGHT;
                            }
                            if (event.key.keysym.sym == SDLK_LEFT) {
                                player_direction = LEFT;
                            }
                            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE) {
                                player_direction = UP;
                            }
                            if (event.key.keysym.sym == SDLK_DOWN) {
                                if (jump_back < 3) {
                                    player_direction = DOWN;
                                }
                            }
                        }
                        break;

                    case SDL_KEYDOWN: // touche pressée
                        if (event.key.keysym.sym==SDLK_q){
                            g.status=DEAD;
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
                        if (event.key.keysym.sym==SDLK_F8) {
                            debug.pouleria = debug.pouleria ? 0 : 1;
                        }
                        break;

                    default:
                        break;
                }
            }
        }
        if (debug.pouleria && (!(p->is_jumping))) {
            
            ai_direction = pouleria_zero(g.board, duration, debug.deepness_ia);
                
            direction = ai_direction;
            
        } else {
            // Le joueur contrôle le jeu
            direction = player_direction;
        }

        // 2. Traiter action
        int collision_type;
        if (!debug.god_mode){
            collision_type= check_future_collision(b, direction);
        }else{
            collision_type=COLLIDE_NONE;
        }
        
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
                    move_player(direction, p,b->grid_ground[V_POS-1],b->grid_ground[V_POS]);
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
            //Enregistremeent du score
            TTF_Font *font = font_load("assets/editundo.ttf", 48);
            char letters[4] = {'A', 'A', 'A', '\0'};
            int selected_letter = 0;
            SDL_bool running = SDL_TRUE;
            // Boucle d'événements pour la saisie du pseudo
            while (running) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT)
                        running = SDL_FALSE;
                    else if (event.type == SDL_KEYDOWN) {
                        switch (event.key.keysym.sym) {
                            case SDLK_RIGHT:
                                selected_letter = (selected_letter + 1) % 3;
                                break;
                            case SDLK_LEFT:
                                selected_letter = (selected_letter + 2) % 3;
                                break;
                            case SDLK_UP:
                                letters[selected_letter] = (letters[selected_letter] == 'Z') ? 'A' : letters[selected_letter] + 1;
                                break;
                            case SDLK_DOWN:
                                letters[selected_letter] = (letters[selected_letter] == 'A') ? 'Z' : letters[selected_letter] - 1;
                                break;
                            case SDLK_RETURN:
                                printf("Pseudo validé : %c%c%c\n", letters[0], letters[1], letters[2]);
                                save_high_score(letters, p->score);
                                running = SDL_FALSE;
                                break;
                        }
                    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                        int x = event.button.x;
                        int y = event.button.y;
                        if (x >= 650 && x <= 850 && y >= 300 && y <= 345) {
                            printf("Pseudo validé par clic : %c%c%c\n", letters[0], letters[1], letters[2]);
                            save_high_score(letters, p->score);
                            running = SDL_FALSE;
                        }
                    }
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
                SDL_RenderClear(renderer);

                // Dessin des 3 lettres
                for (int i = 0; i < 3; i++) {
                    draw_letter(renderer, font, letters[i], 650 + i * 80, 200, i == selected_letter);
                }
                // Bouton "Valider"
                draw_button(renderer, font, "Valider", 710, 300, 80, 50);

                // YOUR SCORE
                draw_text(renderer, font, "YOUR SCORE :", 550, 100, (SDL_Color){255, 255, 255});    
                char score_buf[16];
                sprintf(score_buf, "%d", p->score);
                draw_text(renderer, font, score_buf, 920, 100, (SDL_Color){255, 255, 255});

                // LEADERBOARD
                char top_names[10][4];
                int top_scores[10];
                int top_count = load_top_scores_jsonc(FILE_NAME_SCORE, top_names, top_scores);

                draw_text(renderer, font, "LEADERBOARD", 580, 420, (SDL_Color){255, 255, 255});
                for (int i = 0; i < top_count; i++) {
                    char line[64];
                    sprintf(line, "%d. %s - %d", i + 1, top_names[i], top_scores[i]);
                    draw_text(renderer, font, line, 580, 490 + i * 45, (SDL_Color){255, 255, 255});
                }

                SDL_RenderPresent(renderer);
                SDL_Delay(16); // ~60 FPS
                
            }
        } else {
            
            board_update(b, debug.game_speed, &sprite_sheet,debug);
        }
        
        // On efface l'écran et on nettoie
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
        // Dessin des sols du plateau
        draw_board(b,anim_time, anim_jump_x,cam,display,colors,renderer,&sprite_sheet, &debug,score_actu);
        
        // Gestion des animations
        if (p->is_jumping)
        {
            anim_time += debug.game_speed; // Avancée de l'animation
        }
        
        // Dessin des entités en prenant en compte le temps d'animation s'il y a une animation en cours
        // si le temps d'animation est dépassé, draw_entities renvoie 0 et on met fin à l'état d'animation
        if (draw_entities(b, anim_time, anim_jump_x, anim_jump_z, cam,display,renderer,&sprite_sheet, &debug) == 0)
        {
            anim_time = 0;
            p->is_jumping = 0;
        }
        else
        {
            p->is_jumping = 1;
            anim_time += 0.01;
        }
        
        // Affichage des menus de debug si activés
        if (debug.display_information || debug.display_information_sprites){
            game_debug(&g, debug_font, renderer, cam, &debug);
        }else{
            char score_text[32]; // score pas censé depasser 32 chiffres
            sprintf(score_text, "%d", p->score);
            write_text(score_text, 50,30 , -1, 0, 10, white, renderer, score_fond);
        }
        
        //6. Affichage
        SDL_RenderPresent(renderer);
        SDL_Delay(8); // ~60 FPS  
    }
    


    
    

    TTF_CloseFont(debug_font);
    TTF_CloseFont(score_fond);
    IMG_Quit();
    unload_spritesheet(sprite_sheet);
    board_free(b);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
