#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "gui.h"
#include "ground.h"
#include "debugKit.h"

typedef struct _Player {
    // Skin *skin;
    bool alive;
    int direction; //macro
    float h_position;
    bool is_jumping;
    float grid_cell_width;
    int score;
    int previous_direction;

} Player;

Player *player_make(float grid_cell_size, float h_position, int direction, bool alive, bool isJumping, int score);

Player *player_start(void);

void player_free(Player *player);

void move_player(int direction, Player *player,Ground * next_ground, Ground * ground_actu);

void draw_chicken(Player *p, Sprite_sheet *sprite_sheet, SDL_Renderer *renderer, Camera cam, Display_informations display, debugKit *debug_kit);

int draw_chicken_anim(Player *p, float anim_time, Animation anim_x, Animation anim_z, Sprite_sheet *sprite_sheet, SDL_Renderer *renderer, Camera cam, Display_informations display, debugKit *debug_kit);

#endif
