#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n21.h"


void AmapOccAddFloat(AmapWork *a, s32 kind, s16 v, s32 c, s32 x, s32 y, s32 z) {
    OccEntry *e;
    s32 i;
    s32 t;
    s32 r;

    e = &a->occ[a->occCount];
    e->state = 0;
    e->id = a->occCount;
    e->unk6 = v;
    e->unk1A = 0;
    e->unk1C = 0;
    e->unk1E = 0;
    for (i = 6; i >= 0; i--) {
        ((s16 *)&e->cond)[i] = 0;
    }
    e->flags = 0;
    switch (kind) {
    case 0xAA:
        e->flags = 0x43;
        AmapOccSetMoveB(e, c, 0, 0, x, y, -z, VuRandInt(100) + 200, 0);
        break;
    case 0xAB:
        e->flags = 0x43;
        t = 1000;
        y = 300;
        VuRandInt(10);
        VuRandInt(15);
        VuRandInt(10);
        VuRandInt(16);
        r = VuRandInt(4);
        switch (r) {
        case 0:
            break;
        default:
            t = 3000;
            y = 900;
            break;
        case 1:
            t = 2000;
            y = 600;
            break;
        }
        AmapOccSetRotate(e, c, 1, 1, x, y, -z, t, 0);
        break;
    }
    a->occCount++;
}
