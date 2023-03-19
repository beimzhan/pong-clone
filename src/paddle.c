#include "board.h"
#include "paddle.h"

void paddle_initialize(struct paddle_t *paddle, int is_left)
{
    paddle->y = (board_height - paddle_height) / 2 + 1;
    paddle->vy = 0;
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
    paddle_hide(win, paddle);
    paddle->y += paddle->vy;
    if(paddle->y < 1) {
        paddle->y = 1;
        paddle->vy = 0;
    } else if(paddle->y + paddle_height - 1 > board_height) {
        paddle->y = board_height - paddle_height + 1;
        paddle->vy = 0;
    }
    paddle_show(win, paddle);
}
