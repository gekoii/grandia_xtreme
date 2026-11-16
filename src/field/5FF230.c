/* as: ee-gcc2.96-hazard */
/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"


void ChrHeadTurnStart(ChrObj *o, ChrObj *tgt, u8 mode, s32 roll, f32 speed, s32 yaw0) {
    GphPart *b;
    f32 k;
    f32 p;
    f32 y;
    f32 p2;
    f32 y2;
    f32 d;

    b = ChrModelFindPart(o->model, 0x280);
    if (b == NULL) {
        return;
    }
    k = 0.005493164f;
    y = b->rot[0] * k;
    if (mode == 1) {
        p = b->rot[2] * k;
    } else {
        p = b->rot[1] * k;
    }
    y2 = y;
    if (mode == 2) {
        p2 = -(f32)yaw0;
    } else {
        if (tgt == NULL) {
            p2 = yaw0;
        } else {
            p2 = ((FldCalcYaw(tgt->model->pos, o->model->pos) - o->heading) & 0xFFFF) * 0.005493164f;
        }
        y2 = -(f32)roll;
    }
    if (p >= 360.0f) {
        p -= 360.0f;
    }
    if (p < 0.0f) {
        p += 360.0f;
    }
    if (y >= 360.0f) {
        y -= 360.0f;
    }
    if (y < 0.0f) {
        y += 360.0f;
    }
    if (mode != 2) {
        if (p2 >= 360.0f) {
            p2 -= 360.0f;
        }
        if (p2 < 0.0f) {
            p2 += 360.0f;
        }
        if (y2 >= 360.0f) {
            y2 -= 360.0f;
        }
        if (y2 < 0.0f) {
            y2 += 360.0f;
        }
        d = ChrAngleDiffDeg(p, p2);
        p2 = p + d;
        if ((d > -361.0f && d < -359.0f) || (d < 1.0f && d > -1.0f)) {
            p2 = p;
        }
        d = ChrAngleDiffDeg(y, y2);
        y2 = y + d;
        if ((d > -361.0f && d < -359.0f) || (d < 1.0f && d > -1.0f)) {
            y2 = y;
        }
    }
    o->look[0].unk0 = mode;
    o->look[0].unk1 = roll + 3;
    o->look[0].unk1C = 0.0f;
    o->look[0].unk2 = 0;
    o->look[0].unk3 = 0;
    if (mode == 2) {
        o->look[0].unk18 = speed / o->look[0].unk1;
    } else {
        o->look[0].unk18 = speed;
    }
    o->look[0].unkC = p2;
    o->look[0].unk8 = p;
    if (mode == 2) {
        o->look[0].unk14 = p2;
        o->look[0].unk10 = p;
    } else {
        o->look[0].unk14 = y2;
        o->look[0].unk10 = y;
    }
    o->actFlags |= 0x1000;
}
