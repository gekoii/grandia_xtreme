#include "common.h"
#include "field/n15.h"

s32 CamTrailCount(void) {
    CamTrail *r = camTrail;
    s32 head = r->head;
    s32 tail = r->tail;

    if (head < tail) {
        return tail - head - 1;
    }
    if (tail < head) {
        return 32 - head + tail - 1;
    }
    return 0;
}
