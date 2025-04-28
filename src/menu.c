#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../include/menu.h"
#include "macro.h"

void render_button(Button *button, SDL_Renderer *renderer)
{
    if (button->sprite_id >= button->menu_spritesheet->sprites_nb)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Sprite id invalid: %d", button->sprite_index);
        exit(-1);
    }
    if (button->sprite_index >= button->menu_spritesheet->sprites[button->sprite_id].sprite_count)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Sprite index invalid: %d", button->sprite_index);
        exit(-1);
    }    

    // Les infomations sont bonnes, on récupère le sprite
    int sprite_index = button->sprite_index;
    int sprite_id = button->sprite_id;
    Point2d p2d = {button->x, button->y};

    Sprite sprite = button->menu_spritesheet->sprites[sprite_id];
    
    // Si on en a la possibilité, on choisit la variante appuyée du bouton
    if (button->state == 1 && sprite.sprite_count > 1)
    {
        sprite_index = 1;
    }
    else
    {
        sprite_index = 0;
    }

    // On réutilise l'affichage de sprite dans gui.c
    SDL_Rect src_rect = {sprite.sprites_coord[sprite_index].x, sprite.sprites_coord[sprite_index].y, sprite.sprites_coord[sprite_index].w, sprite.sprites_coord[sprite_index].h};
    SDL_Rect dst_rect = {p2d.x, p2d.y - sprite.sprites_coord[sprite_index].h, sprite.sprites_coord[sprite_index].w, sprite.sprites_coord[sprite_index].h};

    // On vérifie que tout se passe bien pour l'écriture
    if (SDL_RenderCopy(renderer, button->menu_spritesheet->sprite_sheet, &src_rect, &dst_rect) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to render copy: %s", SDL_GetError());
        exit(-1);
    }
}
