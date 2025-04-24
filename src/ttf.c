#include "ttf.h"
#include <math.h>

/**
 * Charge une police d'écriture avec SDL_ttf
 * @param font_path le chemin vers la police de caractères
 * @param taille la taille de la police (en nombre de points)
 * 
 * @return Un pointeur vers la police chargée, ou NULL en cas d'erreur
 * 
 */
TTF_Font* font_load(const char* font_path, int taille) {
    TTF_Font* police = TTF_OpenFont(font_path, taille);
    if (!police) {
        SDL_Log("Erreur lors du chargement de la police : %s", TTF_GetError());
    }
    return police;
}

/**
 * Fonction déchargeant une police d'écriture, on évite les fuites de mémoires ;)
 * @param font la font à décharger
 * 
 */
void font_unload(TTF_Font *font)
{
    if (font) {
        TTF_CloseFont(font);
    }
}

/**
 * Affiche un texte sur l'écran avec des options de style avancées.
 * 
 * @param texte le texte à afficher
 * @param x la position en pixels sur l'axe X où afficher le texte
 * @param y la position en pixels sur l'axe Y où afficher le texte
 * @param shadow_angle l'angle de l'ombre projetée (-1 si pas d'ombre)
 * @param shadow_length la longueur de l'ombre projetée
 * @param outline_size l'épaisseur du contour du texte (0 si pas d'outline)
 * @param color la couleur du texte
 * @param renderer le renderer
 * @param font la police de caractères utilisée pour le texte
 * 
 */
void write_text(const char* texte, int x, int y, float shadow_angle, float shadow_length, float outline_size, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, texte, color);
    if (!surface) {
        SDL_Log("Error while creating the text surface: %s", TTF_GetError());
        exit(-1);
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Error while creating the text texture: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        exit(-1);
    }

    if (shadow_angle >= 0.00)
    {
        // Affichage de l'ombre
        SDL_SetTextureColorMod(texture, 0, 0, 0);

        // Calcul de la position de l'ombre
        int shadow_x = x + (int)(shadow_length * cos(shadow_angle * (3.1415 / 180.0)));
        int shadow_y = y + (int)(shadow_length * sin(shadow_angle * (3.1415 / 180.0)));

        int steps = (int)sqrt((shadow_x - x) * (shadow_x - x) + (shadow_y - y) * (shadow_y - y));
        float step_x = (float)(shadow_x - x) / steps;
        float step_y = (float)(shadow_y - y) / steps;

        for (int i = 0; i < steps; i++)
        {
            int current_x = x + (int)(i * step_x);
            int current_y = y + (int)(i * step_y);

            SDL_Rect shadowRect = {current_x-outline_size, current_y-outline_size, surface->w+outline_size, surface->h+outline_size};
            SDL_RenderCopy(renderer, texture, NULL, &shadowRect);
        }

        // Restauration de la couleur d'origine
        SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    }
    
    // Ajout d'un contour noir au texte
    SDL_Color black = {0, 0, 0, 255};
    SDL_Surface* outline_surface = TTF_RenderText_Blended(font, texte, black);
    if (!outline_surface) {
        SDL_Log("Error while creating the outline surface: %s", TTF_GetError());
        exit(-1);
    }

    SDL_Texture* outline_texture = SDL_CreateTextureFromSurface(renderer, outline_surface);
    if (!outline_texture) {
        SDL_Log("Error while creating the outline texture: %s", SDL_GetError());
        SDL_FreeSurface(outline_surface);
        exit(-1);
    }

    // On dessine le contour en décalant légèrement dans toutes les directions
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx != 0 || dy != 0) { // Éviter de dessiner au centre
                SDL_Rect outlineRect = {x + dx*outline_size, y + dy*outline_size, outline_surface->w, outline_surface->h};
                SDL_RenderCopy(renderer, outline_texture, NULL, &outlineRect);
            }
        }
    }

    SDL_DestroyTexture(outline_texture);
    SDL_FreeSurface(outline_surface);

    // Affichage du texte
    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
