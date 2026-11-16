/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x00703410-0x00703420 */
#include "common.h"
#include "viewer/n35.h"
#include "protos/viewer.h"

/* VuVec4 with VU (16-byte) alignment, as the stack copies need. */
typedef VuVec4 VuVec4A __attribute__((aligned(16)));

void VuVecNormalize(VuVec4 *, VuVec4 *);
f32 VuVecDot(VuVec4 *, VuVec4 *);

void VwrDirToRotXY(VuVec4 *dir, VuVec4 *out) {
    VuVec4A v;
    VuVec4A u;
    VuVec4A ref = { 0.0f, 0.0f, 1.0f, 1.0f };

    v.x = dir->x;
    v.y = 0.0f;
    v.z = dir->z;
    VuVecNormalize(&v, &v);
    out->y = vwr_acosf(VuVecDot(&v, &ref));
    if (dir->x > 0.0f) {
        out->y = -out->y;
    }
    u.x = dir->x;
    u.y = dir->y;
    u.z = dir->z;
    VuVecNormalize(&u, &u);
    out->x = vwr_acosf(VuVecDot(&u, &v));
    if (dir->y > 0.0f) {
        out->x = -out->x;
    }
    out->z = 0.0f;
}
