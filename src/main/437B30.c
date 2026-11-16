#include "common.h"
#include "protos/main.h"
#include "gph.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


/* arg0: pos, rot, scale */
s32 ChrModelGetPartXformById(FVECTOR *arg0, GphModel *arg1, s32 arg2, f32 arg3) {
    GphMotCursor c;
    GphPart *p;
    s32 n;
    s32 i;

    if (arg1 == NULL) {
        return -1;
    }
    if (arg1->motion[0].motion != NULL) {
        GphMotCursorInit(&c, arg1->motion[0].motion, arg3);
        p = ChrModelSearchPart(arg1, arg2);
        if (p == NULL) {
            return 0;
        }
        n = (u32)((u8 *)p - (u8 *)arg1->parts) / 160;
        for (i = 0; i < n; i++) {
            GphMotCursorNextPart(&c);
        }
        return GphMotPartGetTRS(arg0, p, &c);
    }
    p = ChrModelSearchPart(arg1, arg2);
    if (p == NULL) {
        return -1;
    }
    copyq(arg0[0], p->pos);
    copyq(arg0[1], p->rot);
    copyq(arg0[2], p->scale);
    return 0;
}
