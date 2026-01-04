#ifndef GAME_DISPLAYER_H
#define GAME_DISPLAYER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>

#include "window.h"
#include "parking.h"
#include "vehicule.h"
#include "game_state.h"

#define MAX_PNJ_EASY 10
#define MAX_PNJ_HARD 20
#define MAX_PNJ_CAPACITY MAX_PNJ_HARD

typedef struct {
    VEHICULE voiture;
    VEHICULE pnjs[MAX_PNJ_CAPACITY];
    int nb_pnjs;

    GameMode currentMode;
    Parking parking;
    int target_spot_index;
    int score;
} Game;

Game init_game(Window *window, GameMode mode);
void init_pnjs(Game *game, SDL_Renderer *renderer);
void handle_game_events(Game *game, SDL_Event *event, GameState *state);
void update_game(Game *game, GameState *state);
void update_pnjs(Game *game);
void render_game(Window *window, Game *game);
void reset_game(Game *game);
void destroy_game(Game *game);

#endif
