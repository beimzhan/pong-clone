#ifndef BALL_H_SENTRY
#define BALL_H_SENTRY

#include <sys/time.h>
#include <ncurses.h>

struct ball_t {
    int x, y, vx, vy;
    struct timeval spawned_at, moved_at;
};

void ball_hide(WINDOW *win, const struct ball_t *ball);
void ball_show(WINDOW *win, const struct ball_t *ball);

void ball_initialize(WINDOW *win, struct ball_t *ball, int at_bottom);

#endif
