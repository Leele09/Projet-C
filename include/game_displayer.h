#ifndef GAME_DISPLAYER_H
#define GAME_DISPLAYER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "window.h"
#include "button.h"
#include "game_state.h"
#include "voiture.h"

typedef struct {
    Window *SDL_window;
    int map;
    Voiture playerCar;
} Game;

Game init_game_window(Window *SDL_window);
void render_game(Window *SDL_window, Game *game);
void handle_game_events(Game *game, SDL_Event *event, GameState *state);

#endif
