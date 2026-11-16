/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "battle/misc.h"
#include "battle/camera.h"
#include "party.h"
#include "sys.h"

extern u8 btlEscapeOkCaption[];
extern u8 btlEscapeFailCaption[];
extern s8 btlEscapeSucceeded;

void BtlUnitStateEscape(BtlUnit *arg0) {
    PtyMoveEffect *it;
    BtlUnit *p;
    s32 i;
    s32 k;

    switch (arg0->phase) {
    case 0:
        if (arg0->subPhase == 0) {
            it = ptyMoveTable[arg0->skillId].levels[arg0->skillLevel - 1].effects;
            btlEscapeSucceeded = ((rand() >> 15) * (1.0f / 65536.0f) - arg0->params->escapeUp) * 100.0f <= it->base;
            arg0->stateCount = 0;
            arg0->subPhase = 0;
            arg0->phase++;
            if (btlEscapeSucceeded == 0) {
                k = ((rand() >> 15) & 2) + 0x106C;
                SndStreamPlay((arg0->params->id - 0x1001) * 60 + k, 0, 0);
                arg0->phase = 2;
            } else {
                k = ((rand() >> 15) & 2) + 0x1068;
                SndStreamPlay((arg0->params->id - 0x1001) * 60 + k, 0, 0);
            }
        }
        break;
    case 1:
        switch (arg0->subPhase) {
        case 0:
            BtlChrSetMotion(arg0, 3);
            arg0->stateCount = 0;
            arg0->moveTarget[0] = btlCamParam.eye[0];
            arg0->moveTarget[1] = 0.0f;
            arg0->subPhase++;
            arg0->moveTarget[2] = btlCamParam.eye[2];
        case 1:
            BtlUnitMoveToward(arg0, &arg0->moveTarget, 2, BtlUnitGetMoveSpeed(arg0), 10.0f);
            if (arg0->stateCount == 30) {
                p = btlUnits;
                for (i = 3; i >= 0; i--, p++) {
                    if (p->state >= 0x40 && p != arg0 && !(p->params->status & 0x20E0)) {
                        BtlUnitSetState(p, 0x85);
                        p->frameSpeed = 1.0f;
                        p->flags |= 8;
                        p->alpha = 1.0f;
                        p->evtLoop = 0;
                        p->evtCur = 0;
                        FxTraceStopAll(p->id);
                    }
                }
                IpGaugeSetShown(0);
                BtlCaptionShow(btlEscapeOkCaption, 0xB4, arg0);
            } else if (arg0->stateCount == 60) {
            } else if (arg0->stateCount == 210) {
                BtlCaptionClose();
            } else if (arg0->stateCount == 220) {
                btlHandoffResult = 2;
                btlExitRequest = 1;
            }
            break;
        }
        break;
    case 2:
        switch (arg0->subPhase) {
        case 0:
            BtlChrSetMotion(arg0, 3);
            arg0->stateCount = 0;
            arg0->moveTarget[0] = btlCamParam.eye[0];
            arg0->moveTarget[1] = 0.0f;
            arg0->subPhase++;
            arg0->moveTarget[2] = btlCamParam.eye[2];
        case 1:
            BtlUnitMoveToward(arg0, &arg0->moveTarget, 3, BtlUnitGetMoveSpeed(arg0), 10.0f);
            if (arg0->stateCount == 40) {
                BtlCaptionShow(btlEscapeFailCaption, 0xB4, arg0);
                BtlUnitSetFreeSpotTarget(arg0);
            } else if (arg0->stateCount == 60) {
                arg0->ipMode = 1;
                arg0->ipPos = arg0->ipRet;
                arg0->ipSpeed = 0.0f;
                arg0->scopeType = 0;
                arg0->flags &= 0xFFFFFF7F;
                BtlUnitSetState(arg0, 0x80);
                arg0->action = 0x7F;
                BtlSchedActDone(arg0->id);
                btlPhase = 0x80;
                BtlCamSetMode(1, 0x80, 0, 0);
                SndBgmFadeVolume(btlBgmHandle.b, 0x96, 100.0f);
            }
            break;
        }
        break;
    }
}
