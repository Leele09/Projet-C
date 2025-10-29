#include "button.h"

Button create_button(float x, float y, float w, float h, SDL_Color color) {
    Button btn;
    btn.rect = (SDL_FRect){x, y, w, h};
    btn.color = color;
    btn.hoverColor = (SDL_Color){color.r + 30, color.g + 30, color.b + 30, 255};
    btn.clickColor = (SDL_Color){200, 50, 50, 255};
    btn.hovered = false;
    btn.clicked = false;
    return btn;
}

void handle_button_event(Button *btn, SDL_Event *event) {
    if (!btn || !event) return;

    if (event->type == SDL_EVENT_MOUSE_MOTION) {
        float x = event->motion.x;
        float y = event->motion.y;
        btn->hovered = (x >= btn->rect.x && x <= btn->rect.x + btn->rect.w &&
                        y >= btn->rect.y && y <= btn->rect.y + btn->rect.h);
    }
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float x = event->button.x;
        float y = event->button.y;
        if (x >= btn->rect.x && x <= btn->rect.x + btn->rect.w &&
            y >= btn->rect.y && y <= btn->rect.y + btn->rect.h) {
            btn->clicked = true;
        }
    }
    else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        btn->clicked = false;
    }
}

void render_button(SDL_Renderer *renderer, Button *btn) {
    if (!btn) return;

    if (btn->clicked)
        SDL_SetRenderDrawColor(renderer, btn->clickColor.r, btn->clickColor.g, btn->clickColor.b, btn->clickColor.a);
    else if (btn->hovered)
        SDL_SetRenderDrawColor(renderer, btn->hoverColor.r, btn->hoverColor.g, btn->hoverColor.b, btn->hoverColor.a);
    else
        SDL_SetRenderDrawColor(renderer, btn->color.r, btn->color.g, btn->color.b, btn->color.a);

    SDL_RenderFillRect(renderer, &btn->rect);
}
