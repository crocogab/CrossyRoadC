#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "debugKit.h"

// Seules les fonctions bien commentées de cette librairie seront utiles pour le GUI final

/**
 * Définit les informations de l'affichage
 * @param tile_size taille d'une case (en pixels, les cases sont carrées)
 * @param border_gap espace en hauteur entre les cases (en pixels) herbe -> route -> eau
 * @param line_length longueur d'une ligne (en nombre de cases)
 * @param line_width largeur d'une ligne (en nombre de cases)
 * @param board_length longueur du tableau (en nombre de lignes)
 */
typedef struct display_informations_t
{
    int tile_size;
    int border_gap;
    int line_length;
    int line_width;
    int board_length;
} Display_informations;

/**
 * Définit les informations de la caméra
 * @param orientation orientation de la caméra
 * @param cam_x position de la caméra
 * @param cam_y position de la caméra
 * @param rotation rotation de la caméra
 */
typedef struct camera_t
{
    float orientation;
    int cam_x;
    int cam_y;
    float rotation;
} Camera;


/**
 * Définit les couleurs de Crossy_road
 * @param GRASS_COLOR_DARK couleur de l'herbe sombre
 * @param GRASS_COLOR_BRIGHT couleur de l'herbe claire
 * @param GRASS_BORDER_COLOR couleur de la bordure de l'herbe
 * @param DIRT_COLOR couleur de la terre
 * @param WATER_COLOR couleur de l'eau
 * @param ROAD_COLOR couleur de la route
 * @param ROAD_BORDER_COLOR couleur de la bordure de la route
 */
typedef struct Colors_t
{
    SDL_Color GRASS_COLOR_DARK;
    SDL_Color GRASS_COLOR_BRIGHT;
    SDL_Color GRASS_COLOR_SHADOW;
    SDL_Color GRASS_BORDER_COLOR;
    SDL_Color DIRT_COLOR;
    SDL_Color WATER_COLOR;
    SDL_Color WATER_SHADOW_COLOR;
    SDL_Color ROAD_COLOR;
    SDL_Color ROAD_BORDER_COLOR;
    SDL_Color ROAD_LINE_COLOR;
} Colors;

/**
 * Définit un sprites et ses position dans la feuille associée
 * @param x Position x du sprite dans la feuille (coin haut gauche)
 * @param y Position y du sprite dans la feuille (coin haut gauche)
 * @param w Largeur du sprite dans la feuille
 * @param h Hauteur du sprite dans la feuille
 * 
 */
typedef struct sprite_coord_t
{
    int x;
    int y;
    int w;
    int h;
} Sprite_coord;

/**
 * Définit un sprite et ses variantes à partir de la sprite_sheet
 * @param sprites les coordonées des variantes sur la sprite sheet
 * @param sprite_count le nombre de variantes sur la sprite sheet
 * @param sprite_sheet la texture contenant la sprite_sheet
 * 
 */
typedef struct Sprite_t
{
    Sprite_coord sprites_coord[5];
    int sprite_count;
    float lenght;
} Sprite;

/**
 * Définit une sprite_sheet
 * @param sprites liste des sprites sur la feuille
 * @param sprites_nb nombre de sprites sur la feuille
 * @param path chemin vers le fichier de la feuille
 * 
 */
typedef struct Sprite_sheet_t
{
    Sprite *sprites;
    int sprites_nb;
    SDL_Texture *sprite_sheet;
} Sprite_sheet;

/**
 * Définit des points 3D, simple structure de coordonées
 * @param x coordonnée x
 * @param y coordonnée y
 * @param z coordonnée z
 * 
 */
typedef struct point3d_t
{
    float x;
    float y;
    float z;
} Point3d;

/**
 * Définit des points 2D, simple structure de coordonées
 * @param x coordonnée x
 * @param y coordonnée y
 * 
 */
typedef struct point2d_t
{
    float x;
    float y;
} Point2d;

/**
 * Définit une animation en ax² + bx + c
 * @param duration la durée de l'animation en unité de temps
 * @param a le coefficient de degré 2
 * @param b le coefficient de degré 1
 * @param c le coefficient de degré 0
 * 
 */
typedef struct Animation_t
{
    float duration;
    float a;
    float b;
    float c;
} Animation;

Point2d d3_to_2d(float x, float y, float z, Camera cam);
void draw_quad(Point2d p1, Point2d p2, Point2d p3, Point2d p4, SDL_Color color, SDL_Renderer *renderer);
void draw_quad_from_3d(Point3d p1, Point3d p2, Point3d p3, Point3d p4, SDL_Color color, Camera cam, SDL_Renderer *renderer);
void draw_line(Point2d p1, Point2d p2, SDL_Color color, SDL_Renderer *renderer);
void draw_line_from_3d(Point3d p1, Point3d p2, SDL_Color color, Camera cam, SDL_Renderer *renderer);
void draw_board_line(float x, int type, Camera cam, Display_informations display, Colors colors, SDL_Renderer *renderer, Sprite_sheet *sprite_sheet, debugKit *debug_kit,int score_actu, int is_jumping, int p_direction);

Sprite create_sprite(Sprite_coord *sprites_coord, int sprite_count, float lenght);
Sprite_sheet load_spritesheet(char *coord_path, char *sheet_path, SDL_Renderer *renderer, Camera cam);

void unload_spritesheet(Sprite_sheet sprite_sheet);

void draw_sprite(Point3d p, int sprite_id, int sprite_index, Sprite_sheet *sprite_sheet, SDL_Renderer *renderer, Camera cam, debugKit *debug_kit);
void draw_sprite_from_grid(float h_pos, float y, float z, int sprite_id, int sprite_index, Sprite_sheet *sprite_sheet, SDL_Renderer *renderer, Camera cam, Display_informations display, debugKit *debug_kit);

float animation_calc(Animation anim, float t);

#endif
