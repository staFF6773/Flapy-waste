#include "flappy_bird.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <algorithm>
#include <fcntl.h>

const int WIDTH = 50;
const int HEIGHT = 20;
const char BIRD = '>';
const char PIPE = '#';
const int PIPE_WIDTH = 5;
const int PIPE_GAP = 8;
const int GRAVITY = 1;
const int FLAP_FORCE = -2;
const int TICK_DELAY = 100000;

Bird::Bird(int startX, int startY) : x(startX), y(startY), velocity(0) {}

void Bird::flap() {
    velocity = FLAP_FORCE;
}

void Bird::update() {
    velocity += GRAVITY;
    y += velocity;
}

bool Bird::isOutsideBounds() {
    return y <= 0 || y >= HEIGHT;
}

bool Bird::isColliding(int pipeX, int gapPosition) {
    return x == pipeX && (y < gapPosition || y >= gapPosition + PIPE_GAP);
}

Pipe::Pipe(int startX, int startGapPosition) : x(startX), gapPosition(startGapPosition) {}

bool Pipe::isPastBird(int birdX) {
    return x + PIPE_WIDTH < birdX;
}

int main() {
    srand(time(0));

    Bird bird(WIDTH / 3, HEIGHT / 2);
    std::vector<Pipe> pipes;
    int tick = 0;
    int score = 0;
    bool gameOver = false;

    // Set terminal to non-blocking input mode
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);

    while (!gameOver) {
        system("clear");

        if (tick % 20 == 0) {
            int gapPosition = rand() % (HEIGHT - PIPE_GAP);
            pipes.emplace_back(WIDTH, gapPosition);
        }

        for (auto &pipe : pipes) {
            pipe.x--;

            if (bird.isColliding(pipe.x, pipe.gapPosition) || bird.isOutsideBounds()) {
                gameOver = true;
            }

            if (pipe.isPastBird(bird.x)) {
                score++;
            }
        }

        pipes.erase(std::remove_if(pipes.begin(), pipes.end(),
                                   [](const Pipe &pipe) { return pipe.x + PIPE_WIDTH < 0; }),
                    pipes.end());

        bird.update();

        for (auto &pipe : pipes) {
            std::cout << std::string(pipe.x, ' ') << PIPE << std::endl;
            for (int i = 0; i < HEIGHT; i++) {
                if (i < pipe.gapPosition || i >= pipe.gapPosition + PIPE_GAP) {
                    std::cout << std::string(pipe.x, ' ') << PIPE << std::endl;
                } else {
                    std::cout << std::string(pipe.x, ' ') << std::string(PIPE_WIDTH, ' ') << std::endl;
                }
            }
        }

        std::cout << std::string(bird.x, ' ') << BIRD << std::endl;

        std::cout << "Score: " << score << std::endl;
        usleep(TICK_DELAY);

        // Check for user input
        char input;
        std::cin >> input;
        if (input == ' ') {
            bird.flap();
        }

        tick++;
    }

    // Restore terminal settings
    t.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) & ~O_NONBLOCK);

    std::cout << "Game Over" << std::endl;
    std::cout << "Score: " << score << std::endl;

    return 0;
}
