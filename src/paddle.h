#ifndef PADDLE_H_SENTRY
#define PADDLE_H_SENTRY

#include <sys/time.h>
#include <ncurses.h>

enum { paddle_width = 10 };

struct paddle_t {
    int x, vx, is_player, is_bottom;
    struct timeval moved_at;
};

void paddle_initialize(WINDOW *win, struct paddle_t *paddle, int is_bottom);
void paddle_show(WINDOW *win, const struct paddle_t *paddle);
void paddle_move(WINDOW *win, struct paddle_t *paddle);

#endif
