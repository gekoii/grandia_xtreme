#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "party.h"

/* Hit request (BtlHitQueueAdd). */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 unkC;
    PtyUnit *unk10;
    f32 unk14;
    s32 unk18;
} Req;


void FxAtkHitRegister(BtlUnit *c, s32 arg1, s32 arg2, s8 arg3, s32 arg4) {
    Req r;
    PtyUnit *o;

    o = c->params;
    r.unk0 = c->id;
    r.unkC = (u8)c->body;
    r.unk10 = o;
    r.unk14 = 0.0f;
    r.unk18 = 0;
    if (c->id < 4) {
        if (arg3 == 2) {
            r.unk4 = 1;
            r.unk8 = c->def->id - 0xF42;
        } else {
            r.unk4 = 0;
            r.unk8 = c->def->id - 0xF4C;
        }
        /* the attack moves' first level keeps the power where other moves have their effects */
        arg4 = (s32)ptyMoveTable[r.unk8].levels->effects;
        if (arg3 != 0) {
            r.unk14 = o->eqpStaProb;
            r.unk18 = o->eqpBitSta;
            if (arg3 == 1) {
                arg4 += 0x1C;
            }
        }
    } else {
        r.unk4 = 0;
        r.unk8 = 0;
    }
    BtlHitSetNextFlags(BtlHitGetNextFlags() | 0x400000);
    BtlHitQueueAdd(&r, arg1, arg2, arg4);
}
