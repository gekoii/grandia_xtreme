#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"

typedef struct {
    VuVec3 pos;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
} Mark;

extern Mark occLightSaveSlots[];

void OccLightForwardStart(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccTween *a = OCC_PARAMS(obj, OccTween);
    VuVec4 r;
    GphLight *e;
    Mark *m;

    e = FldPointLightGet(a->id);
    if (e == NULL) {
        OccEnd(obj);
        return;
    }
    m = NULL;
    if (a->save == 1 && a->id < 16) {
        m = &occLightSaveSlots[a->id];
    }
    obj->timer = a->time;
    switch (a->ease) {
    case 0:
        a->delta.f[0] = a->to.f[0];
        a->delta.f[1] = a->to.f[1];
        a->delta.f[2] = a->to.f[2];
        break;
    case 1 ... 4:
        switch (a->target) {
        case 0:
            a->from[0] = e->color[0];
            a->from[1] = e->color[1];
            a->from[2] = e->color[2];
            if (m != NULL) {
                m->pos.x = e->color[0];
                m->pos.y = e->color[1];
                m->pos.z = e->color[2];
            }
            break;
        case 1:
            a->from[0] = e->range;
            if (m != NULL) {
                m->unkC = e->range;
            }
            break;
        case 2:
            a->from[0] = e->intensity;
            if (m != NULL) {
                m->unk14 = e->intensity;
            }
            break;
        case 3:
            r.x = -e->dir[0];
            r.y = -e->dir[1];
            r.z = -e->dir[2];
            OccRotToVecStub(&r, a->from);
            if (m != NULL) {
                m->unk1C = a->from[0];
                m->unk20 = a->from[1];
            }
            break;
        }
        if (a->save == 2 && a->id < 16) {
            m = &occLightSaveSlots[a->id];
            switch (a->target) {
            case 0:
                a->to.f[0] = m->pos.x;
                a->to.f[1] = m->pos.y;
                a->to.f[2] = m->pos.z;
                break;
            case 1:
                a->to.f[0] = m->unkC;
                a->to.f[1] = m->unk10;
                a->to.f[2] = 0.0f;
                break;
            case 2:
                a->to.f[0] = m->unk14;
                a->to.f[1] = m->unk18;
                a->to.f[2] = 0.0f;
                break;
            case 3:
                a->to.f[0] = m->unk1C;
                a->to.f[1] = m->unk20;
                a->to.f[2] = 0.0f;
                break;
            }
        }
        a->delta.f[0] = a->to.f[0] - a->from[0];
        a->count = 0;
        a->delta.f[1] = a->to.f[1] - a->from[1];
        a->delta.f[2] = a->to.f[2] - a->from[2];
        break;
    }
}
