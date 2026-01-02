#include "vehicule.h"

VEHICULE init_voiture(SDL_Renderer* renderer, float x, float y, double angle) {
    VEHICULE v;

    v.posx = x;
    v.posy = y;
    v.vitesse = 0.0f;

    v.is_accelerating = false;
    v.is_reversing = false;
    v.is_turning_left = false;
    v.is_turning_right = false;

    v.width = 25.0f;
    v.height = 45.0f;
    v.angle = angle;
    
    v.texture = IMG_LoadTexture(renderer, "assets/car.png");
    
    if (!v.texture) {
        SDL_Log("Erreur chargement voiture : %s", SDL_GetError());
    }

    return v;
}
bool check_collision(SDL_FRect rect1, SDL_FRect rect2) {
    return SDL_HasRectIntersectionFloat(&rect1, &rect2);
}

void destroy_voiture(VEHICULE *v) {
    if (v->texture) {
        SDL_DestroyTexture(v->texture);
        v->texture = NULL;
    }
}