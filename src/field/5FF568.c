/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"

void VuVecLerp(FVECTOR *, FVECTOR *, FVECTOR *, f32);
void VuBezier3(FVECTOR *, FVECTOR *, f32);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void ChrTurnUpdate(ChrObj *o) {
    FVECTOR a;
    FVECTOR c;
    FVECTOR out;
    FVECTOR pts[4];
    GphPart *b;
    f32 one;

    if (o->actFlags & 0x800) {
        one = 1.0f;
        o->look[1].unk1C += one / o->look[1].unk18;
        if (one < o->look[1].unk1C) {
            o->look[1].unk1C = one;
        }
        a[0] = o->look[1].unk8;
        a[1] = 0.0f;
        a[2] = 0.0f;
        a[3] = 0.0f;
        c[0] = o->look[1].unkC;
        c[1] = 0.0f;
        c[2] = 0.0f;
        c[3] = 0.0f;
        VuVecLerp(&out, &a, &c, o->look[1].unk1C);
        o->heading = out[0] * 182.04445f;
        if (o->look[1].unk1C == one) {
            o->motSpeed = one;
            switch (o->look[1].unk4) {
            case 7:
                o->heading = o->heading + 16384.0f;
                break;
            case 0x12:
                o->heading = o->heading + 32768.0f;
                break;
            case 0x11:
                o->heading = o->heading - 16384.0f;
                break;
            case 0x13:
                o->heading = o->heading - 32768.0f;
                break;
            }
            o->moveDir = o->heading;
            ChrMotStateReset(o->model);
            o->actFlags ^= 0x800;
            if (o->flags & 0x40) {
                o->flags ^= 0x40;
            }
        }
    }
    if (o->actFlags & 0x1000) {
        if (o->look[0].unk0 == 2) {
            b = ChrModelFindPart(o->model, 0x280);
            if (o->look[0].unk1C == 0.0f) {
                if (o->look[0].unk1 != 0) {
                    o->look[0].unk1--;
                }
                o->look[0].unk8 = b->rot[1] * 0.005493164f;
                if (o->look[0].unk1 == 0) {
                    o->look[0].unkC = 0.0f;
                } else {
                    o->look[0].unkC = -o->look[0].unkC;
                }
                o->look[0].unk2++;
            }
            if (o->look[0].unk2 == 1 || o->look[0].unk1 == 0) {
                o->look[0].unk1C += 1.0f / o->look[0].unk18 * 2.0f;
            } else {
                o->look[0].unk1C += 1.0f / o->look[0].unk18;
            }
            if (o->look[0].unk1C > 1.0f) {
                o->look[0].unk1C = 1.0f;
            }
            one = 1.0f;
            a[0] = o->look[0].unk10;
            a[1] = o->look[0].unk8;
            a[2] = 0.0f;
            a[3] = 0.0f;
            c[0] = o->look[0].unk14;
            c[1] = o->look[0].unk10 + o->look[0].unkC;
            c[2] = 0.0f;
            c[3] = 0.0f;
            if (c[1] >= 360.0f) {
                c[1] -= 360.0f;
            }
            if (c[1] < 0.0f) {
                c[1] += 360.0f;
            }
            c[1] = a[1] + ChrAngleDiffDeg(a[1], c[1]);
            copyq(&pts[0], &a);
            copyq(&pts[1], &c);
            copyq(&pts[2], &c);
            copyq(&pts[3], &c);
            VuBezier3(&out, pts, o->look[0].unk1C);
            b->rot[1] = out[1] * 182.04445f;
            if (o->look[0].unk1C == one) {
                if (o->look[0].unk1 == 0) {
                    o->actFlags ^= 0x1000;
                } else {
                    o->look[0].unk1C = 0.0f;
                }
            }
        } else {
            b = ChrModelFindPart(o->model, 0x280);
            o->look[0].unk1C += 1.0f / o->look[0].unk18;
            if (1.0f < o->look[0].unk1C) {
                o->look[0].unk1C = 1.0f;
            }
            a[0] = o->look[0].unk10;
            a[1] = o->look[0].unk8;
            a[2] = 0.0f;
            a[3] = 0.0f;
            c[0] = o->look[0].unk14;
            c[1] = o->look[0].unkC;
            c[2] = 0.0f;
            c[3] = 0.0f;
            copyq(&pts[0], &a);
            copyq(&pts[1], &c);
            copyq(&pts[2], &c);
            copyq(&pts[3], &c);
            VuBezier3(&out, pts, o->look[0].unk1C);
            b->rot[0] = out[0] * 182.04445f;
            if (o->look[0].unk0 == 1) {
                b->rot[2] = out[1] * 182.04445f;
            } else {
                b->rot[1] = out[1] * 182.04445f;
            }
            if (o->look[0].unk1C == 1.0f) {
                o->actFlags ^= 0x1000;
            }
        }
    }
}
