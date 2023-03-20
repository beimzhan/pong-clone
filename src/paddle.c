#include "board.h"
#include "paddle.h"

void paddle_initialize(struct paddle_t *paddle, int is_bottom)
{
    paddle->x = (board_width - paddle_width) / 2 + 1;
    paddle->vx = 0;
    paddle->is_bottom = paddle->is_player = is_bottom;
}

void paddle_show(WINDOW *win, const struct paddle_t *paddle)
{
    mvwhline(win,
        paddle->is_bottom ? board_height : 1, paddle->x,
        '#', paddle_width
    );
}

static void paddle_hide(WINDOW *win, const struct paddle_t *paddle)
{
    mvwhline(win,
        paddle->is_bottom ? board_height : 1, paddle->x,
        ' ', paddle_width
    );
}

void paddle_move(WINDOW *win, struct paddle_t *paddle)
{
    paddle_hide(win, paddle);
    paddle->x += paddle->vx;
    if(paddle->x < 1) {
        paddle->x = 1;
        paddle->vx = 0;
    } else if(paddle->x + paddle_width - 1 > board_width) {
        paddle->x = board_width - paddle_width + 1;
        paddle->vx = 0;
    }
    paddle_show(win, paddle);
}
