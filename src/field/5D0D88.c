/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

f32 CamScrInterpStep(CamScrInterp *arg0) {
    f32 t;

    arg0->count++;
    if (arg0->count >= arg0->total) {
        arg0->active = 0;
        return arg0->end;
    }
    arg0->timer--;
    if (arg0->timer <= 0) {
        t = (f32)arg0->count / (f32)arg0->total;
        arg0->timer = arg0->period;
        switch (arg0->mode) {
        case 0:
            arg0->cur = arg0->delta * t;
            break;
        case 1:
            t = (-cosf(t * 3.141592f) + 1.0f) * 0.5f;
            arg0->cur = arg0->delta * t;
            break;
        case 2:
            t = -cosf(t * 1.570796f) + 1.0f;
            arg0->cur = arg0->delta * t;
            break;
        case 3:
            arg0->cur = arg0->delta * sinf(t * 1.570796f);
            break;
        }
    }
    return arg0->cur + arg0->start;
}
