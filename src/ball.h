#ifndef BALL_H_SENTRY
#define BALL_H_SENTRY

#include <sys/time.h>
#include <ncurses.h>

enum { ball_icon = '@' };
enum { ball_spawn_delay = 1000 };

struct ball_t {
    int x, y, vx, vy;
    struct timeval spawned_at, moved_at;
};

void ball_hide(WINDOW *win, const struct ball_t *ball);
void ball_show(WINDOW *win, const struct ball_t *ball);

#endif
