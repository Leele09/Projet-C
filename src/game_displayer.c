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

Game init_game(Window *window, GameMode mode) {
    Game game;

    game.currentMode = mode;
    if (mode == MODE_EASY) {
        game.nb_pnjs = MAX_PNJ_EASY;
    } else {
        game.nb_pnjs = MAX_PNJ_HARD;
    }

    game.score = 0;
    game.parking = init_parking((float)window->width, (float)window->height);
    game.voiture = init_voiture(window->renderer, START_X, START_Y, START_ANGLE);

    init_pnjs_list(&game.pnj_list, &game.nb_pnjs, window->renderer, game.nb_pnjs);
    pick_next_target(&game);

    return game;
}

void handle_game_events(Game *game, SDL_Event *event, GameState *state) {
    // Gestion des entrées clavier
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

void update_game(Game *game, GameState *state) {
    // Mouvement
    if (game->voiture.is_accelerating) game->voiture.vitesse = 2.0f;
    else if (game->voiture.is_reversing) game->voiture.vitesse = -1.5f;
    else game->voiture.vitesse = 0.0f;

    if (game->voiture.is_turning_left) game->voiture.angle -= 3.0;
    if (game->voiture.is_turning_right) game->voiture.angle += 3.0;

    double rad = game->voiture.angle * M_PI / 180.0;
    game->voiture.posx += game->voiture.vitesse * sin(rad);
    game->voiture.posy -= game->voiture.vitesse * cos(rad);

    // Collisions du joueur
    float min_x = game->parking.contour.x;
    float min_y = game->parking.contour.y;
    float max_x = game->parking.contour.x + game->parking.contour.w;
    float max_y = game->parking.contour.y + game->parking.contour.h;

    // Mur GAUCHE
    if (game->voiture.posx < min_x) {
        game->voiture.posx = min_x;
    } 
    // Mur DROITE
    else if (game->voiture.posx + game->voiture.width > max_x) {
        game->voiture.posx = max_x - game->voiture.width;
    }
    // Mur HAUT
    if (game->voiture.posy < min_y) {
        game->voiture.posy = min_y;
    }
    // Mur BAS
    else if (game->voiture.posy + game->voiture.height > max_y) {
        game->voiture.posy = max_y - game->voiture.height;
    }
    
    SDL_FRect car_rect = { game->voiture.posx, game->voiture.posy, 
                           game->voiture.width, game->voiture.height };

    // Mise à jour des PNJ
    update_pnjs(game, &game->parking);

    PNJNode *current = game->pnj_list;
    while (current != NULL) {
        VEHICULE *pnj = &current->vehicule;
        SDL_FRect rect_pnj = { pnj->posx, pnj->posy, pnj->width, pnj->height };

        if (check_collision(car_rect, rect_pnj)) {
            *state = STATE_GAMEOVER;
        }
        current = current->next;
    }

    // Gestion des Bornes en temps réel
    for (int i = 0; i < game->parking.nb_spots; i++) {
        ParkingSpot *spot = &game->parking.spots[i];

        // La borne passe au rouge si la voiture est sur la place, sinon vert
        if (check_collision(car_rect, spot->rect)) {
            spot->occupied = true;
        } else {
            spot->occupied = false; 
        }
    }

    // Logique du Score
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

void update_pnjs(Game *game, Parking *parking) {
    // Limites claires du parking
    float min_x = parking->contour.x;
    float min_y = parking->contour.y;
    float max_x = parking->contour.x + parking->contour.w;
    float max_y = parking->contour.y + parking->contour.h;

    PNJNode *it1 = game->pnj_list;
    
    while (it1 != NULL) {
        VEHICULE *v = &it1->vehicule; // Pointeur vers la voiture

        float old_x = v->posx;
        float old_y = v->posy;

        // Mouvement
        double rad = v->angle * M_PI / 180.0;
        v->posx += v->vitesse * sin(rad);
        v->posy -= v->vitesse * cos(rad);

        // COLLISIONS MURS
        bool changed_dir = false;
        if (v->posx < min_x) { v->posx = min_x; v->angle = -v->angle; changed_dir = true; }
        else if (v->posx + v->width > max_x) { v->posx = max_x - v->width; v->angle = -v->angle; changed_dir = true; }
        
        if (v->posy < min_y) { v->posy = min_y; v->angle = 180 - v->angle; changed_dir = true; }
        else if (v->posy + v->height > max_y) { v->posy = max_y - v->height; v->angle = 180 - v->angle; changed_dir = true; }

        // COLLISIONS ENTRE PNJ
        SDL_FRect rect_v = { v->posx, v->posy, v->width, v->height };
        
        PNJNode *it2 = game->pnj_list;
        while (it2 != NULL) {
            // On ne se compare pas à soi-même
            if (it1 != it2) {
                VEHICULE *other = &it2->vehicule;
                SDL_FRect rect_other = { other->posx, other->posy, other->width, other->height };

                if (check_collision(rect_v, rect_other)) {
                    v->posx = old_x;
                    v->posy = old_y;
                    v->angle += 180 + (rand() % 60 - 30);
                }
            }
            it2 = it2->next;
        }

        // Changement de direction aléatoire
        if (!changed_dir && (rand() % 100) < 2) {
            v->angle += (rand() % 90) - 45;
        }

        it1 = it1->next;
    }
}

void render_game(Window *window, Game *game) {
    afficher_parking_sdl(window->renderer, &game->parking);

    // Mettre à jour le titre avec le score
    char titre[50];
    sprintf(titre, "Parking Simulator - Score: %d", game->score);
    SDL_SetWindowTitle(window->SDL_window, titre);

    // Dessiner la cible jaune
    if (game->target_spot_index != -1) {
        SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(window->renderer, 255, 255, 0, 100);
        SDL_RenderFillRect(window->renderer, &game->parking.spots[game->target_spot_index].rect);
    }

    // Dessiner les PNJs
    PNJNode *current = game->pnj_list;
    while (current != NULL) {
        VEHICULE *p = &current->vehicule;
        SDL_FRect dest_pnj = { p->posx, p->posy, p->width, p->height };
        SDL_FPoint center_pnj = { p->width / 2.0f, p->height / 2.0f };
        
        SDL_RenderTextureRotated(window->renderer, p->texture, NULL, &dest_pnj, p->angle, &center_pnj, SDL_FLIP_NONE);
        
        current = current->next;
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
    free_pnj_list(game->pnj_list);
    game->pnj_list = NULL;
}