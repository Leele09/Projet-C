#include "cars.h"
#include "map.h"
#include <math.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

int game_loop(SDL_Renderer* renderer, Parking* parking)
{
    // Charger la voiture
    SDL_Texture* car = IMG_LoadTexture(renderer, "assets/car.png");
    if (!car) {
        SDL_Log("Erreur chargement voiture : %s", SDL_GetError());
        return 1;
    }

    // Position
    VEHICULE voiture = {500.0f, 350.0f, 0.0f, 'N'};
    float w = 25, h = 45;
    double angle = 0;

    bool avancer = false;
    bool reculer = false;

    bool running = true;
    SDL_Event e;

    while (running) {

        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_EVENT_QUIT)
                return 0;

            if (e.type == SDL_EVENT_KEY_DOWN) {
                switch (e.key.scancode) {

                    case SDL_SCANCODE_UP:
                        avancer = true;
                        reculer = false;
                        break;

                    case SDL_SCANCODE_DOWN:
                        reculer = true;
                        avancer = false;
                        break;

                    case SDL_SCANCODE_LEFT:
                        angle -= 5;
                        break;

                    case SDL_SCANCODE_RIGHT:
                        angle += 5;
                        break;

                    case SDL_SCANCODE_ESCAPE:
                        running = false;
                        break;
                }
            }

            if (e.type == SDL_EVENT_KEY_UP) {
                switch (e.key.scancode) {
                    case SDL_SCANCODE_UP:
                        avancer = false;
                        break;

                    case SDL_SCANCODE_DOWN:
                        reculer = false;
                        break;
                }
            }
        }

        // --- MOUVEMENT ---  
        if (avancer) voiture.vitesse = 2.0f;
        else if (reculer) voiture.vitesse = -1.5f;
        else voiture.vitesse = 0.0f;

        double rad = angle * M_PI / 180.0;

        voiture.posx += voiture.vitesse * sin(rad);
        voiture.posy -= voiture.vitesse * cos(rad);

        printf("x = %.1f   y = %.1f\n", voiture.posx, voiture.posy);

        // --- AFFICHAGE ---
        afficher_parking_sdl(renderer, parking);

        SDL_FRect dest = { voiture.posx, voiture.posy, w, h };
        SDL_FPoint center = { w/2.0f, h/2.0f };

        SDL_RenderTextureRotated(renderer, car, NULL, &dest, angle, &center, SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(car);
    return 0;
}
