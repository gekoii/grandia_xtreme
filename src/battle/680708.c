#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "gph.h"
#include "battle/unit.h"
#include "party.h"
#include "sys.h"


void BtlUpdate(s32 arg0) {
    s32 n;
    s32 i;
    s32 j;
    s32 m;
    s32 p;
    s32 q;
    BtlUnit *u;

    btlDbgStage = 0x20000000;
    BtlSetDrawContext(arg0);
    GphTexTableSelect(btlTexTable);
    GphDrawListSet(0);
    n = btlSysSpeed;
    while (n > 0) {
        BtlCamUpdate();
        if (sysState.pauseMode != 2) {
            btlTickCount++;
            EnvDimUpdate();
            if (btlSlowWait <= 0) {
                switch (btlPhase) {
                case 1:
                    BtlStartUpdate();
                    break;
                case 2:
                    BtlResultUpdate();
                    break;
                }
                btlDbgStage += 0x1000000;
                BtlUnitsUpdate();
                btlDbgStage += 0x1000000;
                MgcUpdate();
                btlDbgStage += 0xFE000000;
                BtlSpecialIntroUpdate();
                FxAtkUpdate();
                BtlHitQueueProcess();
                BtlSchedUpdate();
                FxTraceUpdate();
                FxHitUpdate();
                FxAfterimageUpdate();
                PtclBurstUpdateAll();
                FxUnitUpdateCommon();
                PtclBtlDustUpdate();
                BtlDropObjUpdate();
                EnvStageUpdate();
                FxCloneUpdate();
                EnvFlareUpdate();
                btlSlowWait = btlSlowMotionRate;
            } else {
                btlSlowWait--;
            }
            BtlMenuUpdate();
            BtlHudUpdate();
            BtlFadeUpdate();
            DbgBtlHitMarkFade();
            if (btlSlowMotionTimer > 0) {
                if (--btlSlowMotionTimer <= 0) {
                    btlSlowMotionTimer = 0;
                    btlSlowMotionRate = 0;
                    btlSlowWait = 0;
                }
            }
        } else {
            BtlChrModelInvalidate(0x10);
            u = btlUnits;
            for (p = 0; p < 16; p++, u++) {
                if (u->state > 0 && u->model != NULL) {
                    BtlChrModelUpdate(u);
                }
            }
        }
        m = n - 1;
        GphScreenFxUpdate(&btlScreenFx);
        n = m;
    }
    if (btlExitRequest != 0) {
        s32 r = FileGetReadState();
        if (r == -1 && SndSeBankLoadGetState() == r) {
            ptyPartyData->autoTactics = btlAutoTactics;
            for (i = 0; i < 4; i++) {
                if (btlUnitIds.ids[i] != 0) {
                    for (j = 0; j < 2; j++) {
                        ptyCharData[btlUnitIds.ids[i] - 0x1001].tactics[j] = btlTactics[i][j];
                    }
                }
            }
            u = btlUnits;
            for (q = 0; q < 4; q++, u++) {
                if (u->state >= 0x40) {
                    u->regene = 0;
                    u->antiMagic = 0;
                    u->transp = 0;
                    u->params->status &= 0xFFFF9814;
                    ((union { s16 v; } *)&u->vanish)->v = 0;
                    u->confu = 0;
                    u->sealM = 0;
                    u->sealS = 0;
                    u->bind = 0;
                    u->sleep = 0;
                    PtyUnitCalcParams(u->params, 0, 0);
                }
            }
            SysProgRequest(0, 2);
        }
    }
    GphTexTableSelect(gphTexTableBuf);
    btlDbgStage = 0;
}
