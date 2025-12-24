#ifndef DIRECTION_H
#define DIRECTION_H

typedef struct {
    int posx;
    int posy;
    int vitesse;
} VEHICULE;

char key_pressed();
void direction(VEHICULE *v);

#endif
