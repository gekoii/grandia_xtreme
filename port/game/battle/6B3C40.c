/* Port-only C for BtlChrAttachToPart (0x006B3C40), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "vu.h"

void VuVecCopy(f32 *, f32 *);
void VuMtxNormalizeAxes(f32 (*)[4], f32 *);

/* Places model o at part n's matrix: the position is n->mtx applied to the
 * point ofs ((0, 0, 0) when NULL), the angles those of the normalised matrix
 * (the same as MgcPtclGroupAttachToPart, 0x006B3190, but into a model) */
void BtlChrAttachToPart(GphModel *o, GphPart *n, f32 *ofs)
{
    FMATRIX m;
    FVECTOR v;
    f32 *t;
    s32 i;

    if (o == NULL || n == NULL) {
        return;
    }
    v[0] = v[1] = v[2] = 0.0f; /* $vf0 */
    v[3] = 1.0f;
    if (ofs != NULL) {
        VuVecCopy(v, ofs);
    }
    t = n->mtx;
    for (i = 0; i < 4; i++) {
        o->pos[i] = t[i] * v[0] + t[4 + i] * v[1] + t[8 + i] * v[2] + t[12 + i] * v[3];
    }
    VuMtxNormalizeAxes(m, n->mtx);
    VuMatrixToEuler(o->rot, (f32 *)m);
}
