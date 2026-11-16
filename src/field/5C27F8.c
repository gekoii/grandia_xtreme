/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"


static inline void copyq(void *dst, void *src)
{
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

s32 ChrTenantOpWarpToPoint(void *arg0, ChrObj *a, s32 n, s32 lv)
{
    FVECTOR buf;
    GphModel *m;
    f32 t;
    s32 idx;

    if (a->opTimer > 0) {
        if (a->opTimer > 30) {
            GphModel *m = a->model;
            t = m->scale[0] - 0.033333335f;
            a->heading = (a->heading + 0x2000) & 0xFFFF;
            m->scale[1] += 0.06666667f;
            m->scale[0] = t;
            m->scale[2] = t;
            if (t < 0.0f) {
                if (a->flags & 1) {
                    a->flags ^= 1;
                }
                copyq(m->pos, a->moveTarget);
            }
        } else if (a->opTimer == 30) {
            m = a->model;
            a->flags |= 1;
            m->scale[0] = 2.0f;
            m->scale[2] = 2.0f;
            m->scale[1] = 0.0f;
            EffSpawnChara(m->pos, 1);
        } else {
            m = a->model;
            m->scale[0] -= 0.033333335f;
            m->scale[1] += 0.033333335f;
            m->scale[2] -= 0.033333335f;
            a->heading = (a->heading + 0x1000) & 0xFFFF;
        }
        a->opTimer--;
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(a->vel) : "memory");
    } else {
        if (a->opState == 5) {
            m = a->model;
            a->opState = 0;
            m->scale[0] = 1.0f;
            m->scale[1] = 1.0f;
            m->scale[2] = 1.0f;
            return 0;
        }
        a->opState = 5;
        a->opTimer = lv * 60;
        if (a->opTimer < 60) {
            a->opTimer = 60;
        }
        switch (n) {
        case 7:
            idx = VuRandInt(a->areas[a->area]->pointCount) + 1;
            break;
        case 8:
            idx = ChrTenantFindAreaPoint(a, buf, 0);
            break;
        case 9:
            idx = ChrTenantFindAreaPoint(a, buf, 1);
            break;
        default:
            idx = n;
            break;
        }
        ChrTenantAreaGetPoint(a, idx, a->moveTarget);
        EffSpawnChara(a->model->pos, 0);
    }
    a->headingDeg = a->heading * 0.0054931640625f;
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(a->vel) : "memory");
    return 1;
}
