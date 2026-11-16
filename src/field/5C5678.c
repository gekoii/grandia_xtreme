#include "common.h"
#include "field/n14.h"

void *FldMemAlloc(s32);

void ChrTenantAreaCreateAround(ChrObj *obj, VuVec3 *pos) {
    ChrTenantArea *e;
    s32 i;

    e = FldMemAlloc(0x68);
    e->kind = 0;
    e->id = 0;
    e->x0 = pos->x - 1280.0f;
    e->z0 = pos->z - 1280.0f;
    e->x1 = pos->x + 1280.0f;
    e->z1 = pos->z + 1280.0f;
    e->centre.x = pos->x;
    e->centre.y = pos->y;
    e->centre.z = pos->z;
    for (i = 0; i < 6; i++) {
        e->points[i].x = pos->x;
        e->points[i].y = pos->y;
        e->points[i].z = pos->z;
    }
    for (i = 0; i < 8; i++) {
        obj->areas[i] = e;
    }
}
