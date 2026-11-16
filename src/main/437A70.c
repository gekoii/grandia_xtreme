#include "common.h"
#include "protos/main.h"
#include "gph.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void GphMotPartGetPos(void *, GphPart *, GphMotCursor *);

s32 ChrModelGetPartPos(void *arg0, GphModel *arg1, s32 arg2, f32 arg3) {
    GphMotCursor c;
    s32 i;

    if (arg1 == NULL) {
        return 0;
    }
    if (arg1->motion[0].motion != NULL) {
        GphMotCursorInit(&c, arg1->motion[0].motion, arg3);
        for (i = 0; i < arg2; i++) {
            GphMotCursorNextPart(&c);
        }
        GphMotPartGetPos(arg0, &arg1->parts[arg2], &c);
    } else {
        GphPart *p = &arg1->parts[arg2];
        copyq(arg0, p->pos);
    }
    return 1;
}
