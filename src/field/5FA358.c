/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"

typedef struct { f32 x, y, z, w; } V4;
typedef struct { V4 v[2]; f32 x20; s32 x24; } Cam;

typedef struct {
    u8 pad0[0x10];
    f32 x10;
    f32 y;
    f32 x18;
} Model;

extern Cam chrLadderWork;
extern f32 chrLadderAngle;
extern s32 chrLadderFromTop;
extern u8 camCtlSubType;

void VuSinCosDeg(f32 *, f32 *, f32);

#define VU0_ZERO(p) __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(p) : "memory")

void ChrLadderUpdate(ChrObj *c, s32 arg1) {
    f32 s;
    f32 co;
    V4 v;
    f32 ref;
    f32 y1;
    f32 k;
    f32 d;
    s32 i;
    double d0;
    double d1;
    f32 k2;
    f32 y0;
    f32 t;
    s32 fl;
    f32 x, y, z;

    switch (c->ladderState) {
    case 0:
        {
        fl = c->flags;
        fl |= 0x40;
        fl &= ~0xC04;
        c->flags = fl;
        }
        VU0_ZERO(c->vel);
        ChrMotSet(c, 0);
        ref = ((Model *)c->model)->y;
        v.x = chrLadderWork.v[0].x;
        y0 = chrLadderWork.v[0].y;
        v.y = y0;
        v.z = chrLadderWork.v[0].z;
        v.w = 0.0f;
        y1 = chrLadderWork.v[1].y;
        d0 = y0 - ref;
        if (d0 < 0.0) {
            d0 = 0.0 - d0;
        }
        d1 = y1 - ref;
        if (d1 < 0.0) {
            d1 = 0.0 - d1;
        }
        if (d0 < d1) {
            chrLadderWork.x24 = 1;
        } else {
            chrLadderWork.x24 = 0;
        }
        VuSinCosDeg(&s, &co, chrLadderWork.x20);
        i = chrLadderWork.x24 == 1;
        {
        V4 *p = &chrLadderWork.v[i];
        x = p->x;
        y = p->y;
        z = p->z;
        s = -s;
        co = -co;
        v.x = x;
        v.y = y;
        v.z = z;
        }
        CamSetTransitPoint(&v, chrLadderWork.x20);
        camCtl.aiType = 3;
        camCtl.subType = 0;
        c->ladderState++;
        break;
    case 1:
        if (c->unk28 == 0) {
            c->unk28 = 0;
            c->ladderState++;
        } else {
            c->unk28--;
        }
        break;
    case 2:
        if (c->unk28 == 0) {
            VuSinCosDeg(&s, &co, chrLadderWork.x20);
            s = -s;
            co = -co;
            if (chrLadderWork.x24 == 0) {
                k2 = 40.0f;
                i = 1;
            } else {
                k2 = -40.0f;
                i = 0;
            }
            {
            V4 *p = &chrLadderWork.v[i];
            v.x = p->x + k2 * s;
            v.y = p->y;
            v.z = p->z + k2 * co;
            }
            ChrMotSet(c, 1);
            FldObjMoveTo(c, &v, 0, 10.0f);
        }
        if (c->moveCmds == 0) {
            c->unk28 = 0;
            c->ladderState++;
            VU0_ZERO(c->vel);
            break;
        }
        c->unk28++;
        break;
    case 3:
        c->unk28 = 0;
        c->flags &= ~0x80;
        c->heading = -((chrLadderAngle - 180.0f) * 182.04445f);
        c->ladderState++;
    case 4:
        camCtlSubType = 1;
        if (c->unk28 == 0) {
            if (c->flags & 8) {
                c->flags ^= 8;
            }
            if (chrLadderWork.x24 == 0) {
                VuSinCosDeg(&s, &co, chrLadderWork.x20);
                ((Model *)c->model)->x10 += s * 80.0f;
                ((Model *)c->model)->x18 += co * 80.0f;
                ((Model *)c->model)->y -= 180.0f;
                ChrMotSetImmediateOnce(c, 12);
            } else {
                ChrMotSetOnce(c, 9);
            }
        }
        if (ChrMotIsEnd(c->model) == 1) {
            c->unk28 = 0;
            c->ladderState++;
            break;
        }
        c->unk28++;
        break;
    case 5:
        camCtlSubType = 2;
        if (c->unk28 == 0) {
            c->unk14 = 10;
            if (chrLadderWork.x24 == 0) {
                ChrMotSet(c, 13);
            } else {
                ChrMotSet(c, 10);
            }
        }
        {
        Model *m = (Model *)c->model;
        ref = m->y;
        y0 = chrLadderWork.v[0].y;
        y1 = chrLadderWork.v[1].y;
        if (chrLadderWork.x24 == 0) {
            if (ref == y0) {
                c->vel[1] = 0.0f;
                c->unk28 = 0;
                c->ladderState++;
                m->y = y0;
                break;
            }
            d = y0 - ref;
            if (d < -5.0f) {
                d = -5.0f;
            }
            c->vel[1] = d;
            c->speed = -d;
        } else {
            if (chrLadderWork.x24 == 1) {
                d = 170.0f;
            } else {
                d = 0.0f;
            }
            t = y1 - d;
            if (t <= ref) {
                c->vel[1] = 0.0f;
                c->unk28 = 0;
                c->ladderState++;
                break;
            }
            d = t - ref;
            if (d > 3.0f) {
                d = 3.0f;
            }
            c->vel[1] = 0.0f;
            m->y += d;
            c->speed = d;
        }
        }
        c->unk28++;
        break;
    case 6:
        camCtlSubType = 3;
        if (c->unk28 == 0) {
            if (chrLadderFromTop == 0) {
                ChrMotSetOnce(c, 14);
            } else {
                ChrMotSetOnce(c, 11);
            }
        }
        if (ChrMotIsEnd(c->model) == 1) {
            c->unk28 = 0;
            c->ladderState++;
            break;
        }
        c->unk28++;
        break;
    case 7:
        camCtl.subType = 4;
        if (c->unk28 == 0) {
            k = 12.0f;
            if (chrLadderWork.x24 == 0) {
                k = 11.0f;
            }
            if (EvtGetMapNo() == 0x8010) {
                k *= 0.5f;
            }
            if (chrLadderWork.x24 == 1) {
                ((Model *)c->model)->y += 180.0f;
                ChrMotSetImmediate(c, 1);
            } else {
                ChrMotSet(c, 1);
            }
            c->flags |= 8;
            if (arg1 == 0) {
                if (chrLadderWork.x24 == 1) {
                    chrLadderWork.x20 += 180.0f;
                }
                VuSinCosDeg(&s, &co, chrLadderWork.x20);
                c->flags |= 0x484;
                v.x = s * k;
                v.y = 0.0f;
                v.z = co * k;
                FldObjMoveVelocity(c, &v, 0, 20.0f);
            } else {
                c->flags |= 0x484;
            }
            c->htaArea = 0;
            c->unk28++;
            break;
        }
        if (c->unk28 == 8 && arg1 == 0) {
            c->flags |= 0x800;
        }
        if (c->moveCmds == 0) {
            if (arg1 == 0) {
                c->flags |= 0x800;
            }
            VU0_ZERO(c->vel);
            c->ctrlMode = 0;
            c->unk28 = 0;
            c->ladderState = 0;
            {
            fl = c->flags;
            fl &= ~0x40;
            c->flags = fl;
            }
            FldInputLockSet(0);
            camCtl.aiType = camCtl.prevType;
            break;
        }
        c->unk28++;
        break;
    }
}
