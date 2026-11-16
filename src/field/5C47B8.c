/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "field/n14.h"

f32 VuVecDot(VuVec4 *, VuVec4 *);
static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

s32 ChrTenantMoveToward(ChrObj *obj, f32 *tgt, s32 n, s32 k) {
    VuVec4 d;
    f32 *pos;
    f32 len;
    f32 inv;
    f32 s;
    f32 vy;
    f32 gy;
    f32 ty;
    f32 diff;

    if (n == 0) {
        return 0;
    }
    pos = obj->model->pos;
    d.x = tgt[0] - pos[0];
    d.y = 0.0f;
    d.z = tgt[2] - pos[2];
    d.w = 0.0f;
    if (obj->flags & 0x2000) {
        len = fsqrt(VuVecDot(&d, &d));
        inv = 1.0f / len;
        if (n < len) {
            s = inv * n;
            obj->vel[0] = d.x * s;
            obj->vel[2] = d.z * s;
            vy = obj->vel[1];
            if (vy <= 0.0f) {
                if (obj->htaArea != 0) {
                    obj->vel[1] = 0.0f;
                    return 0;
                }
                gy = obj->model->pos[1];
                if (0.0f <= gy) {
                    ty = obj->moveTarget[1];
                    if (0.0f <= ty) {
                        diff = gy - ty;
                    } else {
                        diff = gy + __builtin_fabsf(ty);
                    }
                } else {
                    diff = obj->moveTarget[1];
                    if (0.0f <= diff) {
                        diff = 0.0f;
                    } else {
                        diff = __builtin_fabsf(diff) - __builtin_fabsf(gy);
                    }
                }
                if (__builtin_fabsf(diff) > 16.333334f) {
                    if (diff < __builtin_fabsf(vy)) {
                        obj->vel[1] = -diff + 16.333334f;
                        return 0;
                    }
                } else {
                    obj->vel[1] = 0.0f;
                    return 0;
                }
            }
            return 1;
        }
        obj->vel[0] = d.x;
        if (!(obj->flags & 4)) {
            obj->vel[1] = 0.0f;
        } else {
            obj->vel[1] = d.y;
        }
        obj->vel[2] = d.z;
        return 0;
    }
    len = fsqrt(VuVecDot(&d, &d));
    inv = 1.0f / len;
    if (!(obj->flags & 4)) {
        d.y = tgt[1] - pos[1];
    } else {
        d.y = 0.0f;
    }
    if (n < len) {
        s = inv * n;
        obj->vel[1] = d.y * s * k;
        obj->vel[0] = d.x * s * k;
        obj->vel[2] = d.z * s * k;
        if (obj->flags & 4) {
            if (__builtin_fabsf(d.x) < 0.5f && __builtin_fabsf(d.z) < 0.5f) {
                obj->speed = len;
                obj->vel[0] = 0.0f;
                obj->vel[1] = 0.0f;
                obj->vel[2] = 0.0f;
                return 0;
            }
        }
        return 1;
    }
    obj->vel[0] = d.x * k;
    obj->vel[1] = d.y * k;
    obj->vel[2] = d.z * k;
    return 0;
}
