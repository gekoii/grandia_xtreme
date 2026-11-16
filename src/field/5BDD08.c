#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccLightUpdate(s16 *arg0) {
    OccTween *a = OCC_PARAMS(arg0, OccTween);
    VuVec4 v;
    VuVec4 r;
    VuVec4 q;
    GphLight *e;

    e = FldPointLightGet(a->id);
    switch (a->ease) {
    case 0:
        if (((OccEntry *)arg0)->state == 4) {
            v.x = a->delta.f[0];
            v.y = a->delta.f[1];
            v.z = a->delta.f[2];
        } else {
            v.x = -a->delta.f[0];
            v.y = -a->delta.f[1];
            v.z = -a->delta.f[2];
        }
        switch (a->target) {
        case 0:
            e->color[0] += v.x;
            e->color[1] += v.y;
            e->color[2] += v.z;
            break;
        case 1:
            e->range += v.x;
            break;
        case 2:
            e->intensity += v.x;
            break;
        case 3:
            r.x = -e->dir[0];
            r.y = -e->dir[1];
            r.z = -e->dir[2];
            OccRotToVecStub(&r, &q);
            q.x += v.x;
            q.y += v.y;
            OccVecToRotStub(&q, &r);
            e->dir[0] = r.x;
            e->dir[1] = r.y;
            e->dir[2] = r.z;
            break;
        }
        break;
    case 1 ... 4:
        OccInterpVec((OccEntry *)arg0, &v.x);
        switch (a->target) {
        case 0:
            e->color[0] = v.x;
            e->color[1] = v.y;
            e->color[2] = v.z;
            break;
        case 1:
            e->range = v.x;
            break;
        case 2:
            e->intensity = v.x;
            break;
        case 3:
            OccVecToRotStub(&v, &r);
            e->dir[0] = r.x;
            e->dir[1] = r.y;
            e->dir[2] = r.z;
            break;
        }
        break;
    }
}
