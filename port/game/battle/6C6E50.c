/* Port-only C for BtlVecRandomDir (0x006C6E50), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/battle.h"
#include "vu.h"

void VuMtxSetRotX(FMATRIX, s32);

/* A random direction: (0, 1, 0, 1) turned by a random angle about x, then a
 * random one about y, into out (xyzw; w stays 1) */
void BtlVecRandomDir(f32 *out)
{
    FMATRIX m;
    f32 r[4];
    s32 i;

    VuMtxSetRotX(m, rand() >> 15);
    VuMtxRotY((s32)m, rand() >> 15, (s32)m);
    out[0] = 0.0f;
    out[1] = 1.0f;
    out[2] = 0.0f;
    out[3] = 1.0f;
    for (i = 0; i < 4; i++) {
        r[i] = m[0][i] * out[0] + m[1][i] * out[1] + m[2][i] * out[2] + m[3][i] * out[3];
    }
    for (i = 0; i < 4; i++) {
        out[i] = r[i];
    }
}
