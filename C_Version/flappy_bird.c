#include "flappy_bird.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define WIDTH 50
#define HEIGHT 20
#define BIRD '>'
#define PIPE '#'
#define PIPE_WIDTH 5
#define PIPE_GAP 8
#define GRAVITY 1
#define FLAP_FORCE -2
#define TICK_DELAY 100000

void clearScreen() {
    printf("\033[H\033[J");
}

int kbhit() {
    // Implementación de kbhit
}

int main() {
    srand(time(0));

    struct Bird bird = {WIDTH / 3, HEIGHT / 2, 0};
    struct Pipe pipes[100];
    int numPipes = 0;
    int tick = 0;
    int score = 0;
    int gameOver = 0;

    while (!gameOver) {
        clearScreen();

        if (tick % 20 == 0) {
            int gapPosition = rand() % (HEIGHT - PIPE_GAP);
            pipes[numPipes].x = WIDTH;
            pipes[numPipes].gapPosition = gapPosition;
            numPipes++;
        }

        for (int i = 0; i < numPipes; i++) {
            pipes[i].x--;

            if (bird.x == pipes[i].x && (bird.y < pipes[i].gapPosition || bird.y >= pipes[i].gapPosition + PIPE_GAP)) {
                gameOver = 1;
            }

            if (pipes[i].x + PIPE_WIDTH < 0) {
                for (int j = i; j < numPipes - 1; j++) {
                    pipes[j] = pipes[j + 1];
                }
                numPipes--;
                i--;
                score++;
            }
        }

        bird.velocity += GRAVITY;
        bird.y += bird.velocity;

        for (int i = 0; i < numPipes; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                if (j < pipes[i].gapPosition || j >= pipes[i].gapPosition + PIPE_GAP) {
                    if (pipes[i].x >= 0 && pipes[i].x < WIDTH) {
                        printf("%*s%c\n", pipes[i].x, "", PIPE);
                    }
                } else {
                    if (pipes[i].x >= 0 && pipes[i].x < WIDTH) {
                        printf("%*s%.*s\n", pipes[i].x, "", PIPE_WIDTH, "#####");
                    }
                }
            }
        }

        if (bird.y <= 0 || bird.y >= HEIGHT) {
            gameOver = 1;
        }

        if (kbhit()) {
            char input = getchar();
            if (input == ' ') {
                bird.velocity = FLAP_FORCE;
            }
        }

        printf("%*s%c\n", bird.x, "", BIRD);
        printf("Score: %d\n", score);
        fflush(stdout);
        usleep(TICK_DELAY);
        tick++;
    }

    clearScreen();
    printf("Game Over\n");
    printf("Score: %d\n", score);

    return 0;
}
