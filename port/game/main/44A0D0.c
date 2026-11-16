/* Port-only C for GphQuad3DSetCorners (0x0044A0D0), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "vu0_port.h"

extern const f32 gphQuadCorners[4][4];

void GphQuad3DSetCorners(GphQuad3D *q, f32 *mtx, f32 *center, f32 w, f32 h);

/*
 * The quad's corners: the unit corners scaled by (w, h, 0, 0), rotated by
 * mtx when there is one (its row 3 counts 0 times), plus centre (xyz; w is
 * the centre's).
 */
void GphQuad3DSetCorners(GphQuad3D *q, f32 *mtx, f32 *center, f32 w, f32 h) {
    f32 corners[4][4];
    f32 s[4];
    s32 k;
    s32 i;

    __builtin_memcpy(corners, gphQuadCorners, sizeof(corners));
    s[0] = w;
    s[1] = h;
    s[2] = 0.0f;
    s[3] = 0.0f;
    for (k = 0; k < 4; k++) {
        f32 v[4];

        for (i = 0; i < 4; i++) {
            corners[k][i] = s[i] * corners[k][i];
        }
        if (mtx != NULL) {
            VuApplyMtx(corners[k], corners[k], mtx);
        }
        VuCopy4(v, center);
        for (i = 0; i < 3; i++) {
            v[i] = center[i] + corners[k][i];
        }
        VuCopy4(&q->pos[k], v);
    }
}
