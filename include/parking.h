#ifndef PARKING_H
#define PARKING_H

#include <SDL3/SDL.h>
#include "parking_spot.h"

#define NB_PLACES 8

typedef struct {
    SDL_FRect contour;

    SDL_FRect entree1;
    SDL_FRect entree2;
    SDL_FRect sortie1;
    SDL_FRect sortie2;

    ParkingSpot spots[NB_PLACES * 8]; 

    float y_top;
    float y_bottom;
    float place_w;

    int nb_spots;
} Parking;

Parking init_parking(float window_w, float window_h);
void afficher_parking_sdl(SDL_Renderer *renderer, const Parking *p);
void create_parking_column(Parking *p, int *index_debut,
                           float x_base, float y_top, float y_bottom,
                           int nb_places,
                           float largeur_place,
                           float hauteur_place,
                           float borne_hauteur,
                           bool borne_a_gauche);

#endif
