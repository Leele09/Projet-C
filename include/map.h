#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <SDL3/SDL.h>
#include "voiture.h"

#define NB_PLACES 8

typedef struct {
    SDL_FRect rect;       // position de la borne
} Borne;  // éventuellement on peut supprimer ce struct et utiliser directement SDL_FRect

typedef struct {
    bool occupied;
    bool borne_active;   // reste pour collisions
    SDL_FRect rect;      // place
    SDL_FRect borne;     // borne à gauche ou à droite
} ParkingSpot;

typedef struct {
    SDL_FRect contour;

    SDL_FRect entree1;
    SDL_FRect entree2;
    SDL_FRect sortie1;
    SDL_FRect sortie2;

    ParkingSpot spots[NB_PLACES * 8];

    // Positions calculées pour l'affichage
    float y_top;
    float y_bottom;
    float place_w;

    int nb_spots;
} Parking;

void init_parking(Parking *p, float window_w, float window_h);
void afficher_parking_sdl(SDL_Renderer *renderer, const Parking *p);
void update_borne(Parking *p, VEHICULE *v);

#endif
