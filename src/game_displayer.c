#include "game_displayer.h"
#include "vehicule.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void reset_voiture_position(Game *game) {
    game->voiture.posx = START_X;
    game->voiture.posy = START_Y;
    game->voiture.angle = START_ANGLE;
    game->voiture.vitesse = 0;
}

void pick_next_target(Game *game) {
    int disponibles[NB_PLACES * 8];
    int count = 0;

    for (int i = 0; i < game->parking.nb_spots; i++) {
        if (!game->parking.spots[i].occupied) {
            disponibles[count++] = i;
        }
    }

    if (count > 0) {
        game->target_spot_index = disponibles[rand() % count];
    } else {
        game->target_spot_index = -1;
    }
}

Game init_game(Window *window) {
    Game game;
    game.score = 0;
    game.parking = init_parking((float)window->width, (float)window->height);
    game.voiture = init_voiture(window->renderer, START_X, START_Y, START_ANGLE);
    
    pick_next_target(&game);
    return game;
}

void handle_game_events(Game *game, SDL_Event *event, GameState *state) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_UP:    game->voiture.is_accelerating = true; break;
            case SDL_SCANCODE_DOWN:  game->voiture.is_reversing = true; break;
            case SDL_SCANCODE_LEFT:  game->voiture.is_turning_left = true; break;
            case SDL_SCANCODE_RIGHT: game->voiture.is_turning_right = true; break;
            case SDL_SCANCODE_ESCAPE:
                *state = STATE_MENU;
                reset_game(game);
                break;
            default: break;
        }
    }
    if (event->type == SDL_EVENT_KEY_UP) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_UP:    game->voiture.is_accelerating = false; break;
            case SDL_SCANCODE_DOWN:  game->voiture.is_reversing = false; break;
            case SDL_SCANCODE_LEFT:  game->voiture.is_turning_left = false; break;
            case SDL_SCANCODE_RIGHT: game->voiture.is_turning_right = false; break;
            default: break;
        }
    }
}

void update_game(Game *game) {
    // --- Mouvement ---
    if (game->voiture.is_accelerating) game->voiture.vitesse = 2.0f;
    else if (game->voiture.is_reversing) game->voiture.vitesse = -1.5f;
    else game->voiture.vitesse = 0.0f;

    if (game->voiture.is_turning_left) game->voiture.angle -= 3.0;
    if (game->voiture.is_turning_right) game->voiture.angle += 3.0;

    double rad = game->voiture.angle * M_PI / 180.0;
    game->voiture.posx += game->voiture.vitesse * sin(rad);
    game->voiture.posy -= game->voiture.vitesse * cos(rad);

    SDL_FRect car_rect = { game->voiture.posx, game->voiture.posy, 
                           game->voiture.width, game->voiture.height };

    // --- Gestion des Bornes en temps réel (Visuel uniquement) ---
    for (int i = 0; i < game->parking.nb_spots; i++) {
        ParkingSpot *spot = &game->parking.spots[i];

        // La borne passe au rouge si la voiture est sur la place, sinon vert
        if (check_collision(car_rect, spot->rect)) {
            spot->occupied = true;
        } else {
            spot->occupied = false; 
        }
        // NOTE: La détection de collision avec spot->borne a été supprimée ici
    }

    // --- Logique du Score (Validation sur la cible jaune) ---
    if (game->target_spot_index != -1) {
        ParkingSpot *target = &game->parking.spots[game->target_spot_index];
        
        // Si on est sur la cible et qu'on s'arrête
        if (check_collision(car_rect, target->rect) && SDL_fabsf(game->voiture.vitesse) < 0.1f) {
            game->score++;
            reset_voiture_position(game);
            pick_next_target(game);
            
            // On réinitialise l'état visuel pour la nouvelle cible
            for(int j=0; j < game->parking.nb_spots; j++) {
                game->parking.spots[j].occupied = false;
            }
        }
    }
}

void render_game(Window *window, Game *game) {
    afficher_parking_sdl(window->renderer, &game->parking);

    // 2. Mettre à jour le titre avec le score
    char titre[50];
    sprintf(titre, "Parking Simulator - Score: %d", game->score);
    SDL_SetWindowTitle(window->SDL_window, titre);

    // 3. Dessiner la cible jaune
    if (game->target_spot_index != -1) {
        SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(window->renderer, 255, 255, 0, 100);
        SDL_RenderFillRect(window->renderer, &game->parking.spots[game->target_spot_index].rect);
    }

    // Rendu de la voiture
    VEHICULE *v = &game->voiture;
    SDL_FRect dest = { v->posx, v->posy, v->width, v->height };
    SDL_FPoint center = { v->width / 2.0f, v->height / 2.0f };
    SDL_RenderTextureRotated(window->renderer, v->texture, NULL, &dest, v->angle, &center, SDL_FLIP_NONE);
}

void reset_game(Game *game) {
    reset_voiture_position(game);
    for (int i = 0; i < game->parking.nb_spots; i++) {
        game->parking.spots[i].occupied = false;
    }
    pick_next_target(game);
}

void destroy_game(Game *game) {
    destroy_voiture(&game->voiture);
}