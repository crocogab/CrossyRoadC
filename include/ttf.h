#ifndef TTF_H
#define TTF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

TTF_Font* font_load(const char* font_path, int taille);

void font_unload(TTF_Font *font);

void write_text(const char* texte, int x, int y, float shadow_angle, float shadow_length, float outline_size, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font);

#endif
