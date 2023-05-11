#ifndef BALL_H_SENTRY
#define BALL_H_SENTRY

#include <ncurses.h>
#include <sys/time.h>

struct ball_t {
  int x, y, fx, vx, vy;
  struct timeval spawned_at, moved_at;
};

void ball_hide(WINDOW *win, const struct ball_t *ball);
void ball_show(WINDOW *win, const struct ball_t *ball);

void ball_initialize(WINDOW *win, struct ball_t *ball, int at_bottom);

void ball_predict(struct ball_t *ball);

#endif
