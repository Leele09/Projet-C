#include <SDL2/SDL.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    bool running = true;
    SDL_Event event;

    // Boucle de jeu
    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Effacer l'écran (fond noir)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Dessiner un rectangle rouge
        SDL_Rect rect = { 200, 150, 100, 100 };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        // Afficher à l'écran
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Limite à ~60 FPS
    }

    // Nettoyage
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
