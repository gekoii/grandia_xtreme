#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/n14.h"

void ChrMotSetLoopFlag(GphModel *a, u8 v) {
    GphMotion *m;

    if (a->motionMode == 2) {
        m = a->motion[1].motion;
    } else {
        m = a->motion[0].motion;
    }
    if (m != NULL) {
        m->playFlags = v;
    }
}
