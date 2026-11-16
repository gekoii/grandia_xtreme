#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"
#include "pad.h"

typedef struct Tbl {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 uC;
    /* 0x10 */ s32 u10;
    /* 0x14 */ s32 u14;
} Tbl;

typedef struct Local {
    s32 type;
    u8 pad4[0xC];
    f32 v[3];
} Local;

void VuSinCosDeg(f32 *, f32 *, f32);

void OccTrapForward(OccEntry *o) {
    OccTrap *s = OCC_PARAMS(o, OccTrap);
    Local l;
    f32 sn;
    f32 cs;
    f32 spd;
    f32 ang;
    ChrObj *c;
    Tbl *t;
    s32 n;
    s32 k;
    u16 t2;

    c = ChrGetByNo(1);
    l.type = 2;
    if (c->blinkTimer != 0) {
        return;
    }
    t = MapSetFileGet();
    if (t == NULL) {
        return;
    }
    FldSndSePlay(1, VuRandInt(2) + 0xFB0, 0);
    FldSndSePlay(1, 0x7E3, 0);
    GphScreenFxSet(&fldScreenFx2, 1, 1, 10, 64.0f);
    switch (OCC_PARAMS(o, OccTrap)->damage) {
    case 0:
        break;
    case 1:
        FldTrapDamageApply(&c->hpGauge, t->u14);
        PadVibrate(padPort0, 0, 0xFF, 10);
        break;
    case 2:
        FldTrapDamageApply(&c->hpGauge, t->u10);
        PadVibrate(padPort0, 1, 0xA0, 10);
        break;
    case 3:
        FldTrapDamageApply(&c->hpGauge, t->uC);
        PadVibrate(padPort0, 1, 0xFF, 0x14);
        break;
    }
    if (!(s->hit & 1)) {
        return;
    }
    l.v[0] = 0.0f;
    switch (s->size) {
    case 1:
        l.v[1] = 4.3f;
        l.v[2] = 14.0f;
        spd = 10.0f;
        break;
    case 2:
        l.v[2] = 6.0f;
        spd = 6.0f;
        l.v[1] = 2.3f;
        break;
    case 0:
    default:
        l.v[1] = 2.7f;
        l.v[2] = 10.0f;
        spd = 8.0f;
        break;
    }
    t2 = s->stun;
    switch (t2) {
    case 0x12D:
        n = 0x3C;
        break;
    case 0x12E:
        ChrStunStart(c, 0x3C);
        n = 0x3C;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    case 0x12F:
        ChrStunStart(c, 0x78);
        n = 0x3C;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    case 0x130:
        ChrStunStart(c, 0x96);
        n = 0x3C;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    case 0x131:
        n = 0xE1;
        break;
    case 0x132:
        n = 0;
        break;
    case 0x133:
        ChrStunStart(c, 0x78);
        n = 0x78;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    case 0x134:
        ChrStunStart(c, 0x3C);
        n = 0x78;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    case 0x135:
        ChrStunStart(c, 0x5A);
        n = 0x78;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    case 0x136:
        ChrStunStart(c, 0x78);
        n = 0x78;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    case 0x137:
        ChrStunStart(c, 0x96);
        n = 0x78;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    case 0x138:
        ChrStunStart(c, 0xB4);
        n = 0x78;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    case 0x139:
        ChrStunStart(c, 0xD2);
        n = 0x78;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    case 0x13A:
        ChrStunStart(c, 0xF0);
        n = 0x78;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    case 0x13B:
        ChrStunStart(c, 0x10E);
        n = 0x78;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    case 0x13C:
        ChrStunStart(c, 0x12C);
        n = 0x78;
        SndSePlay(0x7FC, 0, 0, 0x1E, 0);
        break;
    default:
        n = t2;
        break;
    }
    c->blinkTimer = n;
    c->actFlags |= 0x20;
    switch (s->knock) {
    case 1:
        l.v[1] = 0.0f;
        k = 2;
        break;
    case 2:
        k = 3;
        break;
    case 0:
    default:
        return;
    }
    if (s->dir == 0) {
        ang = -((c->heading + 0x8000) * 0.0054931640625f);
    } else {
        ang = s->dir * 45;
    }
    VuSinCosDeg(&sn, &cs, ang);
    l.v[0] = sn * l.v[2] * s->scale;
    l.v[1] = l.v[1] * s->scale;
    l.v[2] = cs * l.v[2] * s->scale;
    /* cast: EABI loads the float argument first, as declared here (a plain call swaps two loads) */
    ((void (*)(ChrObj *, f32 *, f32, s32))FldObjMoveVelocity)(c, l.v, spd, k);
}
