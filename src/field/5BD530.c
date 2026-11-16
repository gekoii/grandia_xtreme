/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"

void OccFogFadeForward(u8 *arg0) {
    OccFogFade *a = OCC_PARAMS(arg0, OccFogFade);
    VuVec4 v;
    VuVec4 w;
    s32 r, g, b;
    f32 t;

    a->count++;
    t = (f32)a->count / (f32)a->time;
    r = a->colorFrom[0] + (s32)(a->colorDelta[0] * t);
    g = a->colorFrom[1] + (s32)(a->colorDelta[1] * t);
    b = a->colorFrom[2] + (s32)(a->colorDelta[2] * t);
    v.x = a->from[0] + a->delta[0] * t;
    v.y = a->from[1] + a->delta[1] * t;
    w.x = a->from[2] + a->delta[2] * t;
    switch (a->mode) {
    case 0:
        break;
    case 1:
        camField[0].fogDist = w.x;
        camField[0].fogMax = v.x;
        camField[0].fogMin = v.y;
        camField[0].fogColor[0] = (f32)r;
        camField[0].fogColor[1] = (f32)g;
        camField[0].fogColor[2] = (f32)b;
        break;
    }
}
