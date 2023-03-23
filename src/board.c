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

    gettimeofday(&board->ball.spawned_at, NULL);
    board->ball.moved_at = board->ball.spawned_at;
}

static int will_ball_bounce_off_paddle(const struct ball_t *ball,
    const struct paddle_t *paddle)
{
    return ball->y == (paddle->is_bottom ? paddle->y - 1 : paddle->y + 1) &&
        ((ball->x >= paddle->x && ball->x < paddle->x + paddle_width) ||
            (ball->x == paddle->x - 1 && ball->vx == 1) ||
            (ball->x == paddle->x + paddle_width && ball->vx == -1));
}

static void ball_bounce(struct ball_t *ball, const struct paddle_t *paddle)
{
    int dx = paddle->x - paddle_width / 2 - ball->x;

    ball->vy *= -1;
    if(ball->x != 1 && ball->x != board_width && paddle->is_bottom && dx >= 0)
        ball->vx *= -1;
}

void ball_move(struct board_t *board, enum ball_move_result *result)
{
    if(milliseconds_elapsed(&board->ball.spawned_at) < ball_spawn_delay ||
        milliseconds_elapsed(&board->ball.moved_at) < ball_delay)
    {
        *result = ball_didnt_move;
        return;
    }

    *result = ball_in_play;

    gettimeofday(&board->ball.moved_at, NULL);

    ball_hide(board->window, &board->ball);

    board->ball.x += board->ball.vx;
    if(board->ball.x == 1)
        board->ball.vx = 1;
    else if(board->ball.x == board_width)
        board->ball.vx = -1;

    board->ball.y += board->ball.vy;
    if(will_ball_bounce_off_paddle(&board->ball, &board->tpaddle)) {
        ball_bounce(&board->ball, &board->tpaddle);
    } else if(board->ball.y < 1) {
        *result = ball_bottom_scored;
        return;
    } else if(will_ball_bounce_off_paddle(&board->ball, &board->bpaddle)) {
        ball_bounce(&board->ball, &board->bpaddle);
    } else if(board->ball.y > board_height) {
        *result = ball_top_scored;
        return;
    }

    ball_show(board->window, &board->ball);
}

static void board_window_create(WINDOW **win)
{
    int x, y;
    x = (COLS - (board_width + 2)) / 2;
    y = (LINES - (board_height + 2)) / 2;
    *win = newwin(board_height + 2, board_width + 2, y, x);
    box(*win, 0, 0);
    keypad(*win, TRUE);
    wtimeout(*win, 0);
}

void board_window_erase(WINDOW *win)
{
    wclear(win);
    wrefresh(win);
    delwin(win);
}

void board_initialize(struct board_t *board)
{
    board_window_create(&board->window);

    ball_initialize(board, rand() % 2);

    paddle_initialize(board->window, &board->tpaddle, 0);
    paddle_initialize(board->window, &board->bpaddle, 1);

    board_net_show(board->window);

    wrefresh(board->window);

    gettimeofday(&board->loop_start, NULL);
}

void board_reinitialize(struct board_t *board)
{
    board_window_erase(board->window);
    board_window_create(&board->window);
    paddle_show(board->window, &board->tpaddle);
    paddle_show(board->window, &board->bpaddle);
    board_net_show(board->window);
    ball_show(board->window, &board->ball);
    wrefresh(board->window);
}
