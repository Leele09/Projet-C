#include <stdbool.h>

#include "menu_displayer.h"
#include "window.h"
#include "game_state.h"
#include "game_displayer.h"
#include "map.h"
// Si vous avez besoin de fonctions de mise à jour pour le jeu/voiture, 
// incluez les en-têtes nécessaires ici (ex: "game_logic.h")

// Déclaration du prototype de la fonction de mise à jour du jeu (à créer)
// void update_game(Game *game, Parking *parking, float delta_time); 

int main(int argc, char *argv[]) {
    // --- 1. INITIALISATION DE LA FENÊTRE ET DE SDL ---
    Window window;
    window.width = 1200;
    window.height = 700;
    window.backgroundColor = (SDL_Color){219, 233, 238, 255};

    window.SDL_window = SDL_CreateWindow(
        "Simulateur de Parking",
        window.width, window.height,
        SDL_WINDOW_METAL
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

    // --- 2. INITIALISATION DES ÉLÉMENTS DU JEU ---
    
    // Initialisation du menu
    Menu menu = init_menu_window(&window);

    // Initialisation du jeu (incluant la voiture et l'état initial)
    Game game = init_game_window(&window);

    // Initialisation du parking
    Parking parking;
    // CORRECTION : Utilisation de window.width et window.height (comme discuté)
    init_parking(&parking, (float)window.width, (float)window.height);

    // --- 3. BOUCLE PRINCIPALE DU JEU ---
    GameState state = STATE_MENU;
    bool running = true;
    SDL_Event event;

    // Variables pour le timing (utile pour les mises à jour basées sur le temps)
    // Uint64 last_time = SDL_GetTicks();
    // float delta_time;

    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (state == STATE_MENU) {
                handle_menu_events(&menu, &event, &state);
            } else if (state == STATE_GAME) {
                // Gestion des événements spécifiques au jeu (clavier, etc.)
            }
        }
        
        if (state == STATE_GAME) {
 
        }
        
        if (state == STATE_MENU) {
            render_menu(&window, &menu);
        } else if (state == STATE_GAME) {
            afficher_parking_sdl(window.renderer, &parking);
        }
        SDL_RenderPresent(window.renderer); 
        
        SDL_Delay(16); // Environ 60 FPS
    }

    SDL_DestroyTexture(menu.texture);
    SDL_DestroyRenderer(window.renderer);
    SDL_DestroyWindow(window.SDL_window);
    SDL_Quit();
    
    return 0;
}