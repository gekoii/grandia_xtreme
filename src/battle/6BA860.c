#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"


void BtlUnitSetState(BtlUnit *u, s32 state) {
    s32 old;
    BtlCharDef *ops;
    s32 fn;

    do {} while (0); /* do-while(0) x2: block boundaries, the two stores stay ahead of the loads */
    u->unk188 = 0;
    u->unk18C = 0;
    do {} while (0);
    ops = u->def;
    old = u->state;
    u->state = state;
    fn = *(s32 *)&ops->setState; /* s32 load: a pointer load moves above the state store */
    u->subPhase = 0;
    u->phase = 0;
    ((void (*)(BtlUnit *, s32))fn)(u, state);
    if (state != 0x40 && (u->id >= 4 || u->motionNo != 6) && (old != 0x43 || state != 0x84) && state != 2) {
        ChrPartDispApply(u);
        ChrFaceAnimSet(&u->face, 0, 1);
    }
    u->flags &= 0xFFFFFEFD;
    FxTraceStopAll(u->id);
    u->turnLeft = 0;
}
