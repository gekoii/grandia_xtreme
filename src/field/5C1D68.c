/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/n14.h"

void ChrTenantDebugDrawCurArea(ChrObj *a) {
    ChrTenantRect r;
    VuVec4 v[5];
    ChrTenantArea *p;
    GphModel *o;

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
    o = a->model;
    v[0].x = r.x0;
    v[0].y = o->pos[1] + 64.0f;
    v[0].z = r.z0;
    v[0].w = 1.0f;
    v[1].x = r.x1;
    v[1].y = o->pos[1] + 64.0f;
    v[1].z = r.z0;
    v[1].w = 1.0f;
    v[2].x = r.x1;
    v[2].y = o->pos[1] + 64.0f;
    v[2].z = r.z1;
    v[2].w = 1.0f;
    v[3].x = r.x0;
    v[3].y = o->pos[1] + 64.0f;
    v[3].z = r.z1;
    v[3].w = 1.0f;
    v[4].x = r.x0;
    v[4].y = o->pos[1] + 64.0f;
    v[4].z = r.z0;
    v[4].w = 1.0f;
    GphLine3DDraw(5, v, 0x8000FFFF, 1);
}
