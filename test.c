#include <stdio.h>
#include <stdbool.h>

struct Ball {
    float x, y;
    float vx, vy;
};

void update_ball(struct Ball *ball) {
    ball->x += ball->vx;
    ball->y += ball->vy;
}

void check_collision(struct Ball *ball) {
    if (ball->x <= 0.0f || ball->x >= 80.0f) {
        ball->vx = -ball->vx;
    }
    if (ball->y <= 0.0f || ball->y >= 24.0f) {
        ball->vy = -ball->vy;
    }
}

int main() {
    struct Ball ball = {40.0f, 12.0f, 0.1f, 0.1f};

    while (true) {
        update_ball(&ball);
        check_collision(&ball);
        printf("\033[%d;%dH*", (int)ball.y, (int)ball.x);
        fflush(stdout);
        usleep(10000);
    }
    return 0;
}