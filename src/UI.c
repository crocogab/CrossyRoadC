#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <json-c/json.h>
#include "gui.h"
#include "macro.h"
#include "UI.h"

Button create_button(int button_id, int x, int y, int is_hidden, int state, Sprite_sheet *menu_spritesheet, int sprite_id)
{
    return (Button){button_id, x, y, menu_spritesheet->sprites[sprite_id].sprites_coord->w, menu_spritesheet->sprites[sprite_id].sprites_coord->h, is_hidden, state, menu_spritesheet, 0, sprite_id};
}

void save_high_score(const char *name, int score)
{
    if (strlen(name) != 3) {
        printf("Le nom doit être une chaîne de 3 caractères.\n");
        return;
    }

    // Lire le fichier
    FILE *file = fopen(FILE_NAME_SCORE, "r");
    if (file == NULL) {
        printf("Fichier non trouvé\n");
    }

    char *file_content = NULL;
    long file_size = 0;

    // Avoir la taille du fichier
    if (file) {
        fseek(file, 0, SEEK_END);     
        file_size = ftell(file);      
        rewind(file);                 

        file_content = (char *)malloc(file_size + 1);
        fread(file_content, 1, file_size, file);
        file_content[file_size] = '\0';  
        fclose(file);
    }

    // Si le fichier existait et contenait des données
    struct json_object *scores_json = NULL;

    if (file_content) {
        scores_json = json_tokener_parse(file_content);  // Parser le contenu JSON
        free(file_content);  
    }

    
    if (!scores_json) {
        scores_json = json_object_new_object();
    }

    // MAJ du score, s'il existe
    struct json_object *old_score_obj = NULL;
    if (json_object_object_get_ex(scores_json, name, &old_score_obj)) {
        int old_score = json_object_get_int(old_score_obj);

        // Si le nouveau score est meilleur, on met à jour
        if (score > old_score) {
            json_object_object_add(scores_json, name, json_object_new_int(score));
            printf("Nouveau meilleur score pour %s : %d\n", name, score);
        } else {
            printf("Aucun nouveau record pour %s. Score existant : %d\n", name, old_score);
        }
    } else {
        // Si le joueur n'a pas de score enregistré, on ajoute un nouveau score
        json_object_object_add(scores_json, name, json_object_new_int(score));
        printf("Nouveau score pour %s : %d\n", name, score);
    }

    // Sauvegarder le fichier JSON mis à jour
    const char *updated_json = json_object_to_json_string(scores_json);
    file = fopen(FILE_NAME_SCORE, "w");
    if (file) {
        fputs(updated_json, file);  // Écrire dans le fichier
        fclose(file);
    } else {
        printf("Erreur lors de l'écriture dans le fichier.\n");
    }

    // Libérer l'objet JSON
    json_object_put(scores_json);
}

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
 * @return La sprite_sheet chargée
 */
Sprite_sheet load_ui_spritesheet(char *coord_path, char *sheet_path, SDL_Renderer *renderer, Camera cam)
{
    // On charge la grille de lecture en mémoire
    int sprite_reading_grid[12] = {CONTROLS_NB, GIFT_NB, SKINS_NB, ANDROID_NB, SOUND_NB, SHOP_NB, BACK_NB, PAUSE_NB, PLAY_NB, SETTINGS_NB, TITLE_NB, VIDEO_NB};

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
void unload_ui_spritesheet(Sprite_sheet sprite_sheet)
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
