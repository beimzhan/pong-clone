#ifndef BOARD_H_SENTRY
#define BOARD_H_SENTRY

#include <sys/time.h>
#include <ncurses.h>
#include "ball.h"
#include "paddle.h"

struct board_t {
    WINDOW *window;
    struct ball_t ball;
    struct paddle_t tpaddle, bpaddle;
    struct timeval loop_start;
};

int check_screen_size();

void board_net_show(WINDOW *win);

void board_window_erase(WINDOW *win);
void board_initialize(struct board_t *board);
void board_reinitialize(struct board_t *board);

#endif
