#include "common.h"

void SndBusyWait(void) {
    s32 i;

    i = 0x120000;
    do {
        i--;
    } while (i != 0);
}
