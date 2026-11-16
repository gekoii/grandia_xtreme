/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "ptcl.h"


void PtclModelAlphaFade(void *obj, PtclGenDef *f, PtclModelPart *fr) {
    f32 v = 128.0f;
    f32 t;
    s32 n = fr->time - 1;

    if (n < f->colorC1) {
        if (f->colorCurve[0] != 0) {
            t = PtclCurveEval(f->colorCurve[0], (f32)n / (f32)f->colorC1);
            v = ((f32)f->colorKeys[7] - (f32)f->colorKeys[3]) * t + (f32)f->colorKeys[3];
        }
    } else if (n < f->colorC2) {
        if (f->colorCurve[1] != 0) {
            t = PtclCurveEval(f->colorCurve[1], (f32)(n - f->colorC1) / (f32)(f->colorC2 - f->colorC1));
            v = ((f32)f->colorKeys[11] - (f32)f->colorKeys[7]) * t + (f32)f->colorKeys[7];
        }
    } else {
        if (f->colorCurve[2] != 0) {
            t = PtclCurveEval(f->colorCurve[2], (f32)(n - f->colorC2) / (f32)(fr->life - f->colorC2));
            v = ((f32)f->colorKeys[15] - (f32)f->colorKeys[11]) * t + (f32)f->colorKeys[11];
        }
    }
    ChrModelSetAlpha(obj, v);
}
