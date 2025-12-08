#include <stdio.h>
#include <conio.h>
#include "direction.h"
char key_pressed(void); 

char key_pressed(void) {
    if (_kbhit()) {
        return _getch(); 
    }
    return 0;
}

void direction(VEHICULE *v) {
    char touch;
    int running = 1;

    while (running) {
        touch = key_pressed();

        switch (touch) {
            case 'z': v->posy -= v->vitesse; break;
            case 's': v->posy += v->vitesse; break;
            case 'q': v->posx -= v->vitesse; break;
            case 'd': v->posx += v->vitesse; break;
            case 'x': running = 0; break;
            default: break;
        }

        
        if (v->posx < 0) v->posx = 0;
        if (v->posy < 0) v->posy = 0;
//peut etre mettre un sleep
    }
}
