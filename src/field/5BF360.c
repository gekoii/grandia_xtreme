#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"

/* real order is (Effect *, u8, s32, u32, f32); floats-first fixes the EABI arg-load schedule */

void OccScreenFxForwardStart(u8 *arg0) {
    OccScreenFx *a = OCC_PARAMS(arg0, OccScreenFx);
    u8 t;

    if (a->effect < 8) {
        t = a->effect;
        if (t == 1) {
            ((void (*)(void *, f32, u8, s32, u32))GphScreenFxSet)(fldScreenFx, 0.0f, 7, a->noHold ^ 1, a->time);
        } else {
            ((void (*)(void *, f32, u8, s32, u32))GphScreenFxSet)(fldScreenFx, 0.0f, t, a->noHold ^ 1, a->time);
        }
    } else {
        GphShatterStart(0);
        GphFadeSetColor(fldFades, 0.0f, 0.0f, 0.0f, 128.0f);
    }
    if (a->effect == 4) {
        SysScreenCaptureRequest();
    }
}
