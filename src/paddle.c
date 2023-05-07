#include <stdlib.h>
#include "ball.h"
#include "clock.h"
#include "constants.h"
#include "paddle.h"

void paddle_initialize(WINDOW *win, struct paddle_t *paddle, int is_bottom)
{
    paddle->x = (board_width - paddle_width) / 2 + 1;
    paddle->y = is_bottom ? board_height : 1;
    paddle->vx = 0;

    paddle->is_bottom = paddle->is_player = is_bottom;

    paddle->score = 0;

    paddle_show(win, paddle);

    gettimeofday(&paddle->moved_at, NULL);
}

void paddle_show(WINDOW *win, const struct paddle_t *paddle)
{
    mvwhline(win, paddle->y, paddle->x, '#', paddle_width);
}

static void paddle_hide(WINDOW *win, const struct paddle_t *paddle)
{
    mvwhline(win, paddle->y, paddle->x, ' ', paddle_width);
}

static void paddle_move(WINDOW *win, struct paddle_t *paddle,
    const struct ball_t *ball)
{
    if(paddle->vx == 0 ||
        ((ball->x == paddle->x + paddle->vx ||
            ball->x == paddle->x + paddle_width + paddle->vx) &&
        ball->y == paddle->y))
    {
        return;
    }

    paddle_hide(win, paddle);

    paddle->x += paddle->vx;
    if(paddle->x < 1)
        paddle->x = 1;
    else if(paddle->x + paddle_width - 1 > board_width)
        paddle->x = board_width - paddle_width + 1;

    paddle_show(win, paddle);

    gettimeofday(&paddle->moved_at, NULL);
}

void tpaddle_move(WINDOW *win, struct paddle_t *tpaddle,
    const struct ball_t *ball)
{
    int dx = tpaddle->x + paddle_width / 2 - ball->x;

    if(milliseconds_elapsed(&tpaddle->moved_at) < tpaddle_delay)
        return;

    if(ball->vy == -1 && abs(dx) > 2) {
        if(ball->vx == -1)
        {
            if(dx > 0)
                tpaddle->vx = -1;
            else
                tpaddle->vx = 1;
        }
        else if(ball->vx == 1)
        {
            if(dx < 0)
                tpaddle->vx = 1;
            else
                tpaddle->vx = -1;
        }
    } else if(ball->vy == 1) {
        tpaddle->vx = 0;
    }

    paddle_move(win, tpaddle, ball);
}

void bpaddle_move(WINDOW *win, struct paddle_t *bpaddle,
    const struct ball_t *ball)
{
    if(milliseconds_elapsed(&bpaddle->moved_at) < bpaddle_delay)
        return;

    paddle_move(win, bpaddle, ball);

    bpaddle->vx = 0;
}
