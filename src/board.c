#include "board.h"

static int will_ball_bounce_off_lpaddle(const struct board_t *board)
{
    return board->ball.x == 2 &&
        ((board->ball.y >= board->lpaddle.y &&
            board->ball.y < board->lpaddle.y + paddle_height) ||
        (board->ball.y == board->lpaddle.y - 1 && board->ball.vy == 1) ||
        (board->ball.y == board->lpaddle.y + paddle_height &&
            board->ball.vy == -1));
}

static int will_ball_bounce_off_rpaddle(const struct board_t *board)
{
    return board->ball.x == board_width - 1 &&
        ((board->ball.y >= board->rpaddle.y &&
            board->ball.y < board->rpaddle.y + paddle_height) ||
        (board->ball.y == board->rpaddle.y - 1 && board->ball.vy == 1) ||
        (board->ball.y == board->rpaddle.y + paddle_height &&
            board->ball.vy == -1));
}

enum ball_move_result ball_move(struct board_t *board)
{
    ball_hide(board->window, &board->ball);

    board->ball.y += board->ball.vy;
    if(board->ball.y == 1)
        board->ball.vy = 1;
    else if(board->ball.y == board_height)
        board->ball.vy = -1;

    board->ball.x += board->ball.vx;
    if(will_ball_bounce_off_lpaddle(board)) {
        if(!(board->ball.y == 1 || board->ball.y == board_height))
            board->ball.vy = -board->ball.vy;
        board->ball.vx = 1;
    } else if(board->ball.x < 1) {
        return ball_right_scored;
    } else if(will_ball_bounce_off_rpaddle(board)) {
        if(!(board->ball.y == 1 || board->ball.y == board_height))
            board->ball.vy = -board->ball.vy;
        board->ball.vx = -1;
    } else if(board->ball.x > board_width) {
        return ball_left_scored;
    }

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

    ball_initialize(&board->ball);
    ball_show(board->window, &board->ball);

    paddle_initialize(&board->lpaddle, 1);
    paddle_initialize(&board->rpaddle, 0);

    paddle_show(board->window, &board->lpaddle);
    paddle_show(board->window, &board->rpaddle);

    wrefresh(board->window);
}

void board_erase(struct board_t *board)
{
    wborder(board->window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(board->window);
    delwin(board->window);
}
