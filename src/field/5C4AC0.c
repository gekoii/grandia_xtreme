/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

s32 ChrTenantFindAreaPoint(ChrObj *obj, VuVec3 *out, s8 far) {
    ChrTenantArea *p;
    s32 i;
    s32 idx;
    f32 best;
    f32 d;
    VuVec3 v;

    best = 0.0f;
    if (far == 0) {
        best = 65535.0f;
    }
    p = obj->areas[0];
    idx = -1;
    for (i = 0; i < 6; i++) {
        v.x = obj->areas[0]->points[i].x;
        v.y = obj->areas[0]->points[i].y;
        v.z = obj->areas[0]->points[i].z;
        if (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f) {
            continue;
        }
        d = fsqrt(__builtin_fabsf(v.x - obj->model->pos[0]) * __builtin_fabsf(v.x - obj->model->pos[0])
                + __builtin_fabsf(v.y - obj->model->pos[1]) * __builtin_fabsf(v.y - obj->model->pos[1])
                + __builtin_fabsf(v.z - obj->model->pos[2]) * __builtin_fabsf(v.z - obj->model->pos[2]));
if (!far) { if (d < best) { best = d; idx = i; } } else if (best < d) { best = d; idx = i; }
    }
    if (idx != -1) {
        out->x = p->points[idx].x;
        out->y = p->points[idx].y;
        out->z = p->points[idx].z;
        return idx + 1;
    }
    ChrTenantAreaGetPoint(obj, 0, out);
    return 0;
}
