#ifndef BUTTON_H
#define BUTTON_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct {
    SDL_FRect rect;
    SDL_Color color;
    SDL_Color hoverColor;
    SDL_Color clickColor;
    bool hovered;
    bool clicked;
    const char *text;      
    SDL_Color textColor;    
    TTF_Font *font;
    SDL_Texture *textTexture;
    int text_w;
    int text_h;
} Button;

Button create_button(float x, float y, float w, float h, SDL_Color color,
                     const char *text, TTF_Font *font, SDL_Renderer *renderer);
void handle_button_event(Button *btn, SDL_Event *event);
void render_button(SDL_Renderer *renderer, Button *btn);
void destroy_button(Button *btn);

#endif
