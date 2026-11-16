/* Port-only C for EvtAreaHitBoundSphere (0x005B29D0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/event.h"

void VuVecCopy(f32 *, f32 *);
s32 EvtAreaHitBoundSphere(f32 *p, f32 r, EvtArea *e);

/* Whether a sphere at p of radius r reaches the area's bounding sphere:
   the centre is ofs through the world matrix when the area has one
   (flags & 4), else pos (plus the linked model's position). */
s32 EvtAreaHitBoundSphere(f32 *p, f32 r, EvtArea *e) {
    FVECTOR c;
    f32 d;
    f32 rad;
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
    } else {
        VuVecCopy(c, e->pos);
    }
    c[0] = p[0] - c[0];
    c[1] = p[1] - c[1];
    c[2] = p[2] - c[2];
    d = __builtin_sqrtf(c[0] * c[0] + c[1] * c[1] + c[2] * c[2]);
    rad = e->radius;
    if (e->flags & EVT_AREA_SCALED) {
        rad *= e->unk5C;
    }
    return d < r + rad;
}
