#include "game_over.h"

GameOverScreen init_game_over(Window *window) {
    GameOverScreen screen;
    
    // Chargement des polices
    screen.titleFont = TTF_OpenFont("assets/Roboto.ttf", 64); // Gros titre
    screen.buttonFont = TTF_OpenFont("assets/Roboto.ttf", 32); // Boutons

    if (!screen.titleFont || !screen.buttonFont) {
        SDL_Log("Erreur chargement police Game Over: %s", SDL_GetError());
    }

    // Création des boutons centrés
    int w = window->width;
    int h = window->height;
    int btn_w = 250;
    int btn_h = 70;

    SDL_Color blue = {50, 100, 200, 255};
    SDL_Color gray = {100, 100, 100, 255};

    screen.restartButton = create_button(
        (w - btn_w) / 2.0f, h / 2.0f, 
        btn_w, btn_h, 
        blue, "Rejouer", screen.buttonFont, window->renderer
    );

    screen.menuButton = create_button(
        (w - btn_w) / 2.0f, (h / 2.0f) + 100, 
        btn_w, btn_h, 
        gray, "Menu Principal", screen.buttonFont, window->renderer
    );

    return screen;
}

void render_game_over(Window *window, GameOverScreen *screen, int final_score) {
    // Filtre sombre semi-transparent
    SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 180);
    SDL_Rect screen_rect = {0, 0, window->width, window->height};
    SDL_RenderFillRect(window->renderer, (SDL_FRect*)&screen_rect);

    // Texte "GAME OVER"
    SDL_Color red = {220, 50, 50, 255};
    SDL_Surface *surf = TTF_RenderText_Blended(screen->titleFont, "GAME OVER", 0, red);
    if (surf) {
        SDL_Texture *txt = SDL_CreateTextureFromSurface(window->renderer, surf);
        SDL_FRect dst = { (window->width - surf->w)/2.0f, 150, surf->w, surf->h };
        SDL_RenderTexture(window->renderer, txt, NULL, &dst);
        SDL_DestroySurface(surf);
        SDL_DestroyTexture(txt);
    }

    // Texte du Score
    char scoreText[50];
    sprintf(scoreText, "Score Final : %d", final_score);
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surfScore = TTF_RenderText_Blended(screen->buttonFont, scoreText, 0, white);
    if (surfScore) {
        SDL_Texture *txtScore = SDL_CreateTextureFromSurface(window->renderer, surfScore);
        SDL_FRect dstScore = { (window->width - surfScore->w)/2.0f, 250, surfScore->w, surfScore->h };
        SDL_RenderTexture(window->renderer, txtScore, NULL, &dstScore);
        SDL_DestroySurface(surfScore);
        SDL_DestroyTexture(txtScore);
    }

    // Boutons
    render_button(window->renderer, &screen->restartButton);
    render_button(window->renderer, &screen->menuButton);
}

GameOverAction handle_game_over_events(GameOverScreen *screen, SDL_Event *event) {
    
    // Gestion Restart
    handle_button_event(&screen->restartButton, event);
    if (screen->restartButton.clicked) {
        screen->restartButton.clicked = false;
        return ACTION_RESTART;
    }

    // Gestion Menu
    handle_button_event(&screen->menuButton, event);
    if (screen->menuButton.clicked) {
        screen->menuButton.clicked = false;
        return ACTION_MENU;
    }

    return ACTION_NONE;
}

void destroy_game_over(GameOverScreen *screen) {
    // On détruit les boutons (textures internes)
    destroy_button(&screen->restartButton);
    destroy_button(&screen->menuButton);

    // On détruit les polices
    if (screen->titleFont) TTF_CloseFont(screen->titleFont);
    if (screen->buttonFont) TTF_CloseFont(screen->buttonFont);
}