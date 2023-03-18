#include "board.h"

void board_initialize(struct board_t *board)
{
    board->x = (COLS - (board_width + 2)) / 2;
    board->y = (LINES - (board_height + 2)) / 2;
    board->window =
        newwin(board_height + 2, board_width + 2, board->y, board->x);
    box(board->window, 0, 0);

    board->ball.x = board_width / 2 + 1;
    board->ball.y = board_height / 2 + 1;
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
