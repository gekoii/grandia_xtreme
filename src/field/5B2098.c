/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"

f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);
void *memset(void *, s32, u32);

void FldDirToRot(f32 *dir, f32 *rot) {
    f32 a[4];
    f32 b[4];
    f32 z[4];
    f32 d;

    memset(z, 0, 0x10);
    z[2] = 1.0f;
    a[0] = dir[0];
    a[1] = 0.0f;
    a[2] = dir[2];
    a[3] = 0.0f;
    VuVecNormalize(a, a);
    d = VuVecDot(a, z);
    if (d <= -1.0f) {
        rot[1] = 3.141592f;
    } else {
        rot[1] = VuAcosf(d);
    }
    if (dir[0] > 0.0f) {
        rot[1] = -rot[1];
    }
    b[0] = dir[0];
    b[1] = dir[1];
    b[2] = dir[2];
    b[3] = 0.0f;
    VuVecNormalize(b, b);
    d = VuVecDot(b, a);
    if (d <= 1.0f) {
        rot[0] = 0.0f;
    } else {
        rot[0] = VuAcosf(d);
    }
    if (dir[1] > 0.0f) {
        rot[0] = -rot[0];
    }
    rot[2] = 0.0f;
}
