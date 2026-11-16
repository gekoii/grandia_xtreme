#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/n14.h"

s32 ChrTenantOpAppear(s32 arg0, ChrObj *obj) {
    f32 t;

    if (obj->opTimer > 0) {
        GphModel *m = obj->model;
        obj->opTimer--;
        m->scale[1] += 0.06666667f;
        t = m->scale[0] - 0.06666667f;
        m->scale[0] = t;
        m->scale[2] = t;
        obj->heading = (obj->heading + 0x1000) & 0xFFFF;
    } else if (obj->opState == 0x11) {
        GphModel *m = obj->model;

        m->scale[0] = 1.0f;
        m->scale[1] = 1.0f;
        m->scale[2] = 1.0f;
        obj->opState = 0;
        if (obj->sense != NULL) {
            CHR_SENSE(obj)->flags |= 5;
        }
        return 0;
    } else {
        GphModel *m = obj->model;

        m->scale[1] = 0.0f;
        m->scale[0] = 2.0f;
        m->scale[2] = 2.0f;
        obj->opState = 0x11;
        obj->opTimer = 0xF;
        obj->flags |= 0x801;
        EffSpawnChara((s32)m->pos, 1);
    }
    return 1;
}
