#include "common.h"
#include "field/state.h"
#include "field/n14.h"

void ChrTenantAreaGetPoint(ChrObj *a, s32 n, VuVec3 *out) {
    VuVec3 *v;

    if (n == 0) {
        out->x = a->areas[a->area]->centre.x;
        out->y = a->areas[a->area]->centre.y;
        out->z = a->areas[a->area]->centre.z;
        return;
    }
    v = &a->areas[a->area]->points[n - 1];
    out->x = v->x;
    out->y = v->y;
    out->z = v->z;
}
