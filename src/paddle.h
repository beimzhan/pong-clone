#ifndef PADDLE_H_SENTRY
#define PADDLE_H_SENTRY

#include <sys/time.h>
#include <ncurses.h>

enum { paddle_height = 3 };
enum { paddle_max_abs_velocity = 2 };
enum { paddle_braking_time = 30000 };

struct paddle_t {
    int y, vy, is_player, is_left;
    struct timeval moved_at;
};

void paddle_initialize(struct paddle_t *paddle, int is_left);
void paddle_show(WINDOW *win, const struct paddle_t *paddle);
void paddle_move(WINDOW *win, struct paddle_t *paddle);

#endif
