#include "game_displayer.h"
#include <math.h>
#include <stdio.h>

Game init_game(Window *window) {
    Game game;

    // --- Init Parking ---
    game.parking = init_parking((float)window->width, (float)window->height);
    // --- Init Voiture ---
    game.voiture = init_voiture(window->renderer, START_X, START_Y, START_ANGLE);

    return game;
}

void handle_game_events(Game *game, SDL_Event *event, GameState *state) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_UP:
                game->voiture.is_accelerating = true;
                break;
            case SDL_SCANCODE_DOWN:
                game->voiture.is_reversing = true;
                break;
            case SDL_SCANCODE_LEFT:
                game->voiture.is_turning_left = true;
                break;
            case SDL_SCANCODE_RIGHT:
                game->voiture.is_turning_right = true;
                break;
            case SDL_SCANCODE_ESCAPE:
                *state = STATE_MENU;
                reset_game(game);
                break;
            default: break;
        }
    }

    if (event->type == SDL_EVENT_KEY_UP) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_UP:
                game->voiture.is_accelerating = false;
                break;
            case SDL_SCANCODE_DOWN:
                game->voiture.is_reversing = false;
                break;
            case SDL_SCANCODE_LEFT:
                game->voiture.is_turning_left = false;
                break;
            case SDL_SCANCODE_RIGHT:
                game->voiture.is_turning_right = false;
                break;
            default: break;
        }
    }
}

void update_game(Game *game) {
    // Vitesse
    if (game->voiture.is_accelerating) {
        game->voiture.vitesse = 2.0f;
    } else if (game->voiture.is_reversing) {
        game->voiture.vitesse = -1.5f;
    } else {
        game->voiture.vitesse = 0.0f;
    }

    // Angle
    if (game->voiture.is_turning_left) {
        game->voiture.angle -= 3.0;
    }
    if (game->voiture.is_turning_right) {
        game->voiture.angle += 3.0;
    }

    // Position
    double rad = game->voiture.angle * M_PI / 180.0;
    game->voiture.posx += game->voiture.vitesse * sin(rad);
    game->voiture.posy -= game->voiture.vitesse * cos(rad);
}

void render_game(Window *window, Game *game) {
    afficher_parking_sdl(window->renderer, &game->parking);

    VEHICULE *v = &game->voiture;

    SDL_FRect dest = { v->posx, v->posy, v->width, v->height };
    SDL_FPoint center = { v->width / 2.0f, v->height / 2.0f };

    SDL_RenderTextureRotated(
        window->renderer, 
        v->texture,
        NULL, 
        &dest, 
        v->angle,
        &center, 
        SDL_FLIP_NONE
    );
}

void reset_game(Game *game) {
    game->voiture.posx = START_X;
    game->voiture.posy = START_Y;
    game->voiture.angle = START_ANGLE;
    game->voiture.vitesse = 0.0f;

    for (int i = 0; i < game->parking.nb_spots; i++) {
        game->parking.spots[i].occupied = false;
        game->parking.spots[i].borne_active = false;
    }
}

void destroy_game(Game *game) {
    destroy_voiture(&game->voiture);
}