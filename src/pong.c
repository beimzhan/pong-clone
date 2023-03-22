#include <stdlib.h>
#include <ncurses.h>
#include "board.h"
#include "ball.h"
#include "clock.h"
#include "pong.h"

enum { delay_duration = 10 };
enum { tpaddle_delay = 60 };
enum { new_game_delay = 5 };
enum { win_score = 5 };

#define BPADDLE_WIN_MSG "Congratulations on your victory!"
#define TPADDLE_WIN_MSG "I'm sorry to inform you that the bot has won."
#define SUGGEST_NEW_GAME "Press any key to start a new game. %ds"

enum pong_state_t {
    pong_enter_s, pong_update_s, pong_game_over_s,
    pong_game_terminated_s, pong_leave_s
};

static int check_screen_size()
{
    return COLS >= board_width + 2 && LINES >= board_height + 2;
}

static void tpaddle_move(struct board_t *board)
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

static void pong_display_scores(const struct board_t *board)
{
    mvwprintw(board->window, board_height / 2 - 1, 3, "%d",
        board->tpaddle.score);
    mvwprintw(board->window, board_height / 2 + 1, 3, "%d",
        board->bpaddle.score);
}

static void pong_update(struct board_t *board, enum pong_state_t *state)
{
    int ch, since_start;

    since_start = milliseconds_elapsed(&board->loop_start);
    if(since_start < delay_duration) {
        wtimeout(board->window, delay_duration - since_start);
    } else {
        gettimeofday(&board->loop_start, NULL);
        flushinp();
    }

    ch = wgetch(board->window);
    switch(ch) {
        case KEY_RESIZE:
            if(!check_screen_size()) {
                *state = pong_game_terminated_s;
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
                board->tpaddle.score++;
                ball_initialize(board, 0);
                break;
            case ball_bottom_scored:
                board->bpaddle.score++;
                ball_initialize(board, 1);
                break;
            case ball_in_play:
                { }
        }
        pong_display_scores(board);

        wrefresh(board->window);
    }
}

void pong_game_over(const struct board_t *board, int *start_new_game)
{
    int ch, remaining_time = new_game_delay;

    if(board->bpaddle.score == win_score)
        mvwprintw(board->window,
            board_height / 2 - 1,
            (board_width - sizeof(BPADDLE_WIN_MSG) - 1) / 2,
            BPADDLE_WIN_MSG);
    else
        mvwprintw(board->window,
            board_height /2 - 1,
            (board_width - sizeof(TPADDLE_WIN_MSG) - 1) / 2,
            TPADDLE_WIN_MSG);
    wtimeout(board->window, 1000); /* 1 second */
    while(remaining_time > 0) {
        mvwprintw(board->window,
            board_height / 2 + 1,
            (board_width - sizeof(SUGGEST_NEW_GAME) - 1) / 2,
            SUGGEST_NEW_GAME, remaining_time);
        wrefresh(board->window);
        ch = wgetch(board->window);
        if(ch != ERR) {
            *start_new_game = 1;
            return;
        }

        remaining_time--;
    }
    if(ch == ERR)
        *start_new_game = 0;
}

void pong_play()
{
    struct board_t board;
    enum pong_state_t pong_state = pong_enter_s;
    int start_new_game;

    initscr();
    cbreak();
    curs_set(0);
    noecho();

    for(;;) {
        switch(pong_state) {
            case pong_enter_s:
                if(!check_screen_size()) {
                    endwin();
                    return;
                }

                board_initialize(&board);
                pong_state = pong_update_s;
                break;
            case pong_update_s:
                pong_update(&board, &pong_state);
                if(board.tpaddle.score == win_score ||
                    board.bpaddle.score == win_score)
                {
                    pong_state = pong_game_over_s;
                }
                break;
            case pong_game_over_s:
                pong_game_over(&board, &start_new_game);
                board_window_erase(board.window);
                if(start_new_game)
                    pong_state = pong_enter_s;
                else
                    pong_state = pong_leave_s;
                break;
            case pong_game_terminated_s:
                board_window_erase(board.window);
                pong_state = pong_leave_s;
                break;
            case pong_leave_s:
                endwin();
                return;
        }
    };
}
