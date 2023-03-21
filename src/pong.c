#include <stdlib.h>
#include <sys/select.h>
#include <ncurses.h>
#include "board.h"
#include "ball.h"
#include "clock.h"
#include "pong.h"

enum { delay_duration = 10 };
enum { tpaddle_delay = 80 };

enum pong_state_t {
    pong_enter_s, pong_update_s, pong_leave_s
};

static int check_screen_size()
{
    return COLS >= board_width + 2 && LINES >= board_height + 2;
}

void tpaddle_move(struct board_t *board)
{
    if(milliseconds_elapsed(&board->tpaddle.moved_at) < tpaddle_delay)
        return;

    if(board->ball.vy == -1) {
        if(board->ball.vx == -1)
        {
            if(board->ball.x < board->tpaddle.x + paddle_width / 2)
                board->tpaddle.vx = -1;
            else
                board->tpaddle.vx = 1;
        }
        else if(board->ball.vx == 1)
        {
            if(board->ball.x > board->tpaddle.x + paddle_width / 2)
                board->tpaddle.vx = 1;
            else
                board->tpaddle.vx = -1;
        }
    }

    paddle_move(board->window, &board->tpaddle);
}

static void pong_update(struct board_t *board, enum pong_state_t *state)
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
        case KEY_RESIZE:
            if(!check_screen_size()) {
                *state = pong_leave_s;
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

    if(milliseconds_elapsed(&board->loop_start) >= delay_duration) {
        enum ball_move_result result;

        paddle_move(board->window, &board->bpaddle);
        tpaddle_move(board);
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
    enum pong_state_t pong_state = pong_enter_s;

    for(;;) {
        switch(pong_state) {
            case pong_enter_s:
                initscr();
                if(!check_screen_size()) {
                    endwin();
                    return;
                }

                cbreak();
                curs_set(0);
                noecho();
                board_initialize(&board);
                pong_state = pong_update_s;
                break;
            case pong_update_s:
                pong_update(&board, &pong_state);
                break;
            case pong_leave_s:
                board_window_erase(board.window);
                endwin();
                return;
        }
    };
}
