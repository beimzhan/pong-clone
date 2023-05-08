#include "ball.h"

#include <stdlib.h>

#include "constants.h"

void ball_hide(WINDOW *win, const struct ball_t *ball)
{
  mvwaddch(win, ball->y, ball->x, ' ');
}

void ball_show(WINDOW *win, const struct ball_t *ball)
{
  mvwaddch(win, ball->y, ball->x, '@');
}

void ball_initialize(WINDOW *win, struct ball_t *ball, int at_bottom)
{
  ball->x = rand() % (board_width - 2) + 2;
  ball->y = at_bottom ? board_height - 1 : 2;
  ball->vx = rand() % 2 ? -1 : 1;
  ball->vy = at_bottom ? -1 : 1;
  ball_show(win, ball);

  gettimeofday(&ball->spawned_at, NULL);
  ball->moved_at = ball->spawned_at;
}
