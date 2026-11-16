#include "common.h"
#include "protos/viewer.h"
#include "viewer/n35.h"


s32 VwrMotRequest(GphModel *p, GphMotion *m) {
    if (m == NULL) {
        return -1;
    }
    if (p == NULL) {
        return -2;
    }
    if (p->motionMode == 2) {
        if (p->motion[1].motion == m) {
            return -7;
        }
    } else if (p->motion[0].motion == m) {
        return -8;
    }
    if (m->partCount != p->partCount) {
        return -9;
    }
    if (p->motion[0].motion == NULL) {
        p->motion[0].motion = m;
        p->motion[0].frame = 0.0f;
        p->motionMode = 0;
        p->blend = 0.0f;
    } else {
        p->motion[1].motion = m;
        p->motionMode = 2;
        p->motion[1].frame = 0.0f;
        p->blend = 0.0f;
    }
    ChrModelClearDeformBufs(p);
    return 0;
}
