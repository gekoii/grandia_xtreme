#include "common.h"

void AmapAreaAdd(void *, s32, s32, s32, s32, s32, s32, s32, s32);

void AmapAreaAddBox(void *p, s32 flags, s32 arg2, s16 *q) {
    s32 a = q[4];
    s32 b = q[5];
    s32 c = q[6];
    s32 d = q[7];
    s32 e = q[8];
    s32 g = q[9];

    if (q[0]) {
        flags |= 0x80;
    }
    AmapAreaAdd(p, flags, a, b, c, d, e, g, arg2);
}
