#include "common.h"

#include "battle/misc.h"

void FxTraceInit(void)
{
    s32 i;
    s32 j;
    s32 k;
    FxTraceUnit *p;
    s32 *c;
    FxTraceStrip *a;
    FxTraceSpark *b;
    FxTrail *e;

    a = fxTraceStrips;
    b = fxTraceSparks;
    for (i = 0; i < 16; i++, a++, b++) {
        a->buf = NULL;
        b->handle = NULL;
    }
    p = fxTraceUnits;
    for (i = 0; i < 16; i++, p++) {
        p->motion = NULL;
        p->unk14 = 1.0f;
        for (j = 0; j < 6; j++) {
            p->flags[j] = 0;
            p->kinds[j] = 0;
            p->parts[j].part1 = 0;
            p->parts[j].part0 = 0;
            __asm__ volatile(
                "sqc2 $vf0, 0(%0)\n\t"
                "sqc2 $vf0, 0(%1)"
                : : "r"(p->ofs[j].a), "r"(p->ofs[j].b) : "memory");
        }
    }
    c = (s32 *)fxTraceColors;
    *c++ = 0xE69641C0;
    *c++ = 0x5AFA00;
    *c++ = 0xFF3C10FF;
    *c++ = 0xFF001E00;
    *c++ = 0x50FFA4FF;
    *c++ = 0x20406000;
    *c++ = 0xB480FF;
    *c++ = 0xFF00;
    *c++ = 0x804030FF;
    *c++ = 0x281E2800;
    *c++ = 0xFF7E40FF;
    *c++ = 0x80200000;
    *c++ = 0x6428FFFF;
    *c++ = 0xFF00;
    *c++ = 0x5F5AB4FF;
    *c++ = 0x204000;
    *c++ = 0xFF80FF;
    *c++ = 0xFF0000;
    *c++ = 0xE69641C0;
    *c++ = 0x5AFA00;
    *c++ = 0xFF00FF;
    *c++ = 0xFF643200;
    e = fxTrails;
    for (k = 0; k < 32; k++, e++) {
        e->handle = 0;
    }
}
