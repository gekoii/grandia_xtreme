/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "ptcl.h"
#include "protos/main.h"

f32 PtclTrackEval(PtclStarterSlot *a, PtclTrack *k, PtclGenDef **from, PtclGenDef **to) {
    s32 time = a->time;
    PtclCtrl *m = a->ctrl;

    if (time < k->time[1]) {
        *from = &m->keys[0];
        *to = &m->keys[1];
        return PtclCurveEval(k->curve[0], (f32)(time - k->time[0]) / (f32)(k->time[1] - k->time[0]));
    }
    if (time < k->time[2]) {
        *from = &m->keys[1];
        *to = &m->keys[2];
        return PtclCurveEval(k->curve[1], (f32)(time - k->time[1]) / (f32)(k->time[2] - k->time[1]));
    }
    if (time < k->time[3]) {
        *from = &m->keys[2];
        *to = &m->keys[3];
        return PtclCurveEval(k->curve[2], (f32)(time - k->time[2]) / (f32)(k->time[3] - k->time[2]));
    }
    *from = *to = &m->keys[k->lastKey];
    return 0.0f;
}
