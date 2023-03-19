#ifndef PADDLE_H_SENTRY
#define PADDLE_H_SENTRY

#include <ncurses.h>

enum { paddle_height = 3 };

struct paddle_t {
    int y, vy, is_player, is_left;
};

void paddle_initialize(struct paddle_t *paddle, int is_left);
void paddle_show(WINDOW *win, const struct paddle_t *paddle);
void paddle_move(WINDOW *win, struct paddle_t *paddle);

#endif
