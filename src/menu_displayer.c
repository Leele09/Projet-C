#include "menu_displayer.h"

Menu init_menu_window(Window *SDL_window) {
    Menu menu;
    menu.texture = IMG_LoadTexture(SDL_window->renderer, "assets/logo.png");
    if (!menu.texture) {
        SDL_Log("Erreur IMG_LoadTexture: %s", SDL_GetError());
        SDL_DestroyRenderer(SDL_window->renderer);
        SDL_DestroyWindow(SDL_window->SDL_window);
        SDL_Quit();
        return menu;
    }

    int window_width, window_height;
    SDL_GetWindowSize(SDL_window->SDL_window, &window_width, &window_height);

    // Taille des boutons
    int button_width = 250;
    int button_height = 70;

    // Calcul position centrée
    float button_x = (window_width - button_width) / 2.0f;
    float rect_x = (window_width - 500) / 2.0f;

    // Couleurs des boutons
    SDL_Color vert = {60, 200, 90, 255};
    SDL_Color rouge = {200, 60, 90, 255};

    menu.rect = (SDL_FRect){rect_x, 10, 500, 400};
    TTF_Font *font = TTF_OpenFont("assets/Roboto.ttf", 32);
    if (!font) {
        SDL_Log("Erreur TTF_OpenFont: %s", SDL_GetError());
    }

    menu.easyModeButton = create_button(
        button_x, 320, button_width, button_height,
        vert, "Easy Mode", font, SDL_window->renderer
    );

    menu.hardModeButton = create_button(
        button_x, 400, button_width, button_height,
        rouge, "Hard Mode", font, SDL_window->renderer
    );

    return menu;
}

void render_menu(Window *SDL_window, Menu *menu) {
    render_background_color(SDL_window);
    SDL_RenderTexture(SDL_window->renderer, menu->texture, NULL, &menu->rect);
    render_button(SDL_window->renderer, &menu->easyModeButton);
    render_button(SDL_window->renderer, &menu->hardModeButton);
}

void handle_menu_events(Menu *menu, SDL_Event *event, GameState *state) {
    handle_button_event(&menu->easyModeButton, event);
    if (menu->easyModeButton.clicked) {
        *state = STATE_GAME;
    }
    handle_button_event(&menu->hardModeButton, event);
    if (menu->hardModeButton.clicked) {
        *state = STATE_GAME;
    }
}