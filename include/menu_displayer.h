#ifndef MENU_DISPLAYER_H
#define MENU_DISPLAYER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "window.h"
#include "button.h"

typedef struct {
    Window *SDL_window;
    SDL_Texture *texture;
    SDL_FRect rect;
    Button easyModeButton;
    Button hardModeButton;
} Menu;

Menu init_menu_window(Window *SDL_window);
void render_menu(Window *SDL_window, Menu *menu);
void handle_menu_events(Menu *menu, SDL_Event *event);

#endif
