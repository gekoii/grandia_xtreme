#include "common.h"
#include "gph.h"
#include "main/n04.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void VuMtxApplyTRS(s32, s32, void *);
void VuMtxApplyTRSQuat(s32, s32, void *);
void GphMotKeyLerpVec(s32, f32, void *);
void GphMotKeyLerpRot(s32, f32, void *);
void GphMotKeyLerpQuat(s32, f32, void *);

#define SPR ((u8 *)0x70003080)

void GphMotPartCalcMatrix(s32 arg0, GphPart *arg1, GphMotCursor *arg2) {
    f32 f = arg2->frame;
    s32 *q = arg2->track;
    GphMotTrackHdr *p;
    u16 flags;
    s32 done;

    copyq(SPR, arg1->pos);
    copyq(SPR + 0x10, arg1->rot);
    copyq(SPR + 0x20, arg1->scale);
    p = (GphMotTrackHdr *)q;
    q++;
    flags = p->flags;
    if (flags == 0) {
        VuMtxApplyTRS(7, arg0, SPR);
        return;
    }
    done = 0;
    if (flags & 1) {
        GphMotKeyLerpVec(*q, f, SPR);
        q++;
    }
    if (flags & 2) {
        GphMotKeyLerpRot(*q, f, SPR + 0x10);
        q++;
    }
    if (flags & 4) {
        GphMotKeyLerpVec(*q, f, SPR + 0x20);
        q++;
    }
    if (flags & 8) {
        GphMotKeyLerpQuat(*q, f, SPR + 0x10);
        done = 1;
    }
    if (done) {
        VuMtxApplyTRSQuat(7, arg0, SPR);
        return;
    }
    VuMtxApplyTRS(7, arg0, SPR);
}
