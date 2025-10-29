#include <SDL3/SDL.h>
#include "displayer.h"

SDL_Window* display_window(int h, int w) {
    SDL_Window *window = SDL_CreateWindow(
        "Jeu de parking",
        h, w,
        SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        return NULL;
    }

    return window;
}

SDL_Renderer* get_renderer(SDL_Window *window) {
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        return NULL;
    }

    return renderer;
}

void set_background_color(SDL_Renderer *renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}
