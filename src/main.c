#include <time.h>
#include <stdlib.h>
#include "pong.h"

int main()
{
    srand(time(NULL));
    pong_play();
    return 0;
}
