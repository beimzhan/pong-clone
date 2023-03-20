#include <sys/select.h>
#include <ncurses.h>
#include "board.h"
#include "ball.h"
#include "clock.h"
#include "pong.h"

enum { delay_duration = 50 };

static void pong_update(struct board_t *board)
{
    int ch, since_start;
    struct timeval delay;
    fd_set readfds;

    since_start = milliseconds_elapsed(&board->loop_start);
    if(since_start < delay_duration) {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        delay.tv_sec = 0;
        delay.tv_usec = (delay_duration - since_start) * 1000;
        select(1, &readfds, NULL, NULL, &delay);
    } else {
        gettimeofday(&board->loop_start, NULL);
        flushinp();
    }

    ch = wgetch(board->window);
    switch(ch) {
        case KEY_LEFT:
            board->bpaddle.vx = -1;
            break;
        case KEY_RIGHT:
            board->bpaddle.vx = 1;
            break;
    }

    if(milliseconds_elapsed(&board->loop_start) >= delay_duration) {
        enum ball_move_result result;

        paddle_move(board->window, &board->bpaddle);
        board_net_show(board->window);
        ball_move(board, &result);
        switch(result) {
            case ball_top_scored:
                ball_initialize(board, 0);
                break;
            case ball_bottom_scored:
                ball_initialize(board, 1);
                break;
            case ball_in_play:
                { }
        }

        wrefresh(board->window);
    }
}

void pong_play()
{
    struct board_t board;
    enum pong_state_t {
        pong_enter_s, pong_update_s, pong_leave_s
    } pong_state = pong_enter_s;

    do {
        switch(pong_state) {
            case pong_enter_s:
                initscr();
                cbreak();
                curs_set(0);
                noecho();
                board_initialize(&board);
                keypad(board.window, TRUE);
                wtimeout(board.window, 0);
                pong_state = pong_update_s;
                break;
            case pong_leave_s:
                board_erase(&board);
                endwin();
                break;
            case pong_update_s:
                pong_update(&board);
                break;
        }
    } while(pong_state != pong_leave_s);
}
