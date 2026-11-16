#include "common.h"
#include "gph.h"

s32 ChrMotGetLastFrame(GphModel *m) {
    GphMotion *a;

    if (m->motionMode == 2) {
        a = m->motion[1].motion;
    } else {
        a = m->motion[0].motion;
    }
    if (a == NULL) {
        return 0;
    }
    return a->frameCount - 1;
}
