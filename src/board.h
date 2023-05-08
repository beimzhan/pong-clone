#ifndef BOARD_H_SENTRY
#define BOARD_H_SENTRY

#include <ncurses.h>
#include <sys/time.h>
#include "ball.h"
#include "paddle.h"

struct board_t {
  WINDOW *window, *scores;
  struct ball_t ball;
  struct paddle_t tpaddle, bpaddle;
  struct timeval loop_start;
};

int check_screen_size();

void board_net_show(WINDOW *win);
void board_scores_show(const struct board_t *board);

void board_windows_erase(struct board_t *board);
void board_initialize(struct board_t *board);
void board_reinitialize(struct board_t *board);

#endif
