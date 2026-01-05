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

    init_pnjs(&game, window->renderer);

    pick_next_target(&game);

    return game;
}

void init_pnjs(Game *game, SDL_Renderer *renderer) {
    int nb_textures_disponibles = 20; // Nombre de voitures disponibles
    float safe_distance = 120.0f;

    for (int i = 0; i < game->nb_pnjs; i++) {
        float posx, posy;
        bool valid_pos = false;
        int attempts = 0;

        while (!valid_pos && attempts < 100) {
            // Génération candidat
            posx = 100 + rand() % 800; 
            posy = 100 + rand() % 500;

            // Vérification
            // On passe le tableau game->pnjs et 'i' qui est le nombre de voitures DÉJÀ créées
            if (is_position_valid(posx, posy, game->pnjs, i, safe_distance)) {
                valid_pos = true;
            }
            attempts++;
        }

        // Si après 100 tentatives on n'a pas trouvé de place valide, on log une erreur
        if (valid_pos) {
            float angle = rand() % 360;
            int random_index = rand() % nb_textures_disponibles;
            
            game->pnjs[i] = init_pnj(
                renderer,
                posx,
                posy,
                2.0f,  // Vitesse
                true,  // is_accelerating
                false, // is_reversing
                false, // is_turning_left
                false, // is_turning_right
                angle,
                ALL_CAR_TEXTURES[random_index]
            );
        } else {
            SDL_Log("Attention: Impossible de trouver une place libre pour le PNJ %d", i);
        }
    }
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

void update_game(Game *game, GameState *state) {
    // --- Mouvement ---
    if (game->voiture.is_accelerating) game->voiture.vitesse = 2.0f;
    else if (game->voiture.is_reversing) game->voiture.vitesse = -1.5f;
    else game->voiture.vitesse = 0.0f;

    if (game->voiture.is_turning_left) game->voiture.angle -= 3.0;
    if (game->voiture.is_turning_right) game->voiture.angle += 3.0;

    double rad = game->voiture.angle * M_PI / 180.0;
    game->voiture.posx += game->voiture.vitesse * sin(rad);
    game->voiture.posy -= game->voiture.vitesse * cos(rad);

    float min_x = game->parking.contour.x;
    float min_y = game->parking.contour.y;
    float max_x = game->parking.contour.x + game->parking.contour.w;
    float max_y = game->parking.contour.y + game->parking.contour.h;

    // Mur GAUCHE
    if (game->voiture.posx < min_x) {
        game->voiture.posx = min_x; // On bloque la position
    } 
    // Mur DROITE (Attention à la largeur de la voiture)
    else if (game->voiture.posx + game->voiture.width > max_x) {
        game->voiture.posx = max_x - game->voiture.width;
    }

    // Mur HAUT
    if (game->voiture.posy < min_y) {
        game->voiture.posy = min_y;
    }
    // Mur BAS (Attention à la hauteur de la voiture)
    else if (game->voiture.posy + game->voiture.height > max_y) {
        game->voiture.posy = max_y - game->voiture.height;
    }
    
    SDL_FRect car_rect = { game->voiture.posx, game->voiture.posy, 
                           game->voiture.width, game->voiture.height };

    // --- Mise à jour des PNJ ---
    update_pnjs(game, &game->parking);

    for (int i = 0; i < game->nb_pnjs; i++) {
        VEHICULE *pnj = &game->pnjs[i];
        SDL_FRect rect_pnj = { pnj->posx, pnj->posy, pnj->width, pnj->height };

        if (check_collision(car_rect, rect_pnj)) {
            *state = STATE_GAMEOVER;
        }
    }

    // --- Gestion des Bornes en temps réel (Visuel uniquement) ---
    for (int i = 0; i < game->parking.nb_spots; i++) {
        ParkingSpot *spot = &game->parking.spots[i];

        // La borne passe au rouge si la voiture est sur la place, sinon vert
        if (check_collision(car_rect, spot->rect)) {
            spot->occupied = true;
        } else {
            spot->occupied = false; 
        }
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

void update_pnjs(Game *game, Parking *parking) {
    // Limites claires du parking pour simplifier la lecture
    float min_x = parking->contour.x;
    float min_y = parking->contour.y;
    float max_x = parking->contour.x + parking->contour.w;
    float max_y = parking->contour.y + parking->contour.h;

    for (int i = 0; i < game->nb_pnjs; i++) {
        VEHICULE *v = &game->pnjs[i];

        float old_x = v->posx;
        float old_y = v->posy;

        double rad = v->angle * M_PI / 180.0;
        v->posx += v->vitesse * sin(rad);
        v->posy -= v->vitesse * cos(rad);

        // Collisions
        bool changed_dir = false;
         
        // Axe Horizontal (Gauche / Droite)
        if (v->posx < min_x) {
            // Mur de GAUCHE
            v->posx = min_x;      // On repousse la voiture DANS le parking (Important !)
            v->angle = -v->angle; // Rebond miroir
            changed_dir = true;
        } 
        else if (v->posx + v->width > max_x) {
            // Mur de DROITE (On vérifie posx + width)
            v->posx = max_x - v->width; // On repousse pour que le côté droit ne dépasse pas
            v->angle = -v->angle;
            changed_dir = true;
        }
        
        // Axe Vertical (Haut / Bas)
        if (v->posy < min_y) {
            // Mur du HAUT
            v->posy = min_y;            // On repousse
            v->angle = 180 - v->angle;  // Rebond vertical
            changed_dir = true;
        }
        else if (v->posy + v->height > max_y) {
            // Mur du BAS (On vérifie posy + height)
            v->posy = max_y - v->height; // On repousse
            v->angle = 180 - v->angle;
            changed_dir = true;
        }

        SDL_FRect rect_v = { v->posx, v->posy, v->width, v->height };

        for (int j = 0; j < game->nb_pnjs; j++) {
            if (i == j) continue; // On ne se teste pas contre soi-même

            VEHICULE *other = &game->pnjs[j];
            SDL_FRect rect_other = { other->posx, other->posy, other->width, other->height };

            if (check_collision(rect_v, rect_other)) {
                // On annule le mouvement (retour à la position précédente)
                v->posx = old_x;
                v->posy = old_y;

                // On fait rebondir les deux voitures (changement d'angle)
                v->angle += 180 + (rand() % 60 - 30);
                other->angle += (rand() % 60 - 30);
            }
        }

        // Changement de direction aléatoire
        if (!changed_dir && (rand() % 100) < 2) {
            v->angle += (rand() % 90) - 45;
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

    // --- 4. Dessiner les PNJs ---
    for (int i = 0; i < game->nb_pnjs; i++) {
        VEHICULE *p = &game->pnjs[i];
        
        SDL_FRect dest_pnj = { p->posx, p->posy, p->width, p->height };
        SDL_FPoint center_pnj = { p->width / 2.0f, p->height / 2.0f };
        
        SDL_RenderTextureRotated(window->renderer, p->texture, NULL, &dest_pnj, p->angle, &center_pnj, SDL_FLIP_NONE);
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
    for (int i = 0; i < game->nb_pnjs; i++) {
        destroy_voiture(&game->pnjs[i]);
    }
}