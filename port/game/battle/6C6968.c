/* Port-only C for BtlDirFromPitchYaw (0x006C6968), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/battle.h"
#include "vu.h"

void *memset(void *, s32, u32);
void VuVecCopy(f32 *, f32 *);

/* The unit y axis turned by pitch about x, then yaw about y, into out (xyz) */
void BtlDirFromPitchYaw(s32 pitch, s32 yaw, f32 *out)
{
    FVECTOR v;
    FMATRIX m;
    FVECTOR r;
    s32 i;
    s32 j;

    memset(v, 0, sizeof(v));
    v[1] = 1.0f;
    for (i = 0; i < 4; i++) { /* the unit matrix ($vf0 and its vmr32 rotations) */
        for (j = 0; j < 4; j++) {
            m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    VuMtxRotX((s32)m, pitch, (s32)m);
    VuMtxRotY((s32)m, yaw, (s32)m);
    for (i = 0; i < 4; i++) {
        r[i] = m[0][i] * v[0] + m[1][i] * v[1] + m[2][i] * v[2] + m[3][i] * v[3];
    }
    for (i = 0; i < 4; i++) {
        v[i] = r[i];
    }
    VuVecCopy(out, v);
}
