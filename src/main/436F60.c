/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "main/n04.h"


s32 ChrModelMotionUpdate(GphModel *m, f32 dt) {
    GphModelMotion *c = (GphModelMotion *)&m->motionMode;
    s32 ret = 0;
    f32 t;

    if (c->slot[0].motion != NULL) {
        t = ChrMotionStepFrame(c->slot[0].motion, c->slot[0].frame, dt);
        c->slot[0].frame = t;
        if (!(c->slot[0].motion->playFlags & 1) && (f32)(c->slot[0].motion->frameCount - 1) <= t) {
            ret = 1;
        }
    }
    if (c->slot[1].motion != NULL) {
        t = ChrMotionStepFrame(c->slot[1].motion, c->slot[1].frame, dt);
        c->slot[1].frame = t;
        if (!(c->slot[1].motion->playFlags & 1) && (f32)(c->slot[1].motion->frameCount - 1) <= t) {
            ret |= 2;
        }
    }
    return ret;
}
