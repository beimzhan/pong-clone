#include "board.h"
#include "clock.h"
#include "paddle.h"

enum { paddle_delay = 50 };

void paddle_initialize(struct paddle_t *paddle, int is_bottom)
{
    paddle->x = (board_width - paddle_width) / 2 + 1;
    paddle->vx = 0;
    paddle->is_bottom = paddle->is_player = is_bottom;
    gettimeofday(&paddle->moved_at, NULL);
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
    if(milliseconds_elapsed(&paddle->moved_at) < paddle_delay)
        return;
    
    gettimeofday(&paddle->moved_at, NULL);

    paddle_hide(win, paddle);
    paddle->x += paddle->vx;
    if(paddle->x < 1)
        paddle->x = 1;
    else if(paddle->x + paddle_width - 1 > board_width)
        paddle->x = board_width - paddle_width + 1;

    paddle_show(win, paddle);

    paddle->vx = 0;
}
