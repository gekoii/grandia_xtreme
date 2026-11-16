#include "common.h"
#include "protos/main.h"
#include "gph.h"

#define UNIT_MATRIX(m) __asm__ volatile( \
    "vmr32.xyzw $vf5, $vf0\n\t" \
    "vmr32.xyzw $vf6, $vf5\n\t" \
    "vmr32.xyzw $vf7, $vf6\n\t" \
    "sqc2 $vf0, 0x30(%0)\n\t" \
    "sqc2 $vf5, 0x20(%0)\n\t" \
    "sqc2 $vf6, 0x10(%0)\n\t" \
    "sqc2 $vf7, 0x0(%0)" \
    : : "r"(m) : "memory")

extern void MemSet(void *, s32, s32);

/* The empty do-while(0) after the type store is a scheduling barrier: it keeps
 * 5 and 0x80 live together before it, and h (0x1C0) set first after it, which
 * gives the original's v0/v1/a1 for the constants. */

void GphShatterInit(void) {
    GphBatchQuad *p;
    GphShatterPiece **q;
    GphShatter *f;
    u8 c80;
    s16 h, w;

    p = gphShatter.prim;
    f = &gphShatter;
    MemSet(f, 0, 0x2C0);
    q = &gphShatterPieces;
    *q = (GphShatterPiece *)MemAlloc32(0x5000);
    MemSet(*q, 0, 0x5000);
    f->color[3] = 0.0f;
    f->color[0] = 128.0f;
    f->color[1] = 128.0f;
    f->color[2] = 128.0f;
    UNIT_MATRIX(&f->pad30[0x10]); /* matrix at +0x40 */
    p->type = 5;
    c80 = 0x80;
    do {
    } while (0);
    h = 0x1C0;
    w = 0x280;
    p->v[0].r = 0;
    p->v[0].g = 0;
    p->v[0].b = 0;
    p->v[0].a = c80;
    p->v[1].r = 0;
    p->v[1].g = 0;
    p->v[1].b = 0;
    p->v[1].a = c80;
    p->v[2].r = 0;
    p->v[2].g = 0;
    p->v[2].b = 0;
    p->v[2].a = c80;
    p->v[3].r = 0;
    p->v[3].g = 0;
    p->v[3].b = 0;
    p->v[3].a = c80;
    p->v[0].x = 0;
    p->v[0].y = 0;
    p->v[1].x = 0;
    p->v[1].y = h;
    p->v[2].x = w;
    p->v[2].y = 0;
    p->z = 0;
    p->v[3].x = w;
    p->v[3].y = h;
    p[1].type = 0;
    ChrModelPoolInit((struct ChrModelPool *)gphShatter.modelPool, MemAlloc128(0x20000), 0x20000);
}
