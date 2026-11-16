#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/n14.h"

s32 ChrTenantOpVanish(s32 arg0, ChrObj *obj) {
    f32 t;
    GphModel *m;

    if (obj->opTimer > 0) {
        m = obj->model;
        obj->opTimer--;
        m->scale[1] += 0.06666667f;
        t = m->scale[0] - 0.06666667f;
        m->scale[0] = t;
        m->scale[2] = t;
        obj->heading = (obj->heading - 0x3000) & 0xFFFF;
    } else if (obj->opState == 0x12) {
        GphModel *m = obj->model;
        obj->opState = 0;
        m->scale[0] = 1.0f;
        m->scale[1] = 1.0f;
        m->scale[2] = 1.0f;
        if (obj->flags & 1) {
            obj->flags ^= 1;
        }
        if (obj->flags & 0x800) {
            obj->flags ^= 0x800;
        }
        if (obj->sense != NULL) {
            CHR_SENSE(obj)->flags ^= 5;
        }
        return 0;
    } else {
        GphModel *m = obj->model;
        obj->opTimer = 0xF;
        m->scale[0] = 1.0f;
        m->scale[1] = 1.0f;
        m->scale[2] = 1.0f;
        obj->opState = 0x12;
        EffSpawnChara((s32)m->pos, 0);
    }
    return 1;
}
