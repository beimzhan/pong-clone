#include "board.h"
#include "clock.h"
#include "paddle.h"

void paddle_initialize(struct paddle_t *paddle, int is_left)
{
    paddle->y = (board_height - paddle_height) / 2 + 1;
    paddle->vy = 0;
    gettimeofday(&paddle->moved_at, NULL);
    paddle->is_left = paddle->is_player = is_left;
}

void paddle_show(WINDOW *win, const struct paddle_t *paddle)
{
    mvwvline(win,
        paddle->y, paddle->is_left ? 1 : board_width,
        0, paddle_height
    );
}

static void paddle_hide(WINDOW *win, const struct paddle_t *paddle)
{
    mvwvline(win,
        paddle->y, paddle->is_left ? 1 : board_width,
        ' ', paddle_height
    );
}

void paddle_move(WINDOW *win, struct paddle_t *paddle)
{
    if(milliseconds_elapsed(&paddle->moved_at) > paddle_braking_time)
        paddle->vy = 0;
    else if(milliseconds_elapsed(&paddle->moved_at) > paddle_braking_time / 2)
        paddle->vy /= 2;
    paddle_hide(win, paddle);
    paddle->y += paddle->vy;
    if(paddle->y < 1) {
        paddle->y = 1;
        paddle->vy = -paddle->vy;
    } else if(paddle->y + paddle_height - 1 > board_height) {
        paddle->y = board_height - paddle_height;
        paddle->vy = -paddle->vy;
    }
    paddle_show(win, paddle);
}
