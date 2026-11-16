/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/state.h"
#include "field/n15.h"

void VuVecCopy(void *, void *);
void VuBezier3(void *, VuVec4 *, f32);

void CamBlendUpdate(void) {
    VuVec4 out;
    VuVec4 k[4];
    CamBlend *b = &camBlend;

    if (b->active) {
        b->t += 1.0f / b->duration;
        if (b->t > 1.0f) {
            b->t = 1.0f;
            b->active = 0;
        }
        VuVecCopy(&k[0], b->from.eye);
        VuVecCopy(&k[1], b->to.eye);
        VuVecCopy(&k[2], &k[1]);
        VuVecCopy(&k[3], &k[1]);
        VuBezier3(camField[0].eye, k, b->t);
        VuVecCopy(&k[0], b->from.lookAt);
        VuVecCopy(&k[1], b->to.lookAt);
        VuVecCopy(&k[2], &k[1]);
        VuVecCopy(&k[3], &k[1]);
        VuBezier3(camField[0].lookAt, k, b->t);

        k[0].x = b->from.pitch;
        k[0].y = b->from.yaw;
        k[0].z = b->from.roll;
        k[1].x = b->to.pitch;
        k[1].y = b->to.yaw;
        k[1].z = b->to.roll;
        VuVecCopy(&k[2], &k[1]);
        VuVecCopy(&k[3], &k[1]);
        VuBezier3(&out, k, b->t);
        camField[0].pitch = out.x;
        camField[0].yaw = out.y;
        camField[0].roll = out.z;

        k[0].x = b->from.dist;
        k[0].y = b->from.zoom;
        k[0].z = b->from.fov;
        k[1].x = b->to.dist;
        k[1].y = b->to.zoom;
        k[1].z = b->to.fov;
        VuVecCopy(&k[2], &k[1]);
        VuVecCopy(&k[3], &k[1]);
        VuBezier3(&out, k, b->t);
        camField[0].dist = out.x;
        camField[0].zoom = out.y;
        camField[0].fov = out.z;

        k[0].x = b->unk300;
        k[1].x = b->unk304;
        k[0].y = 0.0f;
        k[0].z = 0.0f;
        k[1].y = 0.0f;
        k[1].z = 0.0f;
        VuVecCopy(&k[2], &k[1]);
        VuVecCopy(&k[3], &k[1]);
        VuBezier3(&out, k, b->t);
    }
}
