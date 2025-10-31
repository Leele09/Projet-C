#include <stdbool.h>

#include "menu_displayer.h"
#include "window.h"

int main(int argc, char *argv[]) {
    // Création de la fenêtre
    Window window;
    window.width = 800;
    window.height = 600;
    window.backgroundColor = (SDL_Color){219, 233, 238, 255};

    window.SDL_window = SDL_CreateWindow(
        "Jeu de parking",
        window.width, window.height,
        SDL_WINDOW_RESIZABLE
    );

    if (!window.SDL_window) {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du renderer
    window.renderer = SDL_CreateRenderer(window.SDL_window, NULL);
    if (!window.renderer) {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(window.SDL_window);
        SDL_Quit();
        return 1;
    }

    // Initialisation du menu
    Menu menu = init_menu_window(&window);

    // Boucle principale
    bool running = true;
    SDL_Event event;

    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            handle_menu_events(&menu, &event);
        }

        // Rendu
        render_menu(&window, &menu);
        SDL_RenderPresent(window.renderer);

        SDL_Delay(16); // ~60 FPS
    }

    // Nettoyage
    SDL_DestroyTexture(menu.texture);
    SDL_DestroyRenderer(window.renderer);
    SDL_DestroyWindow(window.SDL_window);
    SDL_Quit();

    return 0;
}
