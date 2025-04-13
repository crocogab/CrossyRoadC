#include "ttf.h"

int main() {
    // Initialisation de la SDL
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL_ttf: %s", TTF_GetError());
        exit(-1);
    }
    atexit(TTF_Quit);

    // Déclaration des paramètres de la police
    const char* font_path = "assets/editundo.ttf";
    int taille = 24;

    // Chargement de la police
    TTF_Font* font = font_load(font_path, taille);
    if (!font) {
        return -1; // Erreur lors du chargement de la police
    }

    // Création de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Test TTF",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window: %s", SDL_GetError());
        font_unload(font);
        return -1;
    }
    // Création du renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        font_unload(font);
        return -1;
    }
    // Couleur du texte
    SDL_Color color = {255, 255, 255, SDL_ALPHA_OPAQUE}; // Blanc
    // Couleur de fond
    SDL_Color const BACKGROUND_COLOR = {255, 255, 255, SDL_ALPHA_OPAQUE}; // Blanc
    if (SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to set render draw color: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        font_unload(font);
        return -1;
    }

    SDL_Event event;
    int running = 1;

    while (running)
    {
        if(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;

            case SDL_KEYDOWN:
            // On utilisera les flèches pour changer l'orientation de la caméra
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = 0;
                }

            default:
                break;
            }
        }

        if (SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to set render draw color: %s", SDL_GetError());
            exit(-1);
        }

        if (SDL_RenderClear(renderer))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to clear renderer: %s", SDL_GetError());
            exit(-1);
        }

        write_text("Bonjour.12&*", 100, 100, 25, 25, 2, color, renderer, font);

        // Mise à jour de l'affichage
        SDL_RenderPresent(renderer);

    }

    // Libération de la police
    font_unload(font);
    // Et du reste
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}
