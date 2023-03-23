#include "ball.h"

void ball_hide(WINDOW *win, const struct ball_t *ball)
{
    mvwaddch(win, ball->y, ball->x, ' ');
}

void ball_show(WINDOW *win, const struct ball_t *ball)
{
    mvwaddch(win, ball->y, ball->x, '@');
}
