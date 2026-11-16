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

void GphModelPartDrawCulled(GphPart *o, s32 arg1)
{
    s32 alt;
    s32 off;
    s32 lb;
    s32 bit;
    s32 drawn;
    s32 total;
    s32 n;
    u16 fl;
    f32 *base;
    f32 *amb;
    GphGaObject *e;
    s8 r;
    f32 sx;
    f32 sy;

    bit = 0;
    o->drawn = 1;
    alt = o->drawFlags & 1;
    off = *(s32 *)0x70002804;
    base = (f32 *)0x70002740;
    if (alt) {
        e = o->gaObjsCopy;
    } else {
        e = o->gaObjs;
    }
    amb = o->ambient;
    fl = o->attr;
    drawn = 0;
    lb = *(s32 *)0x70002704;
    total = 0;
    if (fl & 1) {
        bit = (fl & 0x30) != 0;
        if (fl & 0x20) {
            base = (f32 *)0x70002750;
        }
    }
    VADD(amb, base, o->color);
    /* the do-while(0) wraps weight the refs of b, k and fb (allocation
     * priority); their loop notes also fix the schedule */
    {
        s32 a = o->uvScroll[0];
        s32 b;
        f32 fa, fb, k;
        do {
            b = o->uvScroll[2];
            fa = a;
            do {
                k = 0.00024414062f;
                fb = b;
            } while (0);
        } while (0);
        n = o->gaObjCount;
        sy = fb * k;
        sx = fa * k;
    }
    do {
        total++;
        if (alt) {
            r = off ? 0 : 7;
        } else {
            e->mtx = arg1;
            r = GphMeshClipTest(e);
        }
        e->clip = r;
        if (r >= 0) {
            e->ambient = amb;
            drawn++;
            e->lightBlock = lb;
            e->gsPkt = 0;
            e->uOffset = sx;
            e->vOffset = sy;
            e->flags = (e->flags & 0xFE) | bit;
            GphDrawListAdd(o, e, arg1);
        }
        n--;
        e++;
    } while (n > 0);
    gphRenderCtx->modelCount++;
    gphRenderCtx->gaObjDrawn += drawn;
    gphRenderCtx->gaObjCount += total;
}
