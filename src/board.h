#ifndef BOARD_H_SENTRY
#define BOARD_H_SENTRY

#include <sys/time.h>
#include <ncurses.h>
#include "ball.h"
#include "paddle.h"

enum { board_width = 60, board_height = 22 };

enum ball_move_result {
    ball_didnt_move, ball_in_play, ball_tpaddle_scored, ball_bpaddle_scored
};

struct board_t {
    WINDOW *window;
    struct ball_t ball;
    struct paddle_t tpaddle, bpaddle;
    struct timeval loop_start;
};

void board_net_show(WINDOW *win);

void ball_initialize(struct board_t *board, int at_bottom);
void ball_move(struct board_t *board, enum ball_move_result *result);

void board_window_erase(WINDOW *win);
void board_initialize(struct board_t *board);
void board_reinitialize(struct board_t *board);

#endif
