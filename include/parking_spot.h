#ifndef PARKING_SPOT_H
#define PARKING_SPOT_H

#include <stdbool.h>
#include <SDL3/SDL.h>
#include "borne.h"

typedef struct {
    bool occupied;
    bool borne_active;   
    SDL_FRect rect;
    SDL_FRect borne;
} ParkingSpot;

#endif