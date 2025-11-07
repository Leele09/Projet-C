#include <stdbool.h>

#include "menu_displayer.h"
#include "window.h"
#include "game_state.h"
#include "game_displayer.h"
#include "map.h"

int main(int argc, char *argv[]) {
    // Création de la fenêtre
    Window window;
    window.width = 1200;
    window.height = 700;
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

    // Initialisation du jeu
    Game game = init_game_window(&window);

    // Initialisation du parking
    Parking parking;
    init_parking(&parking, (float)WINDOW_W, (float)WINDOW_H);

  
    // Boucle principale
    GameState state = STATE_MENU;
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;

            if (state == STATE_MENU) {
                handle_menu_events(&menu, &event, &state);
            } else if (state == STATE_GAME) {
                
            }
        }

        // Rendu
        if (state == STATE_MENU) {
            render_menu(&window, &menu);
        } else if (state == STATE_GAME) {
            
        }
        SDL_RenderPresent(window.renderer);
        // Affichage du parking
        afficher_parking_sdl(renderer, &parking);

        SDL_Delay(16); // ~60 FPS
    }

    // Nettoyage
    SDL_DestroyTexture(menu.texture);
    SDL_DestroyRenderer(window.renderer);
    SDL_DestroyWindow(window.SDL_window);
    SDL_Quit();
    return 0;
}
