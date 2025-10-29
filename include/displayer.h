#ifndef DISPLAYER_H
#define DISPLAYER_H

#include <SDL3/SDL.h>

SDL_Window* display_window(int h, int w);
SDL_Renderer* get_renderer(SDL_Window *window);
void set_background_color(SDL_Renderer *renderer, SDL_Color color);

#endif
