#ifndef BOARD_H_SENTRY
#define BOARD_H_SENTRY

#include <ncurses.h>
#include "ball.h"
#include "paddle.h"

enum { board_width = 75, board_height = 20 };

struct board_t {
    WINDOW *window;
    int x, y;
    struct ball_t ball;
    struct paddle_t lpaddle, rpaddle;
};

void board_initialize(struct board_t *board);
void board_erase(struct board_t *board);

#endif
