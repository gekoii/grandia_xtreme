#include "common.h"
#include "protos/battle.h"
#include "battle/unit.h"


void BtlUnitHitAreaTargetsFrom(BtlUnit *arg0, s32 arg1, f32 x, f32 z) {
    BtlUnit *u;
    s32 ang;
    u32 bit;
    s32 h;

    u = btlUnits;
    if (arg0->body >= 0) {
        ang = u[arg0->body].rot[1] + 0x8000;
    } else {
        ang = arg0->rot[1] + 0x8000;
    }
    bit = 1;
    h = BtlHitGetNextVictimHandler();
    for (; bit <= 0xFFFF; u++) {
        if (btlTargetMask & bit) {
            BtlHitSetNextVictimHandler(h);
            if (u->id == arg0->target && (u8)arg0->scopeType != 5) {
                BtlHitQueueAddAttack(arg0, u, ang, arg1);
            } else {
                BtlHitQueueAddAttack(arg0, u, BtlAtan2Angle(x - u->pos[0], z - u->pos[2]), arg1);
            }
        }
        bit <<= 1;
    }
}
