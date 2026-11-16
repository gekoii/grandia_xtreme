/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/n14.h"

void ChrTenantDebugDrawArea(ChrObj *obj) {
    ChrTenantRect r;
    VuVec4 v[5];

    if (obj->areas[obj->area]->x0 <= obj->areas[obj->area]->x1) {
        r.x0 = obj->areas[obj->area]->x0;
        r.x1 = obj->areas[obj->area]->x1;
    } else {
        r.x0 = obj->areas[obj->area]->x1;
        r.x1 = obj->areas[obj->area]->x0;
    }
    if (obj->areas[obj->area]->z0 <= obj->areas[obj->area]->z1) {
        r.z0 = obj->areas[obj->area]->z0;
        r.z1 = obj->areas[obj->area]->z1;
    } else {
        r.z0 = obj->areas[obj->area]->z1;
        r.z1 = obj->areas[obj->area]->z0;
    }
    v[0].x = r.x0;
    v[0].y = obj->model->pos[1] + 64.0f;
    v[0].z = r.z0;
    v[0].w = 1.0f;
    v[1].x = r.x1;
    v[1].y = obj->model->pos[1] + 64.0f;
    v[1].z = r.z0;
    v[1].w = 1.0f;
    v[2].x = r.x1;
    v[2].y = obj->model->pos[1] + 64.0f;
    v[2].z = r.z1;
    v[2].w = 1.0f;
    v[3].x = r.x0;
    v[3].y = obj->model->pos[1] + 64.0f;
    v[3].z = r.z1;
    v[3].w = 1.0f;
    v[4].x = r.x0;
    v[4].y = obj->model->pos[1] + 64.0f;
    v[4].z = r.z0;
    v[4].w = 1.0f;
    GphLine3DDraw(5, v, 0x80FFFFFF, 1);
}
