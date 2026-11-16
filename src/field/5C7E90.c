#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"


s32 ChrMotSetImmediate(ChrObj *o, u8 id) {
    GphModel *a;
    ChrMotTable *p;
    GphMotion *m;
    u64 hi;

    hi = id; hi >>= 7;
    id &= 0x7F;
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
    /* identical arms: the branch on hi stays in the code (one plain load is 70 words off); byte-offset indexing (p->slots[id] is 54 words off) */
    if (hi) {
        m = (GphMotion *)((s32 *)((u8 *)p + (id << 2)))[1];
    } else {
        m = (GphMotion *)((s32 *)((u8 *)p + (id << 2)))[1];
    }
    if (m == (GphMotion *)0xFFFFFFFF) {
        return -10;
    }
    if (m->partCount != o->model->partCount) {
        return -9;
    }
    o->model->motion[0].motion = m;
    o->model->motion[0].frame = 0.0f;
    o->model->motionMode = 0;
    o->model->blend = 0.0f;
    ChrMotFlagsSet((u32 *)o->model, 3, 1);
    ChrMotFlagsSet((u32 *)o->model, 0xC, 0);
    ChrModelClearDeformBufs(o->model);
    return 0;
}
