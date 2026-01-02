#ifndef VEHICULE_H
#define VEHICULE_H

#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#define START_X 1100.0f
#define START_Y 610.0f
#define START_ANGLE 270.0

typedef struct {
    float posx;
    float posy;
    float vitesse;
    
    bool is_accelerating;
    bool is_reversing;
    bool is_turning_left;
    bool is_turning_right;

    SDL_Texture* texture;
    double angle;        
    float width;         
    float height;

} VEHICULE;

VEHICULE init_voiture(SDL_Renderer* renderer, float x, float y, double start_angle);
void destroy_voiture(VEHICULE *v);
bool check_collision(SDL_FRect rect1, SDL_FRect rect2);
#endif