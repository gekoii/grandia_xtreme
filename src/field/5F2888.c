#include "common.h"
#include "party.h"

void PtyCharGetTactics(s16 id, s16 *arg1, s16 *arg2) {
    *arg1 = ptyCharData[id].tactics[0];
    *arg2 = ptyCharData[id].tactics[1];
}
