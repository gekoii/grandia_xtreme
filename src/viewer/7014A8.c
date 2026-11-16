/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "viewer/n35.h"
#include "protos/viewer.h"

void VuVecNormalize(VuVec4 *, VuVec4 *);

void VwrDirToPitchYaw(VuVec4 *dir, f32 *pitch, f32 *yaw) {
    VuVec4 v;
    f32 y;
    f32 x;
    f32 z;

    y = dir->y;
    x = dir->x;
    z = dir->z;
    if (y > 0.99f) {
        *pitch = 1.5393801f;
    } else if (y < -0.99f) {
        *pitch = -1.5393801f;
    } else {
        v.x = x;
        v.y = 0.0f;
        v.z = z;
        v.w = 0.0f;
        VuVecNormalize(&v, &v);
        if (z < 0.0f) {
            *yaw = vwr_asinf(-v.x);
        } else if (x < 0.0f) {
            *yaw = 3.141592f - vwr_asinf(-v.x);
        } else {
            *yaw = -3.141592f - vwr_asinf(-v.x);
        }
        *pitch = vwr_asinf(y);
    }
}
