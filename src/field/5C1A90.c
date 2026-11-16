/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n14.h"

s32 ChrTenantIsOutsideArea(ChrObj *a, f32 *pos) {
    ChrTenantRect r;
    ChrTenantArea *p;

    if (a->aiMode == 0) {
        return 0;
    }
    p = a->areas[a->area];
    if (p->x0 <= p->x1) {
        r.x0 = p->x0;
        r.x1 = p->x1;
    } else {
        r.x0 = p->x1;
        r.x1 = p->x0;
    }
    p = a->areas[a->area];
    if (p->z0 <= p->z1) {
        r.z0 = p->z0;
        r.z1 = p->z1;
    } else {
        r.z0 = p->z1;
        r.z1 = p->z0;
    }
    if (r.x0 <= pos[0] && pos[0] <= r.x1 && r.z0 <= pos[2] && pos[2] <= r.z1) {
        return 0;
    }
    return 1;
}
