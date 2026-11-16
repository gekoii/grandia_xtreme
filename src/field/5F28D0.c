#include "common.h"
#include "party.h"

void PtyCharSetTactics(s16 id, u8 arg1, u8 arg2) {
    ptyCharData[id].tactics[0] = arg1;
    ptyCharData[id].tactics[1] = arg2;
}
