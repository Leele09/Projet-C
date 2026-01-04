#include "vehicule.h"
#include <math.h>

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
    
    v.texture = IMG_LoadTexture(renderer, "assets/Cars/Compact/compact_green.png");
    
    if (!v.texture) {
        SDL_Log("Erreur chargement voiture : %s", SDL_GetError());
    }

    return v;
}

VEHICULE init_pnj(SDL_Renderer* renderer, 
                  float x, 
                  float y, 
                  float vitesse, 
                  bool is_accelerating, 
                  bool is_reversing, 
                  bool is_turning_left, 
                  bool is_turning_right, 
                  double angle, 
                  const char* path_texture) {
    VEHICULE pnj;

    pnj.posx = x;
    pnj.posy = y;
    pnj.vitesse = vitesse;

    pnj.is_accelerating = is_accelerating;
    pnj.is_reversing = is_reversing;
    pnj.is_turning_left = is_turning_left;
    pnj.is_turning_right = is_turning_right;

    pnj.width = 25.0f;
    pnj.height = 45.0f;
    pnj.angle = angle;
    
    pnj.texture = IMG_LoadTexture(renderer, path_texture);
    
    if (!pnj.texture) {
        SDL_Log("Erreur chargement voiture : %s", SDL_GetError());
    }

    return pnj;
}

bool is_position_valid(float x, float y, VEHICULE *existing_pnjs, int count, float min_dist) {
    for (int i = 0; i < count; i++) {
        float other_x = existing_pnjs[i].posx;
        float other_y = existing_pnjs[i].posy;

        // Calcul de la distance entre le point candidat et le PNJ existant
        float dx = x - other_x;
        float dy = y - other_y;
        float distance = sqrt(dx*dx + dy*dy);

        // Si la distance est inférieure au minimum requis, la position n'est pas valide
        if (distance < min_dist) {
            return false; 
        }
    }
    return true;
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