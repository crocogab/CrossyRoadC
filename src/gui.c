#include "gui.h"
#include "macro.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <json-c/json.h>

/**
 * Fonction qui renvoie les coordonées d'un point 3d en 2d suivant la projection isométrique
 * avec une rotation et une orientation de la caméra
 * @param x coordonnée x du point 3d
 * @param y coordonnée y du point 3d
 * @param z coordonnée z du point 3d
 * @param cam caméra
 * 
 * @return point2d coordonées du point 2d
 */
Point2d d3_to_2d(float x, float y, float z, Camera cam)
{
    Point2d p;
    p.x = x - y * cos(cam.orientation);
    p.y = y * sin(cam.orientation) - z;
    p.x += cam.cam_x;
    p.y += cam.cam_y;
    float temp_x = p.x * cos(cam.rotation) - p.y * sin(cam.rotation);
    p.y = p.x * sin(cam.rotation) + p.y * cos(cam.rotation);
    p.x = temp_x;
    return p;
}

/**
 * Fonction qui dessine un quadrilatère
 * @param p1 premier point
 * @param p2 deuxième point
 * @param p3 troisième point
 * @param p4 quatrième point
 * @param color couleur du quadrilatère
 * @param renderer le renderer
 * 
 * @return void
 */
void draw_quad(Point2d p1, Point2d p2, Point2d p3, Point2d p4, SDL_Color color, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Vertex quad[4];
    quad[0].position.x = p1.x;
    quad[0].position.y = p1.y;
    quad[1].position.x = p2.x;
    quad[1].position.y = p2.y;
    quad[2].position.x = p3.x;
    quad[2].position.y = p3.y;
    quad[3].position.x = p4.x;
    quad[3].position.y = p4.y;

    for (int i = 0; i < 4; i++)
    {
        quad[i].color.r = color.r;
        quad[i].color.g = color.g;
        quad[i].color.b = color.b;
        quad[i].color.a = color.a;
    }

    const int indices[6] = {0, 1, 2, 2, 3, 0};

    if (SDL_RenderGeometry(renderer, NULL, quad, 4, indices, 6) == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to render geometry: %s", SDL_GetError());
        exit(-1);
    }
}

/**
 * Fonction qui dessine en 2d iso un quadrilatère en 3d donné en entrée
 * @param p1 premier point
 * @param p2 deuxième point
 * @param p3 troisième point
 * @param p4 quatrième point
 * @param color couleur du quadrilatère
 * @param cam caméra
 * @param renderer le renderer
 *  
 */
void draw_quad_from_3d(Point3d p1, Point3d p2, Point3d p3, Point3d p4, SDL_Color color, Camera cam, SDL_Renderer *renderer)
{
    //Transformation en 2D
    Point2d p2d_1 = d3_to_2d(p1.x, p1.y, p1.z, cam);
    Point2d p2d_2 = d3_to_2d(p2.x, p2.y, p2.z, cam);
    Point2d p2d_3 = d3_to_2d(p3.x, p3.y, p3.z, cam);
    Point2d p2d_4 = d3_to_2d(p4.x, p4.y, p4.z, cam);

    // Dessin du quadrilatère
    draw_quad(p2d_1, p2d_2, p2d_3, p2d_4, color, renderer);
}

/**
 * Fonction qui dessine une ligne de la grille
 * @param y coordonnée y de la ligne (ordonnée sur le plateau)
 * @param type type de la ligne (GRASS, WATER, ROAD, ... cf macro.h)
 * @param cam caméra
 * @param display informations d'affichage
 * @param colors couleurs
 * @param renderer le renderer
 * @param sprite_sheet la sprite sheet (pour le dessins des rails)
 * @param debug_kit le kit de débug
 * @param score_actu le score actuel (pour l'alternance des couleurs de l'herbe)
 * @param is_jumping si le joueur est en train de sauter (pour gérer l'animation du plateau)
 * @param p_direction direction du joueur (pour gérer l'animation du plateau)
 * 
 */
void draw_board_line(float y, int type, Camera cam, Display_informations display, Colors colors, SDL_Renderer *renderer, Sprite_sheet *sprite_sheet, debugKit *debug_kit,int score_actu, int is_jumping, int p_direction)
{
    Point3d p3d_1 = {0, y * (float)(display.line_width*display.tile_size), 0};
    Point3d p3d_2 = {0, (y + 1.0) * (float)(display.line_width*display.tile_size), 0};
    Point3d p3d_3 = {display.line_length*display.tile_size, (y + 1.0) * (float)(display.line_width*display.tile_size), 0};
    Point3d p3d_4 = {display.line_length*display.tile_size, y * (float)(display.line_width*display.tile_size), 0};   
    switch (type)
    {
    case GROUND_GRASS:

        // On pose la zone ombragée
        draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.GRASS_COLOR_SHADOW, cam, renderer);
        
        // Dessin du sol
        p3d_1.x = LEFT_MAP_X;
        p3d_2.x = LEFT_MAP_X;
        p3d_3.x = RIGHT_MAP_X;
        p3d_4.x = RIGHT_MAP_X;
        
        if (((is_jumping*(int)(p_direction == UP)) + score_actu + (int)y)%2)
        {
            draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.GRASS_COLOR_DARK, cam, renderer);
        }
        else
        {
            draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.GRASS_COLOR_BRIGHT, cam, renderer);
        }
        

        // Special debug, on affiche les bordures des cases
        if (debug_kit->display_grid_lines)
        {
            p3d_1.x = LEFT_MAP_X;
            p3d_2.x = LEFT_MAP_X;
            p3d_3.x = LEFT_MAP_X+display.tile_size;
            p3d_4.x = LEFT_MAP_X+display.tile_size;
            for (int i = 0; i < MAP_WIDTH; i++)
            {
                Point3d line_start = p3d_1;
                Point3d line_end = p3d_2;
                line_start.z += 1; // On met la ligne légérement au dessus du sol
                line_end.z += 1;
                line_start.x += 2;
                line_end.x += 2;
                draw_quad_from_3d(p3d_1, p3d_2, line_end, line_start, colors.DIRT_COLOR, cam, renderer);
                p3d_1.x += display.tile_size;
                p3d_2.x += display.tile_size;
                p3d_3.x += display.tile_size;
                p3d_4.x += display.tile_size;
            }
        }
    
        // Dessin des bordures
        p3d_1.x = 0;
        p3d_2.x = 0;
        p3d_3.x = display.line_length*display.tile_size;
        p3d_4.x = display.line_length*display.tile_size;
               
        // On met le plan à la verticale et aligné avec le sol précedemment tracé
        p3d_2.z -= display.border_gap;
        p3d_3.z -= display.border_gap;
        p3d_1.y += display.line_width*display.tile_size;
        p3d_4.y += display.line_width*display.tile_size;

        // Dessin de la première bordure
        draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.GRASS_BORDER_COLOR, cam, renderer);

        // On met le plan à la verticale en le baissant d'un cran
        p3d_1.z -= display.border_gap;
        p3d_2.z -= display.border_gap;
        p3d_3.z -= display.border_gap;
        p3d_4.z -= display.border_gap;

        // On dessine la deuxième bordure
        draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.DIRT_COLOR, cam, renderer);

        break;

    case GROUND_WATER_LILY:
    case GROUND_WATER_LOG:

        // On baisse le plan à la hauteur de l'eau
        p3d_1.z -= 2*display.border_gap;
        p3d_2.z -= 2*display.border_gap;
        p3d_3.z -= 2*display.border_gap;
        p3d_4.z -= 2*display.border_gap;

        // On dessine d'abord la zone ombragée
        draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.WATER_SHADOW_COLOR, cam, renderer);

        // Dessin du sol
        p3d_1.x = LEFT_MAP_X;
        p3d_2.x = LEFT_MAP_X;
        p3d_3.x = RIGHT_MAP_X;
        p3d_4.x = RIGHT_MAP_X;

        // On dessine le sol
        draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.WATER_COLOR, cam, renderer);

        break;
    
    case GROUND_ROAD_CAR:
    case GROUND_ROAD_TRUCK:

        // On baisse le plan à la hauteur de la route
        p3d_1.z -= display.border_gap;
        p3d_2.z -= display.border_gap;
        p3d_3.z -= display.border_gap;
        p3d_4.z -= display.border_gap;

        // On dessine le sol
        draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.ROAD_COLOR, cam, renderer);

        // On met le plan à la verticale et aligné avec le sol précedemment tracé
        p3d_2.z -= display.border_gap;
        p3d_3.z -= display.border_gap;
        p3d_1.y += display.line_width*display.tile_size;
        p3d_4.y += display.line_width*display.tile_size;

        // Dessin de la bordure
        draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.ROAD_BORDER_COLOR, cam, renderer);
        break;
    
    case GROUND_TRAIN:
        // On baisse le plan à la hauteur de la route
        p3d_1.z -= display.border_gap;
        p3d_2.z -= display.border_gap;
        p3d_3.z -= display.border_gap;
        p3d_4.z -= display.border_gap;

        // On dessine le sol
        draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.ROAD_COLOR, cam, renderer);

        // On met le plan à la verticale et aligné avec le sol précedemment tracé
        p3d_2.z -= display.border_gap;
        p3d_3.z -= display.border_gap;
        p3d_1.y += display.line_width*display.tile_size;
        p3d_4.y += display.line_width*display.tile_size;

        // Dessin de la bordure
        draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.ROAD_BORDER_COLOR, cam, renderer);

        // On dessine les rails
        for (int i = 0; i < display.line_length; i++)
        {
            draw_sprite((Point3d){i*display.tile_size, (y+1)*display.tile_size, 0}, RAIL_ID, 0, sprite_sheet, renderer, cam, debug_kit);
        }
        break;
    case GROUND_ROAD_BORDER:

        // On baisse le plan à la hauteur de la route
        p3d_1.z -= display.border_gap;
        p3d_2.z -= display.border_gap;
        p3d_3.z -= display.border_gap;
        p3d_4.z -= display.border_gap;

        // On dessine le sol
        draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.ROAD_COLOR, cam, renderer);

        // On met le plan à la verticale et aligné avec le sol précedemment tracé
        p3d_2.z -= display.border_gap;
        p3d_3.z -= display.border_gap;
        p3d_1.y += display.line_width*display.tile_size;
        p3d_4.y += display.line_width*display.tile_size;

        // Dessin de la bordure
        draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.ROAD_BORDER_COLOR, cam, renderer);

        // On dessine la ligne entre les routes
        p3d_1.z = -display.border_gap+1;
        p3d_2.z = -display.border_gap+1;
        p3d_3.z = -display.border_gap+1;
        p3d_4.z = -display.border_gap+1;
        p3d_1.y = y * display.line_width*display.tile_size - display.tile_size/18;
        p3d_2.y = y * display.line_width*display.tile_size + display.tile_size/18;
        p3d_3.y = y * display.line_width*display.tile_size + display.tile_size/18;
        p3d_4.y = y * display.line_width*display.tile_size - display.tile_size/18;
        
        for (int i = 0; i < display.line_length; i += 2)
        {
            p3d_1.x = i*display.tile_size + display.tile_size/2;
            p3d_2.x = i*display.tile_size + display.tile_size/2;
            p3d_3.x = (i+1)*display.tile_size + display.tile_size/2;
            p3d_4.x = (i+1)*display.tile_size + display.tile_size/2;
            draw_quad_from_3d(p3d_1, p3d_2, p3d_3, p3d_4, colors.ROAD_LINE_COLOR, cam, renderer);
        }    
        break;
    
    default:
        break;
    }
}

/**
 * Fonction dessinant un sprite à un point donné de l'espace après transformation iso.
 * On dessinera le sprite_index donné.
 * @param p Point bas gauche de dessin du sprite
 * @param sprite_id Id du sprite dans la spritesheet
 * @param sprite_index Index du sprite choisi dans la spritesheet ( < sprite.sprite_count)
 * @param sprite_sheet la spritesheet
 * @param renderer le renderer
 * @param cam les paramètres de caméra
 * @param debug_kit le kit de débug
 * 
 */
void draw_sprite(Point3d p, int sprite_id, int sprite_index, Sprite_sheet *sprite_sheet, SDL_Renderer *renderer, Camera cam, debugKit *debug_kit)
{
    if (sprite_id >= sprite_sheet->sprites_nb)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Sprite id invalid: %d", sprite_index);
        exit(-1);
    }
    if (sprite_index >= sprite_sheet->sprites[sprite_id].sprite_count)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Sprite index invalid: %d", sprite_index);
        exit(-1);
    }

    Sprite sprite = sprite_sheet->sprites[sprite_id];
    
    // On transforme le point 3D en 2D
    Point2d p2d = d3_to_2d(p.x, p.y, p.z, cam);

    // On transforme le point pour corriger la modification de position due à la taille de l'image en perspective
    p2d.y += (float)sprite.sprites_coord[sprite_index].w * tan(cam.rotation);
    

    // On dessine le sprite
    SDL_Rect src_rect = {sprite.sprites_coord[sprite_index].x, sprite.sprites_coord[sprite_index].y, sprite.sprites_coord[sprite_index].w, sprite.sprites_coord[sprite_index].h};
    SDL_Rect dst_rect = {p2d.x, p2d.y - sprite.sprites_coord[sprite_index].h, sprite.sprites_coord[sprite_index].w, sprite.sprites_coord[sprite_index].h};

    // Pour le debug
    if (debug_kit->display_sprites)
    {
        SDL_RenderDrawRect(renderer, &dst_rect);
    }


    if (SDL_RenderCopy(renderer, sprite_sheet->sprite_sheet, &src_rect, &dst_rect) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to render copy: %s", SDL_GetError());
        exit(-1);
    }
}

/**
 * Fonction dessinant un sprite à un point donné en coordonnées du jeu (h_pos et y)
 * On dessinera le sprite_index donné à la position transformée en 3d
 * 
 * @param h_pos position horizontale du sprite
 * @param y position verticale du sprite
 * @param z position de hauteur du sprite
 * @param sprite_id l'id du sprite dans la feuille
 * @param sprite_index la variante du sprite à afficher
 * @param sprite_sheet la spritesheet
 * @param renderer le renderer
 * @param cam la camera
 * @param display les informations de l'affichage
 * @param debug_kit le kit de débug
 * 
 */
void draw_sprite_from_grid(float h_pos, float y, float z, int sprite_id, int sprite_index, Sprite_sheet *sprite_sheet, SDL_Renderer *renderer, Camera cam, Display_informations display, debugKit *debug_kit)
{
    // On transforme le point 3D en 2D en corrigeant la position légérement
    Point3d p = {h_pos, (y + (float)1) * (float)(display.line_width*display.tile_size)-(float)22, z + (float)2};
    draw_sprite(p, sprite_id, sprite_index, sprite_sheet, renderer, cam, debug_kit);
}

/**
 * Fonction qui calcule anim(t)
 * (une animation est définie par une fonction de la forme ax² + bx + c, Animation contient uniquement les coefficients)
 * 
 * @param anim une animation
 * @param t le temps écoulé dans l'animation
 * 
 */
float animation_calc(Animation anim, float t)
{
    if (t > anim.duration)
    {
        return -1.0f;
    }
    else
    {
        return (anim.a*(t*t) + anim.b*t + anim.c)*(float)DEFAULT_CELL_SIZE;
    }
}

/**
 * Charge une spritesheet dont le chemin est en argument selon les coordonnées en format JSON array
 * contenues dans le fichier de chemin coord_path
 * /!\ l'ordre des sprites et leurs variantes est donc important et ne sera pas changé
 * /!\ la taille des sprites et leurs variantes peut donc varier sans problème aucun
 * 
 * @param coord_path Chemin du fichier JSON contenant les coordonnées des sprites
 * @param sheet_path Chemin du fichier image contenant la spritesheet
 * @param renderer Le renderer SDL
 * @param cam La caméra
 * 
 * @return Le sprite créé
 */
Sprite_sheet load_spritesheet(char *coord_path, char *sheet_path, SDL_Renderer *renderer, Camera cam)
{
    // On charge la grille de lecture en mémoire
    int sprite_reading_grid[17] = {BLUE_CAR_NB, BLUE_TRUCK_NB, CHICKEN_NB, COIN_NB, GAZ_TRUCK_NB, LILYPAD_NB, LOG_LONG_NB, LOG_MEDIUM_NB, LOG_SMALL_NB, ORANGE_CAR_NB, PURPLE_CAR_NB, RAIL_NB, RED_TRUCK_NB, ROCK_NB, TRAIN_NB, TRAIN_POLE_NB, TREE_NB};

    // On charge la spritesheet
    SDL_Surface *sprite_surface = IMG_Load(sheet_path);
    if (!sprite_surface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image: %s", IMG_GetError());
        exit(-1);
    }

    SDL_Texture *sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);
    if (!sprite_texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture: %s", SDL_GetError());
        exit(-1);
    }
    SDL_FreeSurface(sprite_surface);

    // On charge les coordonnées des sprites
    FILE *file = fopen(coord_path, "r");
    if (!file)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to open file: %s", coord_path);
        exit(-1);
    }

    // On parse le fichier JSON avec une librairie externe
    json_object *j_file = json_object_from_file(coord_path);
    if (!j_file)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to parse JSON file: %s", coord_path);
        fclose(file);
        exit(-1);
    }
    json_object *jarray = json_object_object_get(j_file, "frames");
    if (!jarray)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to get sprites array from JSON file: %s", coord_path);
        fclose(file);
        exit(-1);
    }

    // On initialise la sprite_sheet
    Sprite_sheet ans_sheet;
    // On compte le nombre de sprites et on vérifie que le nombre est bon
    ans_sheet.sprites_nb = sizeof(sprite_reading_grid)/sizeof(int);
    ans_sheet.sprite_sheet = sprite_texture;
    ans_sheet.sprites = malloc(ans_sheet.sprites_nb * sizeof(Sprite));

    int sprite_temp = 0;
    for (int i = 0; i < ans_sheet.sprites_nb; i++)
    {
        ans_sheet.sprites[i].sprite_count = sprite_reading_grid[i];
        for (int j = 0; j < sprite_reading_grid[i]; j++)
        {
            json_object *sprite_obj = json_object_array_get_idx(jarray, sprite_temp);
            json_object *coord_array = json_object_object_get(sprite_obj, "frame");
            //On vérifie que le tableau de coordonées existe
            if (!coord_array)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to get coordinates array from JSON file: %s", coord_path);
                fclose(file);
                exit(-1);
            }
            json_object *x_obj, *y_obj, *w_obj, *h_obj;
            // Récupération des attributs x, y, w et h de chaque image
            json_object_object_get_ex(coord_array, "x", &x_obj);
            json_object_object_get_ex(coord_array, "y", &y_obj);
            json_object_object_get_ex(coord_array, "w", &w_obj);
            json_object_object_get_ex(coord_array, "h", &h_obj);

            ans_sheet.sprites[i].sprites_coord[j].x = json_object_get_int(x_obj);
            ans_sheet.sprites[i].sprites_coord[j].y = json_object_get_int(y_obj);
            ans_sheet.sprites[i].sprites_coord[j].w = json_object_get_int(w_obj);
            ans_sheet.sprites[i].sprites_coord[j].h = json_object_get_int(h_obj);
            sprite_temp ++;
        }
        // Calcul de la longueur de chaque sprite en fonction de la longueur de l'image associée (la taille ne sera pas conservée pour les sprites immobiles)
        // on utilise la longueur de la diagonale pour la longueur du sprite
        if (cam.rotation != 0.00)
        {
            ans_sheet.sprites[i].lenght = (float)ans_sheet.sprites[i].sprites_coord[0].w / cos(cam.rotation);
        }
        else
        {
            ans_sheet.sprites[i].lenght = (float)ans_sheet.sprites[i].sprites_coord[0].h;
        }
        }
    // Libération de l'objet associé au JSON
    json_object_put(j_file);
    return ans_sheet;
}

/**
 * Libère la mémoire allouée pour la sprite_sheet
 * 
 * @param sprite_sheet la sprite_sheet à libérer
 * 
 */
void unload_spritesheet(Sprite_sheet sprite_sheet)
{
    if (sprite_sheet.sprites != NULL)
    {
        free(sprite_sheet.sprites);
    }
    if (sprite_sheet.sprite_sheet != NULL)
    {
        SDL_DestroyTexture(sprite_sheet.sprite_sheet);
    }
    
}
