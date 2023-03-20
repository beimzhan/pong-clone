#ifndef CLOCK_H_SENTRY
#define CLOCK_H_SENTRY

int milliseconds_between(const struct timeval *end,
    const struct timeval *start);
int milliseconds_elapsed(const struct timeval *start);

#endif
