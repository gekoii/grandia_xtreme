#include "common.h"
#include "protos/battle.h"

#include "gph.h"
#include "battle/misc.h"

void FxTrailBuildStrip(GphQuad3D *, FxTrail *, s32, s16, u16);

#define UNIT_VECTOR(v) __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(v) : "memory")

void FxTrailDraw(void) {
    FxTrail *e = fxTrails;
    s32 i;
    s32 j;
    GphQuad3D *buf;
    s32 a;
    s32 prev;
    s32 k;
    GphQuad3D *q;
    s32 idx;
    s32 flip;

    for (i = 0; i < 32; i++, e++) {
        if (e->handle == NULL) {
            continue;
        }
        buf = MemAlloc16A((e->unk20 + 1) * 0x70);
        if (buf == NULL) {
            continue;
        }
        buf[e->unk20].type = 0;
        q = buf;
        for (j = e->unk20; j > 0; j--) {
            q->type = 0x12;
            UNIT_VECTOR(&q->pos[0]);
            UNIT_VECTOR(&q->pos[1]);
            UNIT_VECTOR(&q->pos[2]);
            UNIT_VECTOR(&q->pos[3]);
            q++;
        }
        flip = e->unk1E;
        prev = BtlAtan2Angle(e->handle[e->unk18].b[0], e->handle[e->unk18].b[2]);
        FxTrailBuildStrip(buf, e, prev, 0, flip);
        for (k = 1; k < e->unk1C; k++) {
            idx = e->unk18 - k;
            if (idx < 0) {
                idx += e->unk1A;
            }
            a = BtlAtan2Angle(e->handle[idx].b[0], e->handle[idx].b[2]);
            if (((a - prev + 0x4000) & 0xFFFF) > 0x7FFF) {
                flip = (flip + 0x8000) & 0xFFFF;
            }
            prev = a;
            FxTrailBuildStrip(buf, e, prev, k, flip);
            GphBatchDrawQuads3D(buf);
        }
        MemFree(buf);
    }
}
