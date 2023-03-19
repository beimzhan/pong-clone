#include <stdlib.h>
#include "ball.h"
#include "board.h"

static int ball_random_velocity()
{
    return rand() % 2 == 0 ? 1 : -1;
}

void ball_initialize(struct ball_t *ball)
{
    ball->x = board_width / 2 + 1;
    ball->y = board_height / 2 + 1;
    ball->vx = ball_random_velocity();
    ball->vy = ball_random_velocity();
}

void ball_hide(WINDOW *win, const struct ball_t *ball)
{
    mvwaddch(win, ball->y, ball->x, ' ');
}

void ball_show(WINDOW *win, const struct ball_t *ball)
{
    mvwaddch(win, ball->y, ball->x, ball_icon);
}
