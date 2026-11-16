/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n14.h"
#include "protos/field.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void ChrTrailFindNearest(FVECTOR *arg0, ChrTrail *arg1, FVECTOR *arg2) {
    f32 best;
    f32 d;
    s32 found;
    s32 i;
    FVECTOR *p;

    found = -1;
    best = 999999.0f;
    for (i = 0, p = arg1->pos; i < 60; i++, p++) {
        d = FldVecDist(arg2, p);
        if (d < best) {
            best = d;
            found = i;
        }
    }
    if (found != -1) {
        copyq(arg0, (u8 *)arg1 + (found << 4));
    }
}
