/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"
#include "pad.h"

void CamShakeSetVec(VuVec4 *, VuVec4 *, s32);

void OccQuakeForwardStart(OccEntry *obj) {
    OccQuake *c = OCC_PARAMS(obj, OccQuake);
    f32 f = c->strength;
    u8 lvl, on;
    VuVec4 a;
    VuVec4 b;

    if (f > 80.0f) {
        lvl = 0xFF;
        on = 1;
    } else if (f > 40.0f) {
        lvl = 0x80;
        on = 1;
    } else {
        lvl = 0x40;
        on = 0;
    }
    switch (c->mode) {
    case 0:
        PadVibrate(padPort0, lvl, on, c->time);
        CamShakeStop(c->time);
        break;
    case 1:
        CamShakeStartQuake(c->strength);
        PadVibrate(padPort0, lvl, on, c->strength);
        GphScreenFxSet(&fldScreenFx2, 1, 1, 10, 60.0f);
        break;
    case 2:
        CamShakeStartPreset(c->strength);
        PadVibrate(padPort0, lvl, on, c->strength);
        GphScreenFxSet(&fldScreenFx2, 1, 1, 10, 60.0f);
        break;
    case 3:
        CamShakeSetVert(0, f * 10.0f, f * 20.0f);
        break;
    case 4:
        a.x = -40.0f;
        a.y = 20.0f;
        b.x = 80.0f;
        b.y = 40.0f;
        a.z = 0.0f;
        a.w = 0.0f;
        b.z = 0.0f;
        b.w = 0.0f;
        CamShakeSetVec(&a, &b, 0);
        camField[0].shakeCycle[0] = 4.0f;
        camField[0].shakeCycle[1] = 4.0f;
        camField[0].shakeCycle[2] = 0.0f;
        camField[0].shakeCycle[3] = 0.0f;
        break;
    case 5:
        CamShakeSetVert(0, f * 10.0f, f * 20.0f);
        camField[0].shakeCycle[0] = 4.0f;
        camField[0].shakeCycle[1] = 4.0f;
        camField[0].shakeCycle[2] = 0.0f;
        camField[0].shakeCycle[3] = 0.0f;
        CamShakeStop(c->time);
        PadVibrate(padPort0, lvl, on, c->time);
        GphScreenFxSet(&fldScreenFx2, 1, 1, c->time, 60.0f);
        break;
    case 6:
        a.x = -40.0f;
        a.y = 20.0f;
        b.x = 80.0f;
        b.y = 40.0f;
        a.z = 0.0f;
        a.w = 0.0f;
        b.z = 0.0f;
        b.w = 0.0f;
        CamShakeSetVec(&a, &b, 0);
        camField[0].shakeCycle[0] = 4.0f;
        camField[0].shakeCycle[1] = 4.0f;
        camField[0].shakeCycle[2] = 0.0f;
        camField[0].shakeCycle[3] = 0.0f;
        CamShakeStop(c->time);
        PadVibrate(padPort0, lvl, on, c->time);
        GphScreenFxSet(&fldScreenFx2, 1, 1, c->time, 60.0f);
        break;
    }
}
