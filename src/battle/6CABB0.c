/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "pad.h"

extern s16 btlPartyKoSeTable[];

#define UNIT(i) (&btlUnits[i])

void BtlUnitStatePartyKo(BtlUnit *arg0) {
    PtyUnit *info;
    s32 i;
    s32 j;
    s32 v;
    f32 f;

    switch (arg0->phase) {
    case 0:
        switch (arg0->subPhase) {
        case 0:
            SndSePlay3D(btlPartyKoSeTable[arg0->params->id - 0x1001], 0, 8, &arg0->pos[0]);
            PadVibrate(padPort0, 1, 0xDC, 0x78);
            arg0->stateCount = 0;
            arg0->subPhase++;
        case 1:
            BtlUnitShakeOffset(arg0, 8.0f);
            if (arg0->stateCount >= 7) {
arg0->stateCount = 0; arg0->unkD0[0] = arg0->unkD0[1] = arg0->unkD0[2] = 0.0f; arg0->subPhase++;
            }
            break;
        case 2:
            if (arg0->knockback > 0.0f) {
                if (arg0->flags & 2) {
                    arg0->knockback -= 0.25f;
                } else {
                    arg0->knockback -= 3.0f;
                }
                BtlUnitMoveDir(arg0, arg0->rot[1] + 0x8000, 3, arg0->knockback);
            }
            j = 0;
            while (j < 4 && !(UNIT(j)->state == 4 && UNIT(j)->phase == 0 && UNIT(j)->frameSpeed > 0.0f)) {
                j++;
            }
            if (j < 4) {
                break;
            }
            btlSlowMotionRate = 0;
            btlSlowMotionTimer = 0;
            arg0->stateCount = 0;
            v = ptyCharData[arg0->params->id - 0x1001].equip[5];
            if (v > 0 && ptyItemTable[v].equip->effectType == 0x6B) {
                arg0->phase++;
                arg0->subPhase = 0;
                arg0->unk1D8 = -1.0f;
                return;
            }
            info = arg0->params;
            if (arg0->params->autoYomi > 0.0f && (rand() >> 15) * (1.0f / 65536.0f) <= info->autoYomi) {
                f = 1.0f;
            } else {
                arg0->subPhase++;
                break;
            }

            arg0->phase++;
            arg0->subPhase = 0;
            arg0->unk1D8 = f;
            return;
        case 3:
            if (arg0->stateCount >= 17) {
                i = 0;
                while (i < 4 && !(UNIT(i)->state == 4 && UNIT(i)->phase == 1)) {
                    i++;
                }
                if (btlPartyAliveCount <= 0 && i >= 4) {
                    for (i = 0; i < 4; i++) {
                        if (btlUnits[i].state == 2) {
                            break;
                        }
                    }
                    if (i >= 4) {
                        BtlUnitSetState(arg0, 2);
                        return;
                    }
                }
                BtlSchedKoDone(arg0->id);
                arg0->phase = 2;
                arg0->subPhase = 1;
            }
            break;
        }
        if (BtlChrMotionGetEvent(arg0, 2, 0) != 0) {
            arg0->frameSpeed = 0.0f;
        }
        break;
    case 1:
        if (arg0->stateCount >= 17) {
            BtlSchedKoDone(arg0->id);
            if (arg0->unk1D8 > 0.0f) {
                BtlUnitSetReviveHpMode(arg0, 2);
            } else {
                BtlNoticeBrokenShow(ptyItemTable[ptyCharData[arg0->params->id - 0x1001].equip[5]].name);
                ptyCharData[arg0->params->id - 0x1001].equip[5] = 0;
                BtlUnitRecalcParams(arg0);
                BtlUnitSetReviveHpMode(arg0, 0);
            }
            PtclBurstSpawnAtUnit(0xD, arg0, arg0->rot[1]);
            BtlHitQueueAddStatus(arg0, 0x8000, 0, 0x200, 0x5DC);
            return;
        }
        break;
    case 2:
        if (arg0->subPhase == 0) {
            BtlChrSetMotion(arg0, 2);
            arg0->evtLoop = 0;
            arg0->loopStart = arg0->loopEnd;
            BtlChrMotionSetFrame(arg0, BtlChrMotionFindKey(arg0, 2, 0));
            arg0->frameSpeed = 0.0f;
            ChrFaceAnimSet(&arg0->face, 5, 0);
            arg0->subPhase++;
        }
        break;
    }
}
