#include "common.h"
#include "protos/main.h"
#include "gph.h"




s8 GphMeshClipTest(GphGaObject *);

#define VADD(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyzw $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

void GphModelPartDraw(GphPart *o, s32 arg1)
{
    f32 *base;
    s32 bit;
    s32 off;
    s32 alt;
    s32 t;
    GphGaObject *e;
    s32 n;
    s32 drawn;
    s32 total;
    s8 r;
    f32 sx, sy;
    u16 fl;

    bit = 0;
    off = *(s32 *)0x70002804;
    base = (void *)0x70002740;
    fl = o->attr;
    o->drawn = 1;
    alt = o->drawFlags & 1;
    if (fl & 1) {
        bit = (fl & 0x30) != 0;
        if (fl & 0x20) {
            base = (void *)0x70002750;
        }
    }
    if (alt) {
        e = o->gaObjsCopy;
    } else {
        e = o->gaObjs;
    }
    n = o->gaObjCount;
    t = *(s32 *)0x70002704;
    VADD(o->ambient, base, o->color);
    drawn = 0;
    total = 0;
    sx = o->uvScroll[0] * 0.00024414062f;
    sy = o->uvScroll[2] * 0.00024414062f;
    do {
        total++;
        r = 0;
        if (off == 0) {
            r = 7;
            if (!alt) {
                e->mtx = arg1;
                r = GphMeshClipTest(e);
            }
        }
        e->clip = r;
        if (r >= 0) {
            e->ambient = o->ambient;
            drawn++;
            e->lightBlock = t;
            e->gsPkt = 0;
            e->uOffset = sx;
            e->vOffset = sy;
            e->flags = (e->flags & 0xFE) | bit;
            GphDrawListAdd(o, e, arg1);
        }
        n--;
        e++;
    } while (n > 0);
    gphRenderCtx->gaObjDrawn += drawn;
    gphRenderCtx->gaObjCount += total;
    gphRenderCtx->modelCount++;
}
