#include <stdbool.h>

#include "menu_displayer.h"
#include "window.h"
#include "game_state.h"
#include "game_displayer.h"
#include "parking.h"

#if defined(_WIN32)
    Uint32 window_flags = SDL_WINDOW_RESIZABLE;
#elif defined(__APPLE__)
    Uint32 window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
#elif defined(__linux__)
    Uint32 window_flags = SDL_WINDOW_RESIZABLE;
#else
    Uint32 window_flags = 0;
#endif

int main(int argc, char *argv[]) {
    // --- 1. INITIALISATION DE LA FENÊTRE ET DE SDL ---
    Window window;
    window.width = 1200;
    window.height = 700;
    SDL_Color backgroundColor = {219, 233, 238, 255};
    window.backgroundColor = backgroundColor;

    window.SDL_window = SDL_CreateWindow(
        "Simulateur de Parking",
        window.width, window.height,
        window_flags
    );

    if (!window.SDL_window) {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    window.renderer = SDL_CreateRenderer(window.SDL_window, NULL);
    if (!window.renderer) {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(window.SDL_window);
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == false) {
        SDL_Log("Erreur TTF_Init: %s", SDL_GetError());
        return 1;
    }

    // --- 2. INITIALISATION DES ÉLÉMENTS DU JEU ---
    Menu menu = init_menu(&window);
    Game game = init_game(&window);

    GameState state = STATE_MENU;
    bool running = true;
    SDL_Event event;
    
    // --- 3. BOUCLE PRINCIPALE DU JEU ---
    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (state == STATE_MENU) {
                handle_menu_events(&menu, &event, &state);
            } else if (state == STATE_GAME) {
                handle_game_events(&game, &event, &state);
            }
        }
        
        if (state == STATE_GAME) {
            update_game(&game);
        }
        // Nettoyage l'écran au début
        SDL_SetRenderDrawColor(window.renderer, window.backgroundColor.r, window.backgroundColor.g, window.backgroundColor.b, 255);
        SDL_RenderClear(window.renderer);

        if (state == STATE_MENU) {
            render_menu(&window, &menu);
        } else if (state == STATE_GAME) {
            render_game(&window, &game);
        }

        SDL_RenderPresent(window.renderer); 
        SDL_Delay(16); // 60 FPS
    }

    destroy_game(&game);

    SDL_DestroyTexture(menu.texture);
    TTF_CloseFont(menu.font);

    destroy_button(&menu.easyModeButton);
    destroy_button(&menu.hardModeButton);
    
    SDL_DestroyRenderer(window.renderer);
    SDL_DestroyWindow(window.SDL_window);
    
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}