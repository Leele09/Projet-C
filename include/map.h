#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <SDL3/SDL.h>
#include "voiture.h"

#define NB_PLACES 8

typedef struct {
    bool occupied;
    bool borne_active;
    SDL_FRect rect;   // place
    SDL_FRect borne;  // borne à l'arrière
} ParkingSpot;

typedef struct {
    ParkingSpot spots[NB_PLACES * 8];
    SDL_FRect contour;
    SDL_FRect entree;
    SDL_FRect sortie;

    // Positions calculées pour l'affichage
    float left_x;
    float center_x;
    float right_x;
    float y_top;
    float y_bottom;
    float place_w;

    int nb_spots;  // ✅ nombre total de places générées
} Parking;

void init_parking(Parking *p, float window_w, float window_h);
void afficher_parking_sdl(SDL_Renderer *renderer, const Parking *p);
void update_borne(Parking *p, VEHICULE *v);
void free_parking(Parking *p);

#endif
