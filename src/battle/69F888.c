/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

f32 VuSinAngle(s32);

void BtlCmdMenuRingUpdate(BtlCmdMenu *arg0) {
    BtlCmdRingCam *c = &arg0->ringCam;
    f32 f;
    f32 v;
    f32 step;
    f32 tgt;
    f32 st2;

    if (arg0->ringCam.t < 16) {
        f = 1.0f - VuSinAngle(arg0->ringCam.t << 10);
        c->unk18 = f * 65536.0f;
        c->unk1C = f * 131072.0f;
        c->unk20 = f * 285.0f;
        c->unk24 = f * -140.0f;
        c->unk28 = arg0->ringCam.t * 0.125f;
        if (c->unk28 > 1.0f) {
            c->unk28 = 1.0f;
        }
    } else if (arg0->ringCam.t == 16) {
        c->unk1C = 0;
        c->unk18 = 0;
        arg0->unkB0 = 0;
        c->unk20 = c->unk24 = 0.0f;
        c->unk28 = 1.0f;
        BtlChrPartsSetHidden(BtlChrFindPart(btlCmdMenuRingInst, 11), 1);
    }
    v = c->vel;
    if (v != 0.0f) {
        
        step = ABS(v) * 0.375f + 512.0f;
        if (step > 2048.0f) {
            step = 2048.0f;
        }
        if (c->vel > 0.0f) {
            c->vel -= step;
            if (c->vel <= 0.0f) {
                step = 0.0f;
            }
        } else {
            c->vel += step;
            if (c->vel >= 0.0f) {
                step = 0.0f;
            } else {
                step = -step;
            }
        }
        if (step == 0.0f) {
            c->angle = arg0->unk9 * 10922.667f;
            c->vel = 0.0f;
        } else {
            c->angle += step;
        }
    }
    if (c->dir != 0) {
        
        if (arg0->unk8 != 0) {
            tgt = 45.0f;
            if (arg0->unk8 > 0) {
                tgt = -55.0f;
            }
        } else {
            tgt = 0.0f;
        }
        st2 = ABS(c->tilt - tgt) * 0.26666668f + 2.0f;
        if (st2 > 10.0f) {
            st2 = 10.0f;
        }
        if (c->dir > 0) {
            c->tilt = c->tilt - st2;
            if (c->tilt <= tgt) {
                c->tilt = tgt;
                c->dir = 0;
            }
        } else {
            c->tilt = c->tilt + st2;
            if (c->tilt >= tgt) {
                c->tilt = tgt;
                c->dir = 0;
            }
        }
    }
    c->t++;
    c->inputTimer++;
}
