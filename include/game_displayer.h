#ifndef GAME_DISPLAYER_H
#define GAME_DISPLAYER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>

#include "window.h"
#include "parking.h"
#include "vehicule.h"
#include "game_state.h"

typedef struct {
    VEHICULE voiture;
    Parking parking;
} Game;

Game init_game(Window *window);
void handle_game_events(Game *game, SDL_Event *event, GameState *state);
void update_game(Game *game);
void render_game(Window *window, Game *game);
void reset_game(Game *game);
void destroy_game(Game *game);

#endif
