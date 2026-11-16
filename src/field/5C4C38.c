#include "common.h"
#include "protos/field.h"
#include "field/n14.h"

void ChrTenantAngleToVec(f32 r, f32 pitch, f32 yaw, VuVec3 *out) {
    f32 cp, cy, sp, sy;

    if (__builtin_fabsf(pitch) == 90.0f) {
        cp = 0.0f;
    } else {
        cp = cosf(pitch * 0.017453292f);
    }
    if (__builtin_fabsf(yaw) == 90.0f) {
        cy = 0.0f;
    } else {
        cy = cosf(yaw * 0.017453292f);
    }
    if (__builtin_fabsf(pitch) == 180.0f) {
        sp = 0.0f;
    } else {
        sp = sinf(pitch * 0.017453292f);
    }
    if (__builtin_fabsf(yaw) == 180.0f) {
        sy = 0.0f;
    } else {
        sy = sinf(yaw * 0.017453292f);
    }
    out->y = 0.0f;
    out->x = r * sp * sy;
    out->z = -(r * sp * cy);
}
