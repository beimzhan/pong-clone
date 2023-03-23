#ifndef CONSTANTS_H_SENTRY
#define CONSTANTS_H_SENTRY

enum { tick_duration = 10 };

enum { board_width = 60, board_height = 22 };

enum { ball_spawn_delay = 1000, ball_delay = 100 };

enum { paddle_width = 10, tpaddle_delay = 60, bpaddle_delay = 50 };

enum { endgame_score = 3 };

enum {
    restart_delay = 5, /* seconds */
    restart_tick_duration = 1000 /* milliseconds */
};

#endif
