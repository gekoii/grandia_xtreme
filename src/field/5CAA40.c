/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "pad.h"
#include "save.h"
#include "field/n15.h"

f32 CamPadTurnInput(Camera *unused, Pad *pad, s32 digital, f32 scale) {
    f32 l;
    f32 r;
    f32 la;
    f32 lb;
    f32 ret;
    f32 t;

    switch (sysOptCamReverse) {
    case 0:
        if (digital) {
            l = 0.0f;
            if (pad->held & 4) {
                l = 128.0f;
            }
            r = 0.0f;
            if (pad->held & 8) {
                r = 128.0f;
            }
        } else {
            l = pad->pressure[8];
            r = pad->pressure[9];
        }
        break;
    case 1:
        if (digital) {
            l = 0.0f;
            if (pad->held & 8) {
                l = 128.0f;
            }
            r = 0.0f;
            if (pad->held & 4) {
                r = 128.0f;
            }
        } else {
            l = pad->pressure[9];
            r = pad->pressure[8];
        }
        break;
    default:
        l = r = 0.0f;
        break;
    }

    /* do-while(0) wraps: allocation weight for l and r (unwrapped, 63-129 words off) */
    do {
        if (l <= 10.0f) {
            l = 0.0f;
        } else if (l <= 224.0f) {
            l = 116.0f;
        } else {
            l = 196.0f;
        }
    } while (0);
    do {
        if (r <= 10.0f) {
            r = 0.0f;
        } else if (r <= 224.0f) {
            r = 116.0f;
        } else {
            r = 196.0f;
        }
    } while (0);

    la = camPadTurn.l / 255.0f;
    ret = 0.0f;
    lb = camPadTurn.r / 255.0f;
    if (l < camPadTurn.l * scale) {
        camPadTurn.l = camPadTurn.l * scale;
        ret += la * la * camPadTurnSpeed;
    } else if (l > 20.0f) {
        camPadTurn.l = l;
        ret += la * la * camPadTurnSpeed;
    }
    if (camPadTurn.l < 10.0f) {
        camPadTurn.l = 0.0f;
    }
    t = camPadTurn.r * scale;
    if (r < t) {
        camPadTurn.r = t;
        ret -= lb * lb * camPadTurnSpeed;
        if (t < 10.0f) {
            camPadTurn.r = 0.0f;
        }
    } else if (r > 20.0f) {
        camPadTurn.r = r;
        ret -= lb * lb * camPadTurnSpeed;
    }
    if (camPadTurn.r < 10.0f) {
        camPadTurn.r = 0.0f;
    }
    if (__builtin_fabsf(ret) < 0.001f) {
        ret = 0.0f;
    }
    return ret;
}
