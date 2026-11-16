#include "common.h"
#include "gph.h"
#include "main/n04.h"

typedef int s128 __attribute__((mode(TI)));

void GphMotKeyLerpVec(s32, s128 *, f32);

void GphMotPartGetPos(s128 *arg0, s128 *arg1, GphMotCursor *arg2) {
    f32 f = arg2->frame;
    s32 *q = arg2->track;
    GphMotTrackHdr *p = (GphMotTrackHdr *)q;
    q++;
    if (p->flags & 1) {
        GphMotKeyLerpVec(*q, arg0, f);
        return;
    }
    {
        s128 t;
        __asm__("lq %0, 0(%1)" : "=&r"(t) : "r"(arg1 + 1));
        *arg0 = t;
    }
}
