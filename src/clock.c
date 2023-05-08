#include "clock.h"
#include <stddef.h>

int milliseconds_between(const struct timeval *end,
                         const struct timeval *start)
{
  return (end->tv_sec - start->tv_sec) * 1000 +
         (end->tv_usec - start->tv_usec) / 1000;
}

int milliseconds_elapsed(const struct timeval *start)
{
  struct timeval now;
  gettimeofday(&now, NULL);
  return milliseconds_between(&now, start);
}
