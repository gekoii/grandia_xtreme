/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"

void VuVecCopy(f32 *dst, f32 *src);
static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

s32 ChrTenantOpChaseLeader(void *arg0, ChrObj *a, s32 p1, s32 p2) {
    VuVec3 v;
    ChrObj *c;

    c = ChrGetByNo(1);
    if (a->opState != 6) {
        a->opState = 6;
        VuVecCopy(a->moveTarget, c->model->pos);
        a->opTimer = 0;
    } else if (fsqrt(a->vel[0] * a->vel[0] + a->vel[2] * a->vel[2]) < 0.001f) {
        if (a->opTimer > 0) {
            if (--a->opTimer == 0) {
                VuVecCopy(a->moveTarget, c->model->pos);
            }
        } else {
            a->opTimer = 60;
            a->heading = VuRandInt(0x8000);
            ChrTenantAngleToVec(1.0f, 90.0f, a->headingDeg, &v);
            a->moveTarget[0] = a->model->pos[0] + v.x;
            a->moveTarget[1] = a->model->pos[1];
            a->moveTarget[2] = a->model->pos[2] + v.z;
        }
    } else {
        if (a->opTimer > 0) {
            if (--a->opTimer == 0) {
                VuVecCopy(a->moveTarget, c->model->pos);
            }
        }
    }
    a->headingDeg = a->heading * 0.0054931640625f;
    return ChrTenantMoveToward(a, a->moveTarget, p2, 1);
}
