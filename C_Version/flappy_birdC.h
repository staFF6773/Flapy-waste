#ifndef FLAPPY_BIRD_H
#define FLAPPY_BIRD_H

struct Bird {
    int x, y;
    int velocity;
};

struct Pipe {
    int x, gapPosition;
};

void clearScreen();
int kbhit();

#endif
