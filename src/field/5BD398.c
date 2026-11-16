/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccFogFadeForwardStart(s16 *arg0) {
    OccEntry *obj = (OccEntry *)arg0;
    OccFogFade *a = OCC_PARAMS(obj, OccFogFade);

    switch (a->mode) {
    case 0:
        if (a->to[0] >= 128.0f) {
            a->to[0] = 128.0f;
        }
        GphFadeStart(fldFades, 0, OCC_PARAMS(obj, OccFogFade)->rgb[2], OCC_PARAMS(obj, OccFogFade)->rgb[1],
                    OCC_PARAMS(obj, OccFogFade)->rgb[0], a->to[0], a->time);
        break;
    case 1:
        a->colorFrom[0] = camField[0].fogColor[0];
        a->colorFrom[1] = camField[0].fogColor[1];
        a->colorFrom[2] = camField[0].fogColor[2];
        a->from[0] = camField[0].fogMax;
        a->from[1] = camField[0].fogMin;
        a->from[2] = camField[0].fogDist;
        a->from[3] = 0.0f;
        break;
    }
    obj->timer = a->time;
    a->colorDelta[0] = (f32)a->color[0] - (f32)a->colorFrom[0];
    a->colorDelta[1] = (f32)a->color[1] - (f32)a->colorFrom[1];
    a->colorDelta[2] = (f32)a->color[2] - (f32)a->colorFrom[2];
    a->count = 0;
    a->delta[0] = a->to[0] - a->from[0];
    a->delta[1] = a->to[1] - a->from[1];
    a->delta[2] = a->to[2] - a->from[2];
    a->delta[3] = a->to[3] - a->from[3];
}
