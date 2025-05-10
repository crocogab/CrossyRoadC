#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../include/menu.h"
#include "macro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>



void save_high_score(const char* name, int score) {
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

