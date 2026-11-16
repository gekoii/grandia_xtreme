/* Port-only C for BtlChrGetHeight (0x006AB078), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"

/* The highest part origin of the unit's rest pose (the parts' rest matrices
 * applied to (0, 0, 0, 1), every part but the last), not below 0; without a
 * model, 100 above the unit's position */
f32 BtlChrGetHeight(BtlUnit *u)
{
    GphModel *o = u->model;
    f32 *m;
    f32 h;
    f32 y;
    s32 i;

    if (o == NULL) {
        return u->pos[1] + 100.0f;
    }
    h = 0.0f;
    m = o->restMtx;
    for (i = 0; i < o->partCount - 1; i++) {
        y = m[13] + 0.0f; /* the y of the translation row */
        if (h < y) {
            h = y;
        }
        m += 16;
    }
    return h;
}
