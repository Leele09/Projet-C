#include "parking.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

Parking init_parking(float window_w, float window_h) {
    Parking p;

    float margin_w = window_w * 0.05f;
    float margin_h = window_h * 0.05f;

    p.contour.x = margin_w;
    p.contour.y = margin_h;
    p.contour.w = window_w - 2 * margin_w;
    p.contour.h = window_h - 2 * margin_h;

    p.y_top = p.contour.y;
    p.y_bottom = p.contour.y + p.contour.h;
    p.place_w = p.contour.w * 0.07f;

    float barriere_w = 8, barriere_h = 60, espace_barriere = 20;

    p.entree1 = (SDL_FRect){p.contour.x + p.contour.w - barriere_w - 5,
                             p.contour.y + p.contour.h - barriere_h - 5,
                             barriere_w, barriere_h};
    p.entree2 = (SDL_FRect){p.entree1.x - espace_barriere, p.entree1.y,
                            barriere_w, barriere_h};
    p.sortie1 = (SDL_FRect){p.contour.x + 5, p.contour.y + 5, barriere_w, barriere_h};
    p.sortie2 = (SDL_FRect){p.sortie1.x + espace_barriere, p.sortie1.y, barriere_w, barriere_h};

    int idx = 0;
    int nb_places_par_colonne = NB_PLACES;
    int nb_colonnes = 8;
    float hauteur_place = p.contour.h * 0.045f;
    float largeur_place = p.contour.w * 0.07f;
    float borne_hauteur = largeur_place * 0.08f;

    float y_top_colonnes = p.y_top + p.contour.h * 0.05f;
    float y_bottom_colonnes = p.y_bottom - p.contour.h * 0.05f;

    int nb_espaces = 4;
    float total_columns_width = nb_colonnes * largeur_place;
    float espace_total = p.contour.w - total_columns_width;
    float espace = espace_total / nb_espaces;

    float x = p.contour.x;

    // Colonnes avec bornes alternées
    create_parking_column(&p, &idx, x, y_top_colonnes, y_bottom_colonnes,
                          nb_places_par_colonne, largeur_place, hauteur_place,
                          borne_hauteur, true);
    x += largeur_place + espace;

    create_parking_column(&p, &idx, x, y_top_colonnes, y_bottom_colonnes,
                          nb_places_par_colonne, largeur_place, hauteur_place,
                          borne_hauteur, false);
    x += largeur_place;
    create_parking_column(&p, &idx, x, y_top_colonnes, y_bottom_colonnes,
                          nb_places_par_colonne, largeur_place, hauteur_place,
                          borne_hauteur, true);
    x += largeur_place + espace;

    create_parking_column(&p, &idx, x, y_top_colonnes, y_bottom_colonnes,
                          nb_places_par_colonne, largeur_place, hauteur_place,
                          borne_hauteur, false);
    x += largeur_place;
    create_parking_column(&p, &idx, x, y_top_colonnes, y_bottom_colonnes,
                          nb_places_par_colonne, largeur_place, hauteur_place,
                          borne_hauteur, true);
    x += largeur_place + espace;

    create_parking_column(&p, &idx, x, y_top_colonnes, y_bottom_colonnes,
                          nb_places_par_colonne, largeur_place, hauteur_place,
                          borne_hauteur, false);
    x += largeur_place;
    create_parking_column(&p, &idx, x, y_top_colonnes, y_bottom_colonnes,
                          nb_places_par_colonne, largeur_place, hauteur_place,
                          borne_hauteur, true);
    x += largeur_place + espace;

    create_parking_column(&p, &idx, x, y_top_colonnes, y_bottom_colonnes,
                          nb_places_par_colonne, largeur_place, hauteur_place,
                          borne_hauteur, false);

    p.nb_spots = idx;

    return p;
}

