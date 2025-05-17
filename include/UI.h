#ifndef UI_H
#define UI_H
#define FILE_NAME_SCORE "assets/high_scores.json"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAX_ENTRIES 100

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
 * @param is_hidden si le bouton est caché (1: oui, 0: non)
 * @param state état du bouton (0: off, 1: on)
 * @param menu_spritesheet la sprite_sheet associée
 * @param sprite_index l'index du sprite à afficher
 * @param sprite_id l'id du sprite à afficher
 * @param parents_frame_number pointeur vers le numéro de la frame du parent
 * @param menu_activator id du menu activé par le bouton (-1 pour aucun)
 * @param is_slider si le bouton est un titre qui arrive par le côté (1: oui, 0: non)
 * 
 */
typedef struct Button_t{
    int button_id;
    int x;
    int y;
    int w;
    int h;
    int is_alternate; // 1: on, 0: off
    int state; // 0: off, 1: on
    Sprite_sheet *menu_spritesheet;
    int sprite_index;
    int sprite_id;
    int *parents_frame_number;
    int menu_activator;
    int is_slider; // 1: oui, 0: non
} Button;

/**
 * Structure représentant un menu
 * @param id identifiant du menu
 * @param active état du menu (0: inactif, 1: actif)
 * @param buttons tableau de boutons associés au menu
 * @param nb nombre de boutons dans le menu
 * @param frame_number numéro de la frame actuelle (pour les animations)
 * 
 */
typedef struct Menu_t{
    int id;
    int active;
    Button *buttons;
    int nb;
    int frame_number;
} Menu;

Button create_button(int button_id, int x, int y, int is_alternate, int state, Sprite_sheet *menu_spritesheet, int sprite_id, int menu_activator);

Menu create_menu(int id, int active);
void add_button_to_menu(Menu *menu, Button button);
void destroy_menu(Menu *menu);
void click_button(int x, int y, Menu *menus, int nb_menus);
void save_high_score(const char* name, int score);
void render_button(Button *button, SDL_Renderer *renderer);
void render_menu(Menu *menu, SDL_Renderer *renderer);
void toggle_menu_active(Menu *menu);
void render_menus(Menu *menus, int nb_menus, SDL_Renderer *renderer);

Sprite_sheet load_ui_spritesheet(char *coord_path, char *sheet_path, SDL_Renderer *renderer, Camera cam);

void unload_ui_spritesheet(Sprite_sheet sprite_sheet);



void draw_letter(SDL_Renderer *renderer, TTF_Font *font, char letter, int x, int y, int selected);
void draw_button(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, int w, int h);
int load_top_scores_jsonc(const char *filename, char names[10][4], int scores[10]);
void draw_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color);

#endif
