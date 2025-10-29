#ifndef BUTTON_H
#define BUTTON_H

#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct {
    SDL_FRect rect;
    SDL_Color color;
    SDL_Color hoverColor;
    SDL_Color clickColor;
    bool hovered;
    bool clicked;
} Button;

Button create_button(float x, float y, float w, float h, SDL_Color color);
void handle_button_event(Button *btn, SDL_Event *event);
void render_button(SDL_Renderer *renderer, Button *btn);

#endif
