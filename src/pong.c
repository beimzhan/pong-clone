#include <sys/select.h>
#include <ncurses.h>
#include "board.h"
#include "clock.h"
#include "pong.h"

enum { delay_duration = 100 };

void pong_play()
{
    struct board_t board;
    fd_set readfds;
    struct timeval start, delay;

    initscr();

    cbreak();
    curs_set(0);
    noecho();

    board_initialize(&board);
    keypad(board.window, TRUE);
    wtimeout(board.window, 0);
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    delay.tv_sec = 0;
    gettimeofday(&start, NULL);
    for(;;) {
        int ch, from_start;

        from_start = milliseconds_elapsed(&start);
        if(from_start < delay_duration) {
            delay.tv_usec = (delay_duration - from_start) * 1000;
            select(1, &readfds, NULL, NULL, &delay);
        } else {
            gettimeofday(&start, NULL);
        }

        ch = wgetch(board.window);
        switch(ch) {
            case KEY_UP:
                board.lpaddle.vy = -1;
                gettimeofday(&board.lpaddle.moved_at, NULL);
                break;
            case KEY_DOWN:
                board.lpaddle.vy = 1;
                gettimeofday(&board.lpaddle.moved_at, NULL);
                break;
            case ' ':
                board.lpaddle.vy = 0;
                break;
        }

        if(milliseconds_elapsed(&start) >= delay_duration) {
            enum ball_move_result result;

            paddle_move(board.window, &board.lpaddle);

            result = ball_move(&board);
            switch(result) {
                case ball_left_scored:
                case ball_right_scored:
                    ball_initialize(&board.ball);
                case ball_in_play:
                    { }
            }

            wrefresh(board.window);
        }
    }

    board_erase(&board);

    endwin();
}
