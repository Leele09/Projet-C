#ifndef VOITURE_H
#define VOITURE_H

typedef struct voiture VEHICULE;

struct voiture {
    char direction;       /* N => Nord, S => Sud, E => EST, O => OUEST */
    int posx;             /* Position courante coin haut gauche x */
    int posy;             /* Position courante coin haut gauche y */
    int vitesse;          /* Vitesse du véhicule */
    char alignement;      /* g => gauche ou d => droite */
    char type;            /* v => voiture, c => camion, etc. */
    char Carrosserie[4][30]; /* Représentation du véhicule */
    int code_couleur;     /* Couleur d’affichage */
    char etat;            /* 1 => actif, 0 => inactif */
    unsigned long int tps; /* Temps passé dans le parking */
    struct voiture *NXT;  /* Pointeur vers la voiture suivante */
};

#endif
