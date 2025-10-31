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

    menu.rect = (SDL_FRect){150, 10, 500, 400};
    menu.easyModeButton = create_button(280, 320, 250, 70, (SDL_Color){79, 109, 122, 255});
    menu.hardModeButton = create_button(280, 400, 250, 70, (SDL_Color){79, 109, 122, 255});

    return menu;
}

void render_menu(Window *SDL_window, Menu *menu) {
    render_background_color(SDL_window);
    SDL_RenderTexture(SDL_window->renderer, menu->texture, NULL, &menu->rect);
    render_button(SDL_window->renderer, &menu->easyModeButton);
    render_button(SDL_window->renderer, &menu->hardModeButton);
}

void handle_menu_events(Menu *menu, SDL_Event *event) {
    handle_button_event(&menu->easyModeButton, event);
    handle_button_event(&menu->hardModeButton, event);
}