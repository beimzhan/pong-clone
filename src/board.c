#include <stdlib.h>
#include "board.h"

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

enum ball_move_result ball_move(struct board_t *board)
{
    ball_hide(board->window, &board->ball);

    board->ball.x += board->ball.vx;
    if(board->ball.x == 1)
        board->ball.vx = 1;
    else if(board->ball.x == board_width)
        board->ball.vx = -1;

    board->ball.y += board->ball.vy;
    if(will_ball_bounce_off_tpaddle(board))
        ball_bounce(&board->ball, &board->tpaddle);
    else if(board->ball.y < 1)
        return ball_bottom_scored;
    else if(will_ball_bounce_off_bpaddle(board))
        ball_bounce(&board->ball, &board->bpaddle);
    else if(board->ball.y > board_height)
        return ball_top_scored;

    ball_show(board->window, &board->ball);

    return ball_in_play;
}

void board_initialize(struct board_t *board)
{
    board->x = (COLS - (board_width + 2)) / 2;
    board->y = (LINES - (board_height + 2)) / 2;
    board->window =
        newwin(board_height + 2, board_width + 2, board->y, board->x);
    box(board->window, 0, 0);

    ball_initialize(&board->ball, rand() % 2);
    ball_show(board->window, &board->ball);

    paddle_initialize(&board->tpaddle, 0);
    paddle_initialize(&board->bpaddle, 1);

    paddle_show(board->window, &board->tpaddle);
    paddle_show(board->window, &board->bpaddle);

    mvwhline(board->window, board_height / 2, 1, '.', board_width);

    wrefresh(board->window);
}

void board_erase(struct board_t *board)
{
    wborder(board->window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(board->window);
    delwin(board->window);
}
