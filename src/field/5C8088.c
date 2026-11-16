/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"

s32 ChrMotIsEnd(GphModel *m) {
    GphMotion *a;

    if (m->motionMode == 2) {
        return -1;
    }
    a = m->motion[0].motion;
    if (a == NULL) {
        return -1;
    }
    if ((f32)a->frameCount - 1.0f <= m->motion[0].frame) {
        return 1;
    }
    return 0;
}
