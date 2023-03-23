#ifndef PADDLE_H_SENTRY
#define PADDLE_H_SENTRY

#include <sys/time.h>
#include <ncurses.h>

struct paddle_t {
    int x, y, vx, is_player, is_bottom, score;
    struct timeval moved_at;
};

void paddle_initialize(WINDOW *win, struct paddle_t *paddle, int is_bottom);
void paddle_show(WINDOW *win, const struct paddle_t *paddle);

void tpaddle_move(WINDOW *win, struct paddle_t *tpaddle,
    const struct ball_t *ball);
void bpaddle_move(WINDOW *win, struct paddle_t *bpaddle,
    const struct ball_t *ball);

#endif
