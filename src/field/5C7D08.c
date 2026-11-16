#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"


s32 ChrMotSet(ChrObj *o, u8 id) {
    GphModel *a;
    ChrMotTable *p;
    GphMotion *m;
    s32 blend;
    s32 f;

    if (o == NULL) {
        return -1;
    }
    a = o->model;
    if (a == NULL) {
        return -2;
    }
    p = o->motTable;
    if (p == NULL) {
        return -3;
    }
    if (id >= 64) {
        return -4;
    }
    if (((s32 *)((u8 *)p + (id << 2)))[1] == 0xFFFFFFFF) {
        return -6;
    }
    if (a->motionMode == 2) {
        if (a->motion[1].motion == (GphMotion *)((s32 *)((u8 *)p + (id << 2)))[1]) {
            return -7;
        }
    } else if (a->motion[0].motion == (GphMotion *)((s32 *)((u8 *)p + (id << 2)))[1]) {
        return -8;
    }
    m = (GphMotion *)((s32 *)((u8 *)p + (id << 2)))[1];
    if (m == (GphMotion *)0xFFFFFFFF) {
        return -10;
    }
    if (m->partCount != o->model->partCount) {
        return -9;
    }
    f = m->flags & 0x10;
    blend = f == 0;
    if (o->model->motion[0].motion != NULL) {
        if (o->model->motion[0].motion->flags & 0x10) blend = 0;
    } else {
        blend = 0;
    }
    if (!blend) {
        a->motion[0].motion = m;
        a->motion[0].frame = 0.0f;
        a->motionMode = 0;
        a->blend = 0.0f;
    } else {
        a->motion[1].motion = m;
        a->motionMode = 2;
        a->motion[1].frame = 0.0f;
        a->blend = 0.0f;
    }
    ChrMotFlagsSet((u32 *)o->model, 3, 1);
    ChrMotFlagsSet((u32 *)o->model, 0xC, 0);
    ChrModelClearDeformBufs(o->model);
    return 0;
}
