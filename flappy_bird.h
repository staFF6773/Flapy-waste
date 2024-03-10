#ifndef FLAPPY_BIRD_H
#define FLAPPY_BIRD_H

#include <vector>

struct Bird {
    int x, y;
    int velocity;

    Bird(int startX, int startY);

    void flap();

    void update();

    bool isOutsideBounds();

    bool isColliding(int pipeX, int gapPosition);
};

struct Pipe {
    int x, gapPosition;

    Pipe(int startX, int startGapPosition);

    bool isPastBird(int birdX);
};

void clearScreen();

bool kbhit();

#endif
