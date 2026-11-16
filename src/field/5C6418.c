#include "common.h"
#include "field/n14.h"

static inline s32 wrap(s32 x) {
    if (x >= 59) {
        return x - 59;
    }
    return x;
}

FVECTOR *ChrTrailGet(ChrTrail *arg0, s32 arg1) {
    s32 n;

    n = arg0->head;
    n = wrap(n + arg1);
    return &arg0->pos[n];
}
