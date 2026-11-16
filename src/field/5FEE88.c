/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"


void ChrTurnStart(ChrObj *o, ChrObj *tgt, u8 mode, s32 ang, f32 speed, u8 flag) {
    f32 a;
    f32 b;
    f32 d;
    f32 k;
    f32 z;

    k = 0.005493164f;
    a = o->heading * k;
    if (tgt == NULL) {
        b = ang;
    } else {
        b = FldCalcYaw(tgt->model->pos, o->model->pos) * k;
    }
    o->look[1].unk18 = speed;
    o->look[1].unk0 = mode;
    o->look[1].unk1C = 0.0f;
    if (a >= 360.0f) {
        a -= 360.0f;
    }
    z = o->look[1].unk1C;
    if (a < z) {
        a += 360.0f;
    }
    if (b >= 360.0f) {
        b -= 360.0f;
    }
    if (b < z) {
        b += 360.0f;
    }
    d = ChrAngleDiffDeg(a, b);
    b = a + d;
    if (d > -361.0f && d < -359.0f) {
        return;
    }
    if (d < 1.0f && d > -1.0f) {
        return;
    }
    switch (mode) {
    case 0:
        if (z < d) {
            if (d > 110.0f) {
                o->mot = 0x12;
            } else {
                o->mot = 7;
            }
        } else {
            if (d < -110.0f) {
                o->mot = 0x13;
            } else {
                o->mot = 0x11;
            }
        }
        break;
    case 1:
        if (z < d) {
            o->mot = 7;
        } else {
            o->mot = 0x11;
        }
        break;
    case 2:
        if (z < d) {
            o->mot = 0x12;
        } else {
            o->mot = 0x13;
        }
        break;
    case 3:
        o->mot = 1;
        break;
    }
    o->motSpeed = 1.0f;
    if (ChrMotHas(o, o->mot) == 0) {
        mode = 3;
        o->mot = 1;
    }
    if (mode != 3) {
        ChrMotSetImmediateOnce(o, o->mot);
    }
    if (flag == 0) {
        if (mode != 3) {
            o->motSpeed = ChrMotGetLastFrame(o->model) / o->look[1].unk18;
        }
    } else {
        o->look[1].unk18 = ChrMotGetLastFrame(o->model);
    }
    o->look[1].unkC = b;
    o->look[1].unk8 = a;
    switch ((s8)o->mot) {
    case 7:
        o->look[1].unkC -= 90.0f;
        break;
    case 18:
        o->look[1].unkC -= 180.0f;
        break;
    case 17:
        o->look[1].unkC += 90.0f;
        break;
    case 19:
        o->look[1].unkC += 180.0f;
        break;
    case 1:
        o->motSpeed = __builtin_fabsf(d) / 180.0f;
        ChrMotSetOnce(o, o->mot);
        break;
    }
    o->actFlags |= 0x800;
    if (!(o->flags & 0x40)) {
        o->flags |= 0x40;
    }
    o->look[1].unk4 = o->mot;
}
