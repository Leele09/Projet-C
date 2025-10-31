#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>

typedef struct {
    int width;
    int height;
    SDL_Color backgroundColor;
    SDL_Renderer *renderer;
    SDL_Window *SDL_window;
} Window;

void render_background_color(Window *window);

#endif