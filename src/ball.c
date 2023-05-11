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

  ball_predict(ball);

  ball_show(win, ball);

  gettimeofday(&ball->spawned_at, NULL);
  ball->moved_at = ball->spawned_at;
}

void ball_predict(struct ball_t *ball)
{
  int vx, vy, fy;

  vx = ball->vx;
  vy = ball->vy;

  ball->fx = ball->x;
  fy = ball->y;

  while (fy != 1) {
    fy += vy;
    if (fy == board_height - 1)
      vy = -1;

    ball->fx += vx;
    if (ball->fx == 1)
      vx = 1;
    else if (ball->fx == board_width)
      vx = -1;
  }
}
