#include <stdlib.h>
#include "clock.h"
#include "board.h"

enum { ball_delay = 100 };

void board_net_show(WINDOW *win)
{
    mvwhline(win, board_height / 2, 1, '.', board_width);
}

void ball_initialize(struct board_t *board, int at_bottom)
{
    board->ball.x = rand() % (board_width - 2) + 2;
    board->ball.y = at_bottom ? board_height - 1 : 2;
    board->ball.vx = rand() % 2 ? -1 : 1;
    board->ball.vy = at_bottom ? -1 : 1;
    ball_show(board->window, &board->ball);

    gettimeofday(&board->ball.spawn_time, NULL);
    board->ball.moved_at = board->ball.spawn_time;
}

static int will_ball_bounce_off_tpaddle(const struct board_t *board)
{
    return board->ball.y == 2 &&
        ((board->ball.x >= board->tpaddle.x &&
            board->ball.x < board->tpaddle.x + paddle_width) ||
        (board->ball.x == board->tpaddle.x - 1 && board->ball.vx == 1) ||
        (board->ball.x == board->tpaddle.x + paddle_width &&
            board->ball.vx == -1));
}

static int will_ball_bounce_off_bpaddle(const struct board_t *board)
{
    return board->ball.y == board_height - 1 &&
        ((board->ball.x >= board->bpaddle.x &&
            board->ball.x < board->bpaddle.x + paddle_width) ||
        (board->ball.x == board->bpaddle.x - 1 && board->ball.vx == 1) ||
        (board->ball.x == board->bpaddle.x + paddle_width &&
            board->ball.vx == -1));
}

static void ball_bounce(struct ball_t *ball, const struct paddle_t *paddle)
{
    ball->vy *= -1;
    if(!(ball->x == 1 || ball->x == board_width))
        ball->vx = (paddle->x + paddle_width - 1) / 2 - ball->x < 0 ? 1 : -1;
}

void ball_move(struct board_t *board, enum ball_move_result *result)
{
    *result = ball_in_play;

    if(milliseconds_elapsed(&board->ball.spawn_time) < ball_spawn_delay ||
        milliseconds_elapsed(&board->ball.moved_at) < ball_delay)
    {
        return;
    }

    gettimeofday(&board->ball.moved_at, NULL);

    ball_hide(board->window, &board->ball);

    board->ball.x += board->ball.vx;
    if(board->ball.x == 1)
        board->ball.vx = 1;
    else if(board->ball.x == board_width)
        board->ball.vx = -1;

    board->ball.y += board->ball.vy;
    if(will_ball_bounce_off_tpaddle(board)) {
        ball_bounce(&board->ball, &board->tpaddle);
    } else if(board->ball.y < 1) {
        *result = ball_bottom_scored;
        return;
    } else if(will_ball_bounce_off_bpaddle(board)) {
        ball_bounce(&board->ball, &board->bpaddle);
    } else if(board->ball.y > board_height) {
        *result = ball_top_scored;
        return;
    }

    ball_show(board->window, &board->ball);
}

void board_initialize(struct board_t *board)
{
    board->x = (COLS - (board_width + 2)) / 2;
    board->y = (LINES - (board_height + 2)) / 2;
    board->window =
        newwin(board_height + 2, board_width + 2, board->y, board->x);
    box(board->window, 0, 0);

    ball_initialize(board, rand() % 2);

    paddle_initialize(&board->tpaddle, 0);
    paddle_initialize(&board->bpaddle, 1);

    paddle_show(board->window, &board->tpaddle);
    paddle_show(board->window, &board->bpaddle);

    board_net_show(board->window);

    wrefresh(board->window);

    gettimeofday(&board->loop_start, NULL);
}

void board_erase(struct board_t *board)
{
    wborder(board->window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(board->window);
    delwin(board->window);
}
