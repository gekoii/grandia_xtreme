#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "party.h"

void PtyMoveLevelsUpdate(void);
extern char btlCfgFileName[];

void BtlDebugCfgLoad(void) {
    s32 i;
    s32 r;
    PtyMoveLevel *e;

    btlDebugCfg.flags = 0;
    btlDebugCfg.mgcNo = 0;
    btlDebugCfg.mgcCmb = 1;
    btlDebugCfg.aiNo = -1;
    btlDebugCfg.atkNo = -1;
    btlDebugCfg.ipStopMask = 0;
    btlDebugCfg.dimKind = -1;
    btlDebugCfg.se = -1;
    if (btlFromField != 1) {
        PtyMoveLevelsUpdate();
        e = &ptyPartyData->moveLevels[PTY_MOVE_LEVEL_MOVES];
        for (i = 0; i < 50; i++, e++) {
            if (e->moveId >= 0xF1) {
                e->level = 1;
            }
        }
        for (i = 0; i < 30; i++) {
            if (ptyPartyData->bag[i].count == 0) {
                ptyPartyData->bag[i].id = 0x67;
                ptyPartyData->bag[i].count = 10;
            }
        }
        r = UtilLoadFile((s32)btlCfgFileName, 0);
        DbgBtlCfgParse(r);
        MemFree(r);
    }
}
