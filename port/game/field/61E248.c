/* Port-only C for FldCollShapeTest (0x0061E248), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/event.h"

/* The swept sphere FldCollShapeTest takes (the Hit of 0x0061E3C8) */
typedef struct FldCollShapeHit {
    /* 0x00 */ s32 hit;
    /* 0x04 */ u8 pad4[8];
    /* 0x0C */ f32 radius;
    /* 0x10 */ FVECTOR pos;     /* start */
    /* 0x20 */ FVECTOR move;
    /* 0x30 */ FVECTOR unk30;
} FldCollShapeHit;

void VuVecCopy(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);
f32 VuVecDot(f32 *, f32 *);
void FldCollShapeBoxTest(EvtArea *, FldCollShapeHit *);
void FldCollShapeWallTest(EvtArea *, FldCollShapeHit *);
void FldCollShapeTest(EvtArea *e, FldCollShapeHit *s);

/* Tests the sweep against the area's shape (0 box, 1 wall) when the line
   of the sweep passes within the two radii of the area's bounding-sphere
   centre. The asm returns the shape test's v0 (1 for other shapes, junk on
   the early exit); the only caller ignores it, so this is void. */
void FldCollShapeTest(EvtArea *e, FldCollShapeHit *s) {
    FVECTOR c;
    FVECTOR d;
    FVECTOR dir;
    f32 r;
    f32 t;
    s32 i;

    if (e->flags & 4) {
        for (i = 0; i < 4; i++) {
            c[i] = e->mtx[0][i] * e->ofs[0] + e->mtx[1][i] * e->ofs[1]
                 + e->mtx[2][i] * e->ofs[2] + e->mtx[3][i] * e->ofs[3];
        }
    } else if (e->link != NULL) {
        c[0] = e->pos[0] + e->link->pos[0];
        c[1] = e->pos[1] + e->link->pos[1];
        c[2] = e->pos[2] + e->link->pos[2];
        c[3] = e->pos[3];
    } else {
        VuVecCopy(c, e->pos);
    }
    r = e->radius;
    d[0] = c[0] - s->pos[0];
    d[1] = c[1] - s->pos[1];
    d[2] = c[2] - s->pos[2];
    d[3] = c[3];
    VuVecNormalize(dir, s->move);
    /* the part of d across the line */
    t = VuVecDot(dir, d);
    d[0] = d[0] - dir[0] * t;
    d[1] = d[1] - dir[1] * t;
    d[2] = d[2] - dir[2] * t;
    if (r + s->radius < __builtin_sqrtf(VuVecDot(d, d))) {
        return;
    }
    switch (e->shape) {
    case EVT_AREA_BOX:
        FldCollShapeBoxTest(e, s);
        break;
    case 1:
        FldCollShapeWallTest(e, s);
        break;
    }
}
