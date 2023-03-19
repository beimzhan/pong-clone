#ifndef BALL_H_SENTRY
#define BALL_H_SENTRY

#include <ncurses.h>

enum { ball_icon = '*' };
enum { ball_vx = 1, ball_vy = 1 };

struct ball_t {
    int x, y, vx, vy;
};

void ball_initialize(struct ball_t *ball);
void ball_hide(WINDOW *win, const struct ball_t *ball);
void ball_show(WINDOW *win, const struct ball_t *ball);

#endif
