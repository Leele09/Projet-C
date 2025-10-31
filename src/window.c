#include "window.h"

void render_background_color(Window *window) {
    SDL_SetRenderDrawColor(window->renderer, window->backgroundColor.r, window->backgroundColor.g, window->backgroundColor.b, window->backgroundColor.a);
    SDL_RenderClear(window->renderer);
}