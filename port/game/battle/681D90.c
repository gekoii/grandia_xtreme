/* Port-only C for FxHomingUpdate (0x00681D90), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/battle.h"
#include "battle/n28.h"

void VuMtxSetRotX(FMATRIX, s32);
void VuMtxSetRotY(FMATRIX, s32);
void VuMtxSetRotZ(FMATRIX, s32);
void VuVecNormalize(f32 *, f32 *);

/* v = m * v (xyzw), the vmulax/vmadday/vmaddaz/vmaddw sequence */
static void MulVec(FMATRIX m, f32 *v)
{
    f32 r[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        r[i] = m[0][i] * v[0] + m[1][i] * v[1] + m[2][i] * v[2] + m[3][i] * v[3];
    }
    for (i = 0; i < 4; i++) {
        v[i] = r[i];
    }
}

/* Steers e toward its target: the target and the direction are turned into a
 * frame where the target lies along +z, the direction's pitch there is
 * eased by a turn rate (unk10) that grows and shrinks by turnMin, the
 * speed rises while the target is within turnMax of straight ahead and falls
 * otherwise, then the position moves on. Within unk18 of the target (while
 * aimed at it) the entry's callback (state, called with unk4 and e) runs and
 * the entry is freed. */
void FxHomingUpdate(FxHomingPool *p, FxHoming *e)
{
    FMATRIX m;
    FVECTOR d;
    FVECTOR v;
    f32 *dir = e->dir;
    f32 *pos;
    s32 ay;
    s32 ax;
    s32 az;
    s32 a;
    s32 half;
    s32 rate;
    s32 diff;
    s32 i;

    for (i = 0; i < 4; i++) { /* vsub.xyz: w stays the target's */
        v[i] = (*e->target)[i];
    }
    for (i = 0; i < 3; i++) {
        v[i] -= (*e->pos)[i];
    }

    ay = BtlAtan2Angle(v[0], v[2]);
    VuMtxSetRotY(m, ay);
    MulVec(m, v);
    for (i = 0; i < 4; i++) {
        d[i] = dir[i];
    }
    MulVec(m, d);

    ax = BtlAtan2Angle(v[1], v[2]);
    VuMtxSetRotX(m, ax);
    MulVec(m, v);
    MulVec(m, d);

    az = BtlAtan2Angle(d[0], d[1]) + (e->unk14 << 15);
    VuMtxSetRotZ(m, az);
    MulVec(m, v);
    MulVec(m, d);

    a = BtlAtan2Angle(d[1], d[2]);

    if ((u16)(a + p->turnMax) < p->turnMax * 2) {
        e->speed += p->accel;
        if (p->speedMax < e->speed) {
            e->speed = p->speedMax;
        }
    } else {
        e->speed -= p->accel;
        if (e->speed < p->speedMin) {
            e->speed = p->speedMin;
        }
    }

    half = p->turnStep / 2;
    rate = e->unk10;
    if (!(half < a && a < 0x10000 - half) && (rate < half || 0x10000 - half < rate)) {
        /* nearly aimed and turning slowly: snap */
        e->unk10 = 0;
        VuMtxSetRotX(m, a);
    } else {
        if (a < p->turnMax / 2) {
            if (0x7FFF < rate) {
                e->unk10 = rate + (p->turnMin << 2);
            } else {
                e->unk10 = (s32)((f32)rate + (f32)p->turnMin * ((f32)a / (f32)(p->turnMax / 4) - 0.0f));
            }
        } else if (0x10000 - p->turnMax / 2 < a) {
            if (0x7FFF < rate) {
                e->unk10 = (s32)((f32)rate - (f32)p->turnMin * ((f32)(0x10000 - a) / (f32)(p->turnMax / 4) - 0.0f));
            } else {
                e->unk10 = rate - (p->turnMin << 2);
            }
        } else if (0x7FFF < a) {
            e->unk10 = rate - p->turnMin;
        } else {
            e->unk10 = rate + p->turnMin;
        }

        rate = (u16)e->unk10; /* lhu: the low half */
        e->unk10 = rate;
        if (rate <= 0x7FFF) {
            if (p->turnStep < rate) {
                e->unk10 = p->turnStep;
            }
        } else {
            if (rate < 0x10000 - p->turnStep) {
                e->unk10 = 0x10000 - p->turnStep;
            }
        }
        VuMtxSetRotX(m, e->unk10);

        diff = (u16)(a - e->unk10);
        if (a <= 0x8000) {
            if (0x8000 < diff) {
                e->unk14 ^= 1;
            }
        } else {
            if (diff <= 0x8000) {
                e->unk14 ^= 1;
            }
        }
    }

    /* back out of the target frame */
    MulVec(m, d);
    VuMtxSetRotZ(m, -az);
    MulVec(m, d);
    VuMtxSetRotX(m, -ax);
    MulVec(m, d);
    VuMtxSetRotY(m, -ay);
    MulVec(m, d);
    for (i = 0; i < 4; i++) {
        dir[i] = d[i];
    }
    VuVecNormalize(dir, dir);
    for (i = 0; i < 3; i++) {
        dir[i] *= e->speed;
    }
    pos = *e->pos;
    for (i = 0; i < 3; i++) {
        pos[i] += dir[i];
    }

    if ((u16)(a + p->turnMax) < p->turnMax * 2) {
        f32 *t = *e->target;
        f32 dx = pos[0] - t[0];
        f32 dy = pos[1] - t[1];
        f32 dz = pos[2] - t[2];

        if (dx * dx + dy * dy + dz * dz <= p->unk18 * p->unk18) {
            /* state holds the done callback (FxHomingSetState) */
            ((void (*)(s32, FxHoming *))e->state)(e->unk4, e);
            e->state = 0;
        }
    }
}
