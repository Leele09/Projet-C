#include <SDL3/SDL.h>
#include "map.h"

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return 1;
    }

    // Dimensions de la fenêtre
    const int WINDOW_W = 1200;
    const int WINDOW_H = 700;

    SDL_Window *window = SDL_CreateWindow("Parking Simulation", WINDOW_W, WINDOW_H, 0);
    if (!window) {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialisation du parking
    Parking parking;
    init_parking(&parking, (float)WINDOW_W, (float)WINDOW_H);

    bool running = true;
    SDL_Event event;

    // Boucle principale
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Affichage du parking
        afficher_parking_sdl(renderer, &parking);
        SDL_Log("Rendering frame...\n");
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Nettoyage
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
