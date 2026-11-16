/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n15.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


void CamBlendStart(f32 t) {
    ChrObj *o = ChrGetByNo(1);
    f32 d;

    camBlend.from = camField[0];
    camBlend.active = 1;
    camBlend.duration = t;
    camBlend.t = 0;
    copyq(camBlend.to.lookAt, o->model->pos);
    camBlend.to.lookAt[1] += camCtl.height;
    if (camBlend.from.yaw < 0.0f) {
        camBlend.from.yaw += 6.283184f;
    } else if (camBlend.from.yaw >= 6.283184f) {
        camBlend.from.yaw -= 6.283184f;
    }
    d = ChrAngleDiffDeg(camBlend.from.yaw * 57.295776f, camBlend.to.yaw * 57.295776f) + camBlend.from.yaw * 57.295776f;
    if ((d > -361.0f && d < -359.0f) || (d < 1.0f && d > -1.0f)) {
        camBlend.from.yaw = camBlend.to.yaw;
    } else {
        camBlend.to.yaw = d * 0.017453292f;
    }
}
