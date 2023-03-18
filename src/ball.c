#include "ball.h"

void ball_show(WINDOW *win, const struct ball_t *ball)
{
    mvwaddch(win, ball->y, ball->x, ball_icon);
}

void ball_move(WINDOW *win, struct ball_t *ball)
{
    ball->x += ball->vx;
    ball->y += ball->vy;
    ball_show(win, ball);
}
