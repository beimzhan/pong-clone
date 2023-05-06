#include <stdlib.h>
#include "clock.h"
#include "constants.h"
#include "board.h"

int check_screen_size()
{
    return COLS >= 80 && LINES >= board_height + 2;
}

void board_net_show(WINDOW *win)
{
    mvwhline(win, board_height / 2, 1, '.', board_width);
}

void board_scores_show(const struct board_t *board)
{
    mvwprintw(board->scores, 0, 0, "%d", board->tpaddle.score);
    mvwprintw(board->scores, 2, 0, "%d", board->bpaddle.score);
    wrefresh(board->scores);
}

static void board_windows_create(struct board_t *board)
{
    int x, y;
    x = (COLS - (board_width + 2)) / 2;
    y = (LINES - (board_height + 2)) / 2;
    board->window = newwin(board_height + 2, board_width + 2, y, x);
    board->scores = newwin(3, 1, LINES / 2 - 2, x - 5);
    box(board->window, 0, 0);
    keypad(board->window, TRUE);
    wtimeout(board->window, 0);
}

void board_windows_erase(struct board_t *board)
{
    wclear(board->window);
    wclear(board->scores);
    wrefresh(board->window);
    wrefresh(board->scores);
    delwin(board->window);
    delwin(board->scores);
}

void board_initialize(struct board_t *board)
{
    board_windows_create(board);

    ball_initialize(board->window, &board->ball, rand() % 2);

    paddle_initialize(board->window, &board->tpaddle, 0);
    paddle_initialize(board->window, &board->bpaddle, 1);

    board_net_show(board->window);

    wrefresh(board->window);

    gettimeofday(&board->loop_start, NULL);
}

void board_reinitialize(struct board_t *board)
{
    board_windows_erase(board);
    board_windows_create(board);
    paddle_show(board->window, &board->tpaddle);
    paddle_show(board->window, &board->bpaddle);
    board_net_show(board->window);
    ball_show(board->window, &board->ball);
    wrefresh(board->window);
}
