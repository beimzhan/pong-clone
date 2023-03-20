#ifndef BOARD_H_SENTRY
#define BOARD_H_SENTRY

#include <ncurses.h>
#include "ball.h"
#include "paddle.h"

enum { board_width = 60, board_height = 22 };

enum ball_move_result {
    ball_in_play, ball_top_scored, ball_bottom_scored
};

struct board_t {
    WINDOW *window;
    int x, y;
    struct ball_t ball;
    struct paddle_t tpaddle, bpaddle;
};

enum ball_move_result ball_move(struct board_t *board);

void board_initialize(struct board_t *board);
void board_erase(struct board_t *board);

#endif
