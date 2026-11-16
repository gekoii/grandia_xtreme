/* Port-only C for BtlChrPartDirToWorld (0x006AAFB8), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"

/* Turns the direction v (xyzw) by part n's matrix with the translation
 * dropped (its w row element kept), in place; nothing for a unit that isn't
 * active or has no motion */
void BtlChrPartDirToWorld(f32 *v, BtlUnit *u, GphPart *n)
{
    FMATRIX m;
    FVECTOR r;
    s32 i;

    if ((u32)u->id < 16 && u->state != 0 && u->motion != NULL && n != NULL) {
        BtlChrModelUpdate(u);
        __builtin_memcpy(m, n->mtx, sizeof(m));
        m[3][0] = 0.0f;
        m[3][2] = 0.0f;
        m[3][1] = 0.0f;
        for (i = 0; i < 4; i++) {
            r[i] = m[0][i] * v[0] + m[1][i] * v[1] + m[2][i] * v[2] + m[3][i] * v[3];
        }
        for (i = 0; i < 4; i++) {
            v[i] = r[i];
        }
    }
}
