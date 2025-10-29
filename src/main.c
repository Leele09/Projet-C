#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>
#include "displayer.h"
#include "button.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char *argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = display_window(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!window) return 1;

    SDL_Renderer *renderer = get_renderer(window);
    if (!renderer) return 1;

    SDL_Texture *texture = IMG_LoadTexture(renderer, "assets/logo.png");
    if (!texture) {
        SDL_Log("Erreur IMG_LoadTexture: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_FRect rect = {150, 10, 500, 400};
    Button easyModeButton = create_button(280, 320, 250, 70, (SDL_Color){79, 109, 122, 255});
    Button hardModeButton = create_button(280, 400, 250, 70, (SDL_Color){79, 109, 122, 255});

    bool running = true;
    SDL_Event event;

    // Boucle de jeu
    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            handle_button_event(&easyModeButton, &event);
            handle_button_event(&hardModeButton, &event);
        }

        set_background_color(renderer, (SDL_Color){219, 233, 238, 1});
        SDL_RenderTexture(renderer, texture, NULL, &rect);
        render_button(renderer, &easyModeButton);
        render_button(renderer, &hardModeButton);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Nettoyage
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

