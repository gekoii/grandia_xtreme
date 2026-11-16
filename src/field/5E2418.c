/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/state.h"
#include "protos/field.h"

f32 ChrTenantTurnStep(ChrObj *o, f32 speed) {
    f32 rot[4];
    f32 cur;
    f32 target;
    f32 d;
    f32 s;

    cur = (f32)o->heading * 0.0054931640625f;
    if (speed > 0.01f) {
        FldDirToRot(o->vel, rot);
        o->moveDir = rot[1] * 10430.378f;
    } else {
        rot[1] = (f32)(o->moveDir & 0xFFFF) * 9.58738019e-05f;
    }
    if ((o->moveDir & 0xFFFF) == (o->heading & 0xFFFF)) {
        return 0.0f;
    }
    target = rot[1] * 57.2957764f;
    if (cur >= 360.0f) {
        cur -= 360.0f;
    }
    if (cur <= 0.0f) {
        cur += 360.0f;
    }
    if (target >= 360.0f) {
        target -= 360.0f;
    }
    if (target <= 0.0f) {
        target += 360.0f;
    }
    d = ChrAngleDiffDeg(cur, target);
    if (d > 0.0f) {
        if (d > 100.0f) {
            s = 16.0f;
        } else if (d > 45.0f) {
            s = 8.0f;
        } else if (d > 10.0f) {
            s = 4.0f;
        } else if (d > 2.0f) {
            s = 2.0f;
        } else {
            s = d;
        }
    } else {
        if (d < -100.0f) {
            s = -16.0f;
        } else if (d < -45.0f) {
            s = -8.0f;
        } else if (d < -10.0f) {
            s = -4.0f;
        } else if (d < -2.0f) {
            s = -2.0f;
        } else {
            s = d;
        }
    }
    if (s == d) {
        o->heading = (u16)o->moveDir;
        return 0.0f;
    }
    o->heading = o->heading + s * 182.044449f;
    return s;
}
