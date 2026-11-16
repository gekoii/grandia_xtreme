/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "pad.h"
#include "field/state.h"

void VuVecCopy(void *, void *);

void CamAiEye(Camera *o, ChrObj *s, Pad *pad) {
    f32 a = o->pitch * 57.295776f;
    f32 d;

    o->mode = CAM_MODE_NORMAL;
    while (a > 180.0f) {
        a -= 180.0f;
    }
    while (a < -180.0f) {
        a += 180.0f;
    }
    if (a < -60.0f) {
        a = -60.0f;
    }
    if (a > 60.0f) {
        a = 60.0f;
    }
    d = 0.0f;
    if (pad->leftY < 0x5A) {
        d -= (f32)((0x5A - pad->leftY) * 2) * 0.0054931641f;
    } else if (pad->leftY > 0xA6) {
        d += (f32)((pad->leftY - 0xA6) * 2) * 0.0054931641f;
    }
    if (a > 1.0f && d > 0.0f) {
        d *= 1.0f - a / 60.0f;
    }
    if (a < -1.0f && d < 0.0f) {
        d *= 1.0f - a / -60.0f;
    }
    a += d;
    o->pitch = a * 0.017453292f;
    if (pad->leftX < 0x50) {
        o->yaw -= (f32)((0x50 - pad->leftX) * 4) * 9.58738e-05f;
    } else if (pad->leftX > 0xB0) {
        o->yaw += (f32)((pad->leftX - 0xB0) * 4) * 9.58738e-05f;
    }
    VuVecCopy(o->lookAt, s->model->pos);
    o->dist = 0.01f;
    s->heading = o->yaw * 10430.378f;
    o->lookAt[1] += camCtl.height * 3.0f;
}
