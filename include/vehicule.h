#ifndef VEHICULE_H
#define VEHICULE_H

#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#define START_X 1100.0f
#define START_Y 610.0f
#define START_ANGLE 270.0

static const char *ALL_CAR_TEXTURES[] = {
    // Compact
    "assets/Cars/Compact/compact_blue.png",
    "assets/Cars/Compact/compact_green.png",
    "assets/Cars/Compact/compact_orange.png",
    "assets/Cars/Compact/compact_red.png",
    // Coupe
    "assets/Cars/Coupe/coupe_blue.png",
    "assets/Cars/Coupe/coupe_green.png",
    "assets/Cars/Coupe/coupe_midnight.png",
    "assets/Cars/Coupe/coupe_red.png",
    // Sedan
    "assets/Cars/Sedan/sedan_blue.png",
    "assets/Cars/Sedan/sedan_gray.png",
    "assets/Cars/Sedan/sedan_green.png",
    "assets/Cars/Sedan/sedan_red.png",
    // Sport
    "assets/Cars/Sport/sport_blue.png",
    "assets/Cars/Sport/sport_green.png",
    "assets/Cars/Sport/sport_red.png",
    "assets/Cars/Sport/sport_yellow.png",
    // Truck
    "assets/Cars/Truck/truck_blue.png",
    "assets/Cars/Truck/truck_cream.png",
    "assets/Cars/Truck/truck_green.png",
    "assets/Cars/Truck/truck_red.png"
};

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
VEHICULE init_pnj(SDL_Renderer* renderer, 
                  float x, 
                  float y, 
                  float vitesse, 
                  bool is_accelerating, 
                  bool is_reversing, 
                  bool is_turning_left, 
                  bool is_turning_right, 
                  double angle, 
                  const char* path_texture);
bool is_position_valid(float x, float y, VEHICULE *pnjs, int nb_pnjs, float safe_distance);
void destroy_voiture(VEHICULE *v);
bool check_collision(SDL_FRect rect1, SDL_FRect rect2);
#endif