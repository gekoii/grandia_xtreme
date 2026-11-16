/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"


void BtlUnitStateHitHeavy(BtlUnit *arg0) {
    s32 fl;
    f32 f;
    f32 g;
    f32 h;

    switch (arg0->phase) {
    case 0:
        switch (arg0->subPhase) {
        case 0:
            fl = arg0->params->status;
            if (fl & 0x80) {
                BtlChrSetMotion(arg0, 0);
                BtlChrMotionSetFrame(arg0, arg0->loopEnd);
                arg0->frameSpeed = 0.0f;
            } else if ((fl & 0x2000) && !(arg0->flags & 0x400)) {
                BtlChrSetMotion(arg0, 1);
            } else {
                BtlChrSetMotion(arg0, 2);
            }
            arg0->evtLoop = 0;
            arg0->loopStart = arg0->loopEnd;
            arg0->subPhase++;
            arg0->stateCount = 0;
        case 1:
            BtlUnitShakeOffset(arg0, 8.0f);
            if (arg0->stateCount >= 7) {
                arg0->unkD0[0] = arg0->unkD0[1] = arg0->unkD0[2] = 0.0f;
                if (arg0->params->status & 0x80) {
                    arg0->subPhase = 0;
                    arg0->phase++;
                } else {
                    arg0->subPhase++;
                }
            }
            break;
        case 2:
        case 3:
            if (!(arg0->params->bitCsf & 4)) {
                arg0->knockback -= 1.0f;
                if (arg0->knockback < 0.0f) {
                    arg0->knockback = 0.0f;
                }
                BtlUnitMoveDir(arg0, arg0->rot[1] + 0x8000, (arg0->params->bitCsf & 0x200) ? 1 : 3, arg0->knockback);
            }
            if (arg0->subPhase == 2) {
                if (arg0->frameSpeed <= 0.0f) {
                    if (arg0->params->status & 0x20E0) {
                        arg0->subPhase = 0;
                        arg0->phase++;
                    } else if (arg0->id < 4) {
                        if (arg0->motionNo == 2) {
                            arg0->wake = 30;
                        }
                        BtlUnitSetState(arg0, 0x40);
                    } else {
                        arg0->stateCount = 0;
                        arg0->subPhase = 3;
                    }
                }
            } else if (arg0->stateCount + arg0->loopEnd >= 68.0f) {
                BtlUnitSetState(arg0, 0x40);
            }
            break;
        }
        break;
    case 1:
        switch (arg0->subPhase) {
        case 0:
            if (arg0->params->status & 0x2000) {
                h = 56.0f;
            } else {
                h = 0.0f;
            }
            arg0->unk1D8 = h;
            arg0->subPhase = 1;
        case 1:
            if (!(arg0->params->bitCsf & 4) && arg0->knockback > 0.0f) {
                if (arg0->knockback < 0.0f) {
                    arg0->knockback = 0.0f;
                }
                BtlUnitMoveDir(arg0, arg0->rot[1] + 0x8000, (arg0->params->bitCsf & 0x200) ? 1 : 3, arg0->knockback);
                arg0->knockback -= 1.0f;
            }
            if (btlActingUnit == 0xFF) {
                f = arg0->unk1D8;
                if (f >= 0.0f) {
                    g = f + 1.0f;
                    arg0->unk1D8 = g;
                    if (g >= 68.0f) {
                        BtlHudComboPopupAdd(arg0->id);
                        arg0->unk1D8 = -1.0f;
                    }
                }
            }
            if (!(arg0->params->status & 0x20E0)) {
                BtlUnitSetState(arg0, 0x40);
            }
            break;
        }
        break;
    }
    if (BtlChrMotionGetEvent(arg0, 2, 0) != 0) {
        arg0->frameSpeed = 0.0f;
    }
}
