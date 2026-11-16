#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccMapJumpForwardStart(OccEntry *obj) {
    OccMapJump *s = OCC_PARAMS(obj, OccMapJump);
    ChrObj *e;
    s32 time;
    s32 mode;
    s32 r;
    VuVec3 v;

    e = ChrGetByNo(1);
    s->unk14 = 0;
    s->unk15 = 0;
    if (obj->flags & 0x100) {
        time = s->fadeTime;
        mode = s->fade;
    } else {
        r = MapIsCached(s->map);
        time = 6;
        if (r != 1) {
            time = MapGetMdtSizeMB(s->map, time) * 30;
            if (time == 0) {
                time = 15;
            }
        }
        mode = 1;
    }
    obj->timer = time + 2;
    s->wait = 0;
    s->frame = 0;
    switch (mode) {
    case 0:
        break;
    case 1:
        GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 128.0f, time);
        break;
    case 2:
        GphFadeStart(fldFades, 0xFF, 0.0f, 255.0f, 255.0f, 128.0f, time);
        break;
    }
    FldInputLockSet(1);
    e->vel[0] = 0.0f;
    e->vel[1] = 0.0f;
    e->vel[2] = 0.0f;
    v.x = e->model->pos[0];
    v.y = e->model->pos[1];
    v.z = e->model->pos[2];
    switch (s->walkDir) {
    case 0:
        break;
    case 1:
        v.z += 1000.0f;
        break;
    case 2:
        v.x -= 1000.0f;
        v.z += 1000.0f;
        break;
    case 3:
        v.x -= 1000.0f;
        break;
    case 4:
        v.x -= 1000.0f;
        v.z -= 1000.0f;
        break;
    case 5:
        v.z -= 1000.0f;
        break;
    case 6:
        v.x += 1000.0f;
        v.z -= 1000.0f;
        break;
    case 7:
        v.x += 1000.0f;
        break;
    case 8:
        v.x += 1000.0f;
        v.z += 1000.0f;
        break;
    case 9:
        v.x = s->x;
        v.z = -s->z;
        break;
    }
    if (s->walkDir != 0) {
        FldObjMoveTo(e, &v, 0, 10.0f);
    }
}
