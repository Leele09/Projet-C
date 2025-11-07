#include "map.h"
#include <stdlib.h>
#include <math.h>

// --- Création d'une colonne de places ---
void create_parking_column(Parking *p, int *index_debut,
                           float x_base, float y_top, float y_bottom,
                           int nb_places,
                           float largeur_place,
                           float hauteur_place,
                           float borne_offset, float borne_hauteur)
{
    // Petite marge en haut/bas
    float margin = (y_bottom - y_top) * 0.03f;
    float total_height = y_bottom - y_top - 2 * margin;

    // Espacement vertical entre les places
    float espace_vertical = (total_height - nb_places * hauteur_place) / (nb_places - 1);

    for (int i = 0; i < nb_places; i++) {
        int idx = (*index_debut)++;
        float y = y_top + margin + i * (hauteur_place + espace_vertical);

        // Place
        p->spots[idx].rect.x = x_base;
        p->spots[idx].rect.y = y;
        p->spots[idx].rect.w = largeur_place;
        p->spots[idx].rect.h = hauteur_place;

        // Borne à l’arrière
        p->spots[idx].borne.x = x_base - borne_offset;
        p->spots[idx].borne.y = y;
        p->spots[idx].borne.w = borne_hauteur;
        p->spots[idx].borne.h = hauteur_place;

        p->spots[idx].borne_active = false;
        p->spots[idx].occupied = false;
    }
}

// --- Initialisation du parking ---
void init_parking(Parking *p, float window_w, float window_h) {
    // --- Marges globales ---
    float margin_w = window_w * 0.05f;
    float margin_h = window_h * 0.05f;

    // --- Contour du parking ---
    p->contour.x = margin_w;
    p->contour.y = margin_h;
    p->contour.w = window_w - 2 * margin_w;
    p->contour.h = window_h - 2 * margin_h;

    p->y_top = p->contour.y;
    p->y_bottom = p->contour.y + p->contour.h;

    // --- Entrée / Sortie ---
    p->sortie = (SDL_FRect){p->contour.x - 60, p->y_top + p->contour.h * 0.45f, 50, 25};
    p->entree = (SDL_FRect){p->contour.x + p->contour.w + 10, p->y_top + p->contour.h * 0.45f, 50, 25};

    // --- Dimensions ajustées ---
    int nb_colonnes = 8;
    int nb_places_par_colonne = NB_PLACES;

    float largeur_zone_places = p->contour.w * 0.85f;   // zone utilisable réduite
    float hauteur_zone_places = p->contour.h * 0.9f;    // hauteur totale

    float largeur_place = largeur_zone_places / (nb_colonnes * 2.0f); // plus étroit
    float hauteur_place = hauteur_zone_places * 0.045f;               // plus court

    float borne_offset = largeur_place * 0.1f;
    float borne_hauteur = largeur_place * 0.08f;

    // --- Espacement entre colonnes ---
    float espace_entre_colonnes = largeur_place * 0.2f;

    // --- Centrage horizontal ---
    float total_width = nb_colonnes * largeur_place + (nb_colonnes - 1) * espace_entre_colonnes;
    float x_zone_debut = p->contour.x + (p->contour.w - total_width) / 2.0f;

    // --- Zone verticale (5 % de marge en haut et bas) ---
    float y_top_colonnes = p->y_top + p->contour.h * 0.05f;
    float y_bottom_colonnes = p->y_bottom - p->contour.h * 0.05f;

    // --- Création des colonnes ---
    int idx = 0;
    for (int c = 0; c < nb_colonnes; c++) {
        float x_col = x_zone_debut + c * (largeur_place + espace_entre_colonnes);
        create_parking_column(p, &idx, x_col, y_top_colonnes, y_bottom_colonnes,
                              nb_places_par_colonne, largeur_place,
                              hauteur_place, borne_offset, borne_hauteur);
    }

    p->nb_spots = idx;
}

// --- Affichage du parking ---
void afficher_parking_sdl(SDL_Renderer *renderer, const Parking *p) {
    SDL_Color asphalt = {45, 50, 55, 255};
    SDL_Color white   = {250, 250, 250, 255};
    SDL_Color vert    = {60, 200, 90, 255};
    SDL_Color rouge   = {230, 60, 60, 255};

    // Fond (asphalte)
    SDL_SetRenderDrawColor(renderer, asphalt.r, asphalt.g, asphalt.b, 255);
    SDL_RenderClear(renderer);

    // Contour du parking
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
    SDL_RenderRect(renderer, &p->contour);

    // Entrée / sortie
    SDL_SetRenderDrawColor(renderer, vert.r, vert.g, vert.b, 255);
    SDL_RenderFillRect(renderer, &p->entree);
    SDL_SetRenderDrawColor(renderer, rouge.r, rouge.g, rouge.b, 255);
    SDL_RenderFillRect(renderer, &p->sortie);

    // --- Places et bornes ---
    for (int i = 0; i < p->nb_spots; i++) {
        const ParkingSpot *spot = &p->spots[i];

        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
        SDL_RenderRect(renderer, &spot->rect);

        if (spot->borne_active) {
            SDL_SetRenderDrawColor(renderer, vert.r, vert.g, vert.b, 255);
            SDL_RenderFillRect(renderer, &spot->borne);
        }
    }
}

// --- Mise à jour des bornes ---
void update_borne(Parking *p, VEHICULE *v) {
    if (!v || v->etat == '0') return;

    SDL_FRect car = {v->posx, v->posy, 40, 20};

    for (int i = 0; i < p->nb_spots; i++) {
        ParkingSpot *spot = &p->spots[i];
        bool collision =
            car.x + car.w > spot->borne.x &&
            car.x < spot->borne.x + spot->borne.w &&
            car.y + car.h > spot->borne.y &&
            car.y < spot->borne.y + spot->borne.h;
        spot->borne_active = collision;
    }
}
