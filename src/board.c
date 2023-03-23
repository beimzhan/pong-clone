#include <stdlib.h>
#include "clock.h"
#include "constants.h"
#include "board.h"

int check_screen_size()
{
    return COLS >= board_width + 2 && LINES >= board_height + 2;
}

void board_net_show(WINDOW *win)
{
    mvwhline(win, board_height / 2, 1, '.', board_width);
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

    ball_initialize(board->window, &board->ball, rand() % 2);

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
