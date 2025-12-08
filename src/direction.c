#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <direction.h>

void init_terminal() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);

    ttystate.c_lflag &= ~ICANON; // mode caractère par caractère
    ttystate.c_lflag &= ~ECHO;   // désactive l'écho
    ttystate.c_cc[VMIN] = 0;     // lecture non bloquante
    ttystate.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

int kbhit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

char getch() {
    char c;
    if (read(STDIN_FILENO, &c, 1) < 0)
        return 0;
    return c;
}

char key_pressed(void) {
    if (kbhit()) {
        return getch(); 
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