void create_parking_column(Parking *p, int *index_debut,
                           float x_base, float y_top, float y_bottom,
                           int nb_places,
                           float largeur_place,
                           float hauteur_place,
                           float borne_hauteur,
                           bool borne_a_gauche){
    float margin = (y_bottom - y_top) * 0.1f;
    float total_height = y_bottom - y_top - 2 * margin;
    float espace_vertical = (total_height - nb_places * hauteur_place) / (nb_places - 1);

    for (int i = 0; i < nb_places; i++) {
        int idx = (*index_debut)++;
        float y = y_top + margin + i * (hauteur_place + espace_vertical);

        p->spots[idx].rect.x = x_base;
        p->spots[idx].rect.y = y;
        p->spots[idx].rect.w = largeur_place;
        p->spots[idx].rect.h = hauteur_place;
        p->spots[idx].occupied = false;
        p->spots[idx].borne_active = false;

        if (borne_a_gauche) {
            p->spots[idx].borne.x = x_base; 
        } else {
            p->spots[idx].borne.x = x_base + largeur_place - borne_hauteur;
        }
        p->spots[idx].borne.y = y + 2; 
        p->spots[idx].borne.w = borne_hauteur;
        p->spots[idx].borne.h = hauteur_place - 4;
    }
}

void afficher_parking_sdl(SDL_Renderer *renderer, const Parking *p) {
    SDL_Color asphalt = {45, 50, 55, 255};
    SDL_Color white   = {250, 250, 250, 255};
    SDL_Color vert    = {60, 200, 90, 255};
    SDL_Color bleu    = {0, 120, 255, 255};
    SDL_Color rouge = {255, 0, 0, 255};

    SDL_SetRenderDrawColor(renderer, asphalt.r, asphalt.g, asphalt.b, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
    SDL_RenderRect(renderer, &p->contour);

    SDL_SetRenderDrawColor(renderer, bleu.r, bleu.g, bleu.b, 255);
    SDL_RenderFillRect(renderer, &p->entree1);
    SDL_RenderFillRect(renderer, &p->entree2);
    SDL_RenderFillRect(renderer, &p->sortie1);
    SDL_RenderFillRect(renderer, &p->sortie2);

    for (int i = 0; i < p->nb_spots; i++) {
        const ParkingSpot *spot = &p->spots[i];

        if (spot->occupied) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 60, 200, 90, 255);
        }
        SDL_RenderFillRect(renderer, &spot->borne);
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
        SDL_RenderRect(renderer, &spot->rect);
    }

    int nb_colonnes = 8;
    int nb_espaces = 4;
    float trait_w = 4;
    float trait_h = 20;
    float espace_h = 15;

    float colonnes_x[nb_colonnes];
    for(int i=0; i<nb_colonnes; i++){
        colonnes_x[i] = p->spots[i * NB_PLACES].rect.x;
    }

    int allee_indices[4][2] = {
        {0, 1},
        {2, 3},
        {4, 5},
        {6, 7}
    };

    for(int a=0; a<nb_espaces; a++){
        int idx_debut = allee_indices[a][0] * NB_PLACES;
        int idx_droite = allee_indices[a][1] * NB_PLACES;
        int idx_fin   = allee_indices[a][1] * NB_PLACES + NB_PLACES - 1;

        float x1 = p->spots[idx_debut].rect.x;
        float x2 = p->spots[idx_droite].rect.x;
        float largeur1 = p->spots[idx_debut].rect.w;
        float x_trait = (x1 + largeur1) + (x2 - (x1 + largeur1)) / 2.0f - (trait_w / 2.0f);
        
        float y_top_trait = p->spots[idx_debut].rect.y;
        float y_bottom_trait = p->spots[idx_fin].rect.y + p->spots[idx_fin].rect.h;

        for(float y = y_top_trait; y < y_bottom_trait; y += trait_h + espace_h) {
            SDL_FRect segment = {x_trait, y, trait_w, trait_h};
            SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
            SDL_RenderFillRect(renderer, &segment);
        }
    }
}
