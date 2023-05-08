#include "pong.h"

#include <ncurses.h>
#include <stdlib.h>

#include "ball.h"
#include "board.h"
#include "clock.h"
#include "constants.h"

const char bpaddle_win_msg[] = "Congratulations on your victory!";
const char tpaddle_win_msg[] = "Bot is the winner :(";

const char restart_message[] =
    "Press any key to start a new game. %d second%s left. ";

enum pong_state_t {
  pong_enter_s,
  pong_update_s,
  pong_game_over_s,
  pong_game_terminated_s,
  pong_leave_s
};

enum ball_move_result_t {
  ball_didnt_move,
  ball_in_play,
  ball_tpaddle_scored,
  ball_bpaddle_scored
};

static int will_ball_bounce_off_paddle(const struct ball_t *ball,
                                       const struct paddle_t *paddle)
{
  return ball->y == (paddle->is_bottom ? paddle->y - 1 : paddle->y + 1) &&
         ball->x >= paddle->x - 1 && ball->x <= paddle->x + paddle_width &&
         ball->vy == (paddle->is_bottom ? 1 : -1);
}

static void ball_bounce(struct ball_t *ball)
{
  ball->vy *= -1;
}

static void ball_move(struct board_t *board, enum ball_move_result_t *result)
{
  if (milliseconds_elapsed(&board->ball.spawned_at) < ball_spawn_delay ||
      milliseconds_elapsed(&board->ball.moved_at) < ball_delay) {
    *result = ball_didnt_move;
    return;
  }

  *result = ball_in_play;

  gettimeofday(&board->ball.moved_at, NULL);

  ball_hide(board->window, &board->ball);

  if (will_ball_bounce_off_paddle(&board->ball, &board->tpaddle) ||
      will_ball_bounce_off_paddle(&board->ball, &board->bpaddle)) {
    ball_bounce(&board->ball);
  }

  board->ball.x += board->ball.vx;
  if (board->ball.x == 1)
    board->ball.vx = 1;
  else if (board->ball.x == board_width)
    board->ball.vx = -1;

  board->ball.y += board->ball.vy;
  if (will_ball_bounce_off_paddle(&board->ball, &board->tpaddle)) {
    ball_bounce(&board->ball);
  }
  else if (board->ball.y < 1) {
    *result = ball_bpaddle_scored;
    return;
  }
  else if (will_ball_bounce_off_paddle(&board->ball, &board->bpaddle)) {
    ball_bounce(&board->ball);
  }
  else if (board->ball.y > board_height) {
    *result = ball_tpaddle_scored;
    return;
  }

  board_net_show(board->window);

  ball_show(board->window, &board->ball);
}

static void pong_update(struct board_t *board, enum pong_state_t *state)
{
  int ch, since_start;

  since_start = milliseconds_elapsed(&board->loop_start);
  if (since_start < tick_duration) {
    wtimeout(board->window, tick_duration - since_start);
  }
  else {
    gettimeofday(&board->loop_start, NULL);
    flushinp();
  }

  ch = wgetch(board->window);
  switch (ch) {
    case KEY_RESIZE:
      if (!check_screen_size()) {
        *state = pong_game_terminated_s;
        return;
      }
      board_reinitialize(board);
      break;
    case KEY_LEFT:
      board->bpaddle.vx = -1;
      break;
    case KEY_RIGHT:
      board->bpaddle.vx = 1;
      break;
  }

  if (milliseconds_elapsed(&board->loop_start) >= tick_duration) {
    enum ball_move_result_t result;

    bpaddle_move(board->window, &board->bpaddle, &board->ball);
    tpaddle_move(board->window, &board->tpaddle, &board->ball);
    ball_move(board, &result);
    switch (result) {
      case ball_tpaddle_scored:
        board->tpaddle.score++;
        ball_initialize(board->window, &board->ball, 0);
        break;
      case ball_bpaddle_scored:
        board->bpaddle.score++;
        ball_initialize(board->window, &board->ball, 1);
        break;
      case ball_didnt_move:
      case ball_in_play:
        break;
    }
    board_scores_show(board);

    wrefresh(board->window);
  }
}

void pong_game_over(const struct board_t *board, int *restart)
{
  int x, y, time_left;

  *restart = 0;

  y = board_height / 2 - 1;
  if (board->bpaddle.score == endgame_score) {
    x = (board_width - sizeof(bpaddle_win_msg) + 1) / 2;
    mvwprintw(board->window, y, x, bpaddle_win_msg);
  }
  else {
    x = (board_width - sizeof(tpaddle_win_msg) + 1) / 2;
    mvwprintw(board->window, y, x, tpaddle_win_msg);
  }

  y = board_height / 2 + 1;
  for (time_left = restart_delay; time_left > 0; time_left--) {
    int ch;

    x = (board_width - sizeof(restart_message) + 6) / 2;
    mvwprintw(board->window, y, x, restart_message, time_left,
              time_left == 1 ? "" : "s");
    wrefresh(board->window);

    flushinp();
    napms(restart_tick_duration);
    ch = wgetch(board->window);
    if (ch != ERR) {
      *restart = 1;
      break;
    }
  }
}

void pong_play()
{
  struct board_t board;
  enum pong_state_t pong_state = pong_enter_s;
  int restart;

  initscr();
  cbreak();
  curs_set(0);
  noecho();

  do {
    switch (pong_state) {
      case pong_enter_s:
        if (!check_screen_size()) {
          pong_state = pong_leave_s;
          break;
        }

        board_initialize(&board);
        pong_state = pong_update_s;
        break;
      case pong_update_s:
        pong_update(&board, &pong_state);
        if (board.tpaddle.score == endgame_score ||
            board.bpaddle.score == endgame_score) {
          pong_state = pong_game_over_s;
        }
        break;
      case pong_game_over_s:
        pong_game_over(&board, &restart);
        board_windows_erase(&board);
        if (restart)
          pong_state = pong_enter_s;
        else
          pong_state = pong_leave_s;
        break;
      case pong_game_terminated_s:
        board_windows_erase(&board);
        pong_state = pong_leave_s;
        break;
      case pong_leave_s:
        break;
    }
  } while (pong_state != pong_leave_s);

  endwin();
}
