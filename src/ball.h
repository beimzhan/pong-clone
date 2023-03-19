#ifndef BALL_H_SENTRY
#define BALL_H_SENTRY

#include <ncurses.h>

enum { ball_icon = '*' };

struct ball_t {
    int x, y, vx, vy;
};

void ball_initialize(struct ball_t *ball);
void ball_hide(WINDOW *win, const struct ball_t *ball);
void ball_show(WINDOW *win, const struct ball_t *ball);

#endif
