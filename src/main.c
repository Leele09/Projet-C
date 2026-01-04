#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "menu_displayer.h"
#include "window.h"
#include "game_state.h"
#include "game_displayer.h"
#include "parking.h"
#include <game_over.h>

#if defined(_WIN32)
    Uint32 window_flags = SDL_WINDOW_RESIZABLE;
#elif defined(__APPLE__)
    Uint32 window_flags = SDL_WINDOW_METAL;
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

    srand(time(NULL));

    // --- 2. INITIALISATION DES ÉLÉMENTS DU JEU ---
    Menu menu = init_menu(&window);
    
    Game game;
    GameState state = STATE_MENU;
    GameOverScreen gameOverScreen = init_game_over(&window);

    bool running = true;
    SDL_Event event;
    
    // --- 3. BOUCLE PRINCIPALE DU JEU ---
    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            
            if (state == STATE_MENU) {
                handle_menu_events(&menu, &event, &state, &game.currentMode);
                game = init_game(&window, game.currentMode);
            } else if (state == STATE_GAME) {
                handle_game_events(&game, &event, &state);
                if (state == STATE_MENU) {
                    destroy_game(&game);
                }
            } else if (state == STATE_GAMEOVER) {
                GameOverAction action = handle_game_over_events(&gameOverScreen, &event);
                
                if (action == ACTION_RESTART) {
                    destroy_game(&game);
                    game = init_game(&window, game.currentMode);
                    state = STATE_GAME;
                }
                else if (action == ACTION_MENU) {
                    destroy_game(&game);
                    state = STATE_MENU;
                }
            }
        }
        
        if (state == STATE_GAME) {
            update_game(&game, &state);
        }
        
        // Nettoyage l'écran au début
        SDL_SetRenderDrawColor(window.renderer, window.backgroundColor.r, window.backgroundColor.g, window.backgroundColor.b, 255);
        SDL_RenderClear(window.renderer);

        if (state == STATE_MENU) {
            render_menu(&window, &menu);
        } else if (state == STATE_GAME) {
            render_game(&window, &game);
        } else if (state == STATE_GAMEOVER) {
            render_game(&window, &game);
            render_game_over(&window, &gameOverScreen, game.score);
        }

        SDL_RenderPresent(window.renderer); 
        SDL_Delay(16); // 60 FPS
    }

    destroy_game(&game);
    destroy_game_over(&gameOverScreen);

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