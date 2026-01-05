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
        SDL_Log("Erreur chargement PNJ : %s", SDL_GetError());
    }

    return pnj;
}

const char* get_random_car_texture_path() {
    int nb_textures = sizeof(ALL_CAR_TEXTURES) / sizeof(ALL_CAR_TEXTURES[0]);
    int index = rand() % nb_textures;
    return ALL_CAR_TEXTURES[index];
}

void add_pnj_node(PNJNode **head, VEHICULE v) {
    PNJNode *new_node = (PNJNode*)malloc(sizeof(PNJNode));
    if (new_node) {
        new_node->vehicule = v;
        new_node->next = *head;
        *head = new_node;
    }
}

void free_pnj_list(PNJNode *head) {
    PNJNode *current = head;
    while (current != NULL) {
        PNJNode *next = current->next;
        
        // Si la texture est unique au PNJ, on la détruit ici
        if (current->vehicule.texture) {
            SDL_DestroyTexture(current->vehicule.texture); 
        }
        
        free(current);
        current = next;
    }
}

bool is_position_valid_list(float x, float y, PNJNode *head, float min_dist) {
    PNJNode *current = head;
    while (current != NULL) {
        float dx = x - current->vehicule.posx;
        float dy = y - current->vehicule.posy;
        if (sqrt(dx*dx + dy*dy) < min_dist) return false;
        
        current = current->next;
    }
    return true;
}

void init_pnjs_list(PNJNode **list_head, int *nb_pnjs, SDL_Renderer *renderer, int nb_to_spawn) {
    *list_head = NULL;
    *nb_pnjs = 0;

    float safe_distance = 80.0f;

    for (int i = 0; i < nb_to_spawn; i++) {
        float posx, posy;
        bool valid_pos = false;
        int attempts = 0;

        while (!valid_pos && attempts < 100) {
            posx = 100 + rand() % 800;
            posy = 100 + rand() % 500;
            // On vérifie par rapport à la liste actuelle
            if (is_position_valid_list(posx, posy, *list_head, safe_distance)) {
                valid_pos = true;
            }
            attempts++;
        }

        if (valid_pos) {
            float angle = rand() % 360;
            const char* path = get_random_car_texture_path();
            
            // Création du véhicule temporaire
            VEHICULE v = init_pnj(renderer, posx, posy, 2.0f, true, false, false, false, angle, path);
            
            // Ajout à la liste chaînée
            add_pnj_node(list_head, v);
            (*nb_pnjs)++;
        }
    }
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