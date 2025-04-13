#ifndef BOARD_H
#define BOARD_H

#include "obstacle.h"
#include "player.h"
#include "ground.h"
#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


typedef struct _Board {
    Player *player; // pointeur vers le joueur
    Ground **grid_ground;  
} Board;

Board* board_make(Sprite_sheet *sprite_sheet);
void board_free(Board* b);
void board_set_ground(Board* b, Ground** ground);
void board_set_player(Board* b, Player* player);
void board_update(Board* b, float delta_t, Sprite_sheet *sprite_sheet);
int check_future_collision(Board *, int direction);
void player_move (Player* p, int direction, float delta_t);
void ground_move (Board* b, int direction, Sprite_sheet *sprite_sheet);
char **grid_tui_make(Board *);
void grid_ground_starter_set(Board *b, Sprite_sheet *sprite_sheet);
void grid_tui_free(char **g);
Ground **grid_ground_make(Sprite_sheet *sprite_sheet);
void grid_ground_free(Ground **);
Ground *gen_next_ground(Board *b, int score, Sprite_sheet *sprite_sheet);

void draw_board(Board *b, Camera cam, Display_informations display, Colors colors, SDL_Renderer *renderer, Sprite_sheet *sprite_sheet, debugKit *debug_kit);
void draw_entities(Board *b, Camera cam, Display_informations display, Colors colors, SDL_Renderer *renderer, Sprite_sheet *sprite_sheet, debugKit *debug_kit);

#endif

