#ifndef VOITURE_H
#define VOITURE_H

typedef struct {
    char direction ; /*N => Nord, S => Sud, E => EST, O => OUEST*/
    int posx; /*Position courante coin haut gauche x de la voiture*/
    int posy; /*Position courante coin haut gauche y de la voiture*/
    int vitesse; /*Vitesse du véhicule*/
    char alignement; /*’g’=>gauche ou ’d’=>droite*/
    char type; /*’v’=>voiture, ’c’=>camion, etc.*/
    char Carrosserie [4][30]; /*Carrosserie de la voiture, servira pour
    l’affichage du véhicule à tout moment*/
    int code_couleur; /*Code couleur de la voiture à utiliser lors de
    l’affichage*/
    char etat; /*État du véhicule : ’1’ => actif et ’0’ => inactif*/
    unsigned long int tps;/*pour stocker le temps passé dans le parking*/
    struct voiture * NXT; /*Pointeur vers une prochaine voiture,
    nécessaire pour la liste chaînée*/
    /*Vous pouvez rajouter d’autres variables si nécessaire */
} Voiture;

#endif
