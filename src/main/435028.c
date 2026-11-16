#include "common.h"
#include "gph.h"
#include "main/n04.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void GphMotKeyLerpVec(s32, void *, f32);
void GphMotKeyLerpRot(s32, f32, void *);
void GphMotKeyLerpQuat(s32, f32, void *);

s32 GphMotPartGetTRS(u8 *arg0, GphPart *arg1, GphMotCursor *arg2) {
    s32 *q = arg2->track;
    f32 f = arg2->frame;
    u8 *p1;
    u8 *p2;
    GphMotTrackHdr *p;
    u16 flags;
    s32 ret;

    copyq(arg0, arg1->pos);
    p1 = arg0 + 0x10;
    copyq(p1, arg1->rot);
    p2 = arg0 + 0x20;
    copyq(p2, arg1->scale);
    p = (GphMotTrackHdr *)q;
    ret = 0;
    q++;
    flags = p->flags;
    if (flags != 0) {
        if (flags & 1) {
            GphMotKeyLerpVec(*q, arg0, f);
            q++;
        }
        if (flags & 2) {
            GphMotKeyLerpRot(*q, f, p1);
            q++;
        }
        if (flags & 4) {
            GphMotKeyLerpVec(*q, p2, f);
            q++;
        }
        if (flags & 8) {
            ret = 1;
            GphMotKeyLerpQuat(*q, f, p1);
        }
    }
    return ret;
}
