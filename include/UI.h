#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "gui.h"

/**
 * Définit un bouton, l'affichage se fait depuis une sprite_sheet
 * @param button_id id du bouton
 * @param x position x du bouton
 * @param y position y du bouton
 * @param w largeur du bouton
 * @param h hauteur du bouton
 * @param state état du bouton (0: off, 1: on)
 * @param menu_spritesheet la sprite_sheet associée
 * @param sprite_index l'index du sprite à afficher
 * @param sprite_id l'id du sprite à afficher
 * 
 */
typedef struct Button_t{
    int button_id;
    int x;
    int y;
    int w;
    int h;
    int state; // 0: off, 1: on
    Sprite_sheet *menu_spritesheet;
    int sprite_index;
    int sprite_id;
} Button;

void render_button(Button *button, SDL_Renderer *renderer);

Sprite_sheet load_ui_spritesheet(char *coord_path, char *sheet_path, SDL_Renderer *renderer, Camera cam);

void unload_ui_spritesheet(Sprite_sheet sprite_sheet);

#endif
