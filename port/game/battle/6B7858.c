/* Port-only C for EnvFlareCalcPos (0x006B7858), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/battle.h"
#include "battle/env.h"
#include "battle/n31.h"

void VuMtxSetRotX(FMATRIX, s32);

/* envFlarePos: the point envFlareDistance up the y axis, turned by
 * envFlareAngleX about x and envFlareAngleY about y, then negated (xyz).
 * e is not used: the callers copy its flare fields into the globals first
 * (EnvFlareInit). */
void EnvFlareCalcPos(struct EnvParam *e)
{
    FVECTOR v;
    FMATRIX m;
    f32 r[4];
    s32 i;

    v[0] = v[1] = v[2] = 0.0f; /* $vf0 */
    v[3] = 1.0f;
    v[1] = envFlareDistance;
    VuMtxSetRotX(m, envFlareAngleX);
    VuMtxRotY((s32)m, envFlareAngleY, (s32)m);
    for (i = 0; i < 4; i++) {
        r[i] = m[0][i] * v[0] + m[1][i] * v[1] + m[2][i] * v[2] + m[3][i] * v[3];
    }
    envFlarePos[0] = -r[0];
    envFlarePos[1] = -r[1];
    envFlarePos[2] = -r[2];
}
