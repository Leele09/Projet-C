#ifndef GAME_OVER_H
#define GAME_OVER_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "button.h"
#include "window.h"
#include "game_state.h"

typedef struct {
    Button restartButton;
    Button menuButton;
    TTF_Font *titleFont;
    TTF_Font *buttonFont;
} GameOverScreen;

typedef enum {
    ACTION_NONE,
    ACTION_RESTART,
    ACTION_MENU
} GameOverAction;

GameOverScreen init_game_over(Window *window);
void render_game_over(Window *window, GameOverScreen *screen, int final_score);
GameOverAction handle_game_over_events(GameOverScreen *screen, SDL_Event *event);
void destroy_game_over(GameOverScreen *screen);

#endif