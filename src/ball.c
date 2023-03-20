#include <stdlib.h>
#include "ball.h"
#include "board.h"

void ball_initialize(struct ball_t *ball, int at_bottom)
{
    ball->x = rand() % board_width + 1;
    ball->y = at_bottom ? board_height - 1 : 2;
    ball->vx = rand() % 2 ? -1 : 1;
    ball->vy = at_bottom ? -1 : 1;
}

void ball_hide(WINDOW *win, const struct ball_t *ball)
{
    mvwaddch(win, ball->y, ball->x, ' ');
}

void ball_show(WINDOW *win, const struct ball_t *ball)
{
    mvwaddch(win, ball->y, ball->x, ball_icon);
}
