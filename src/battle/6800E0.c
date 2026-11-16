/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "gph.h"
#include "party.h"
#include "sys.h"


extern void (*btlSysDrawHook)(void);

void BtlStart(void) {
    s8 cnt[8];
    s32 i;
    s32 j;
    PtySkill *m;

    MemHeapReset(sysState.battleHeap);
    MemHeapSelect(sysState.battleHeap);
    btlTexTable = MemAlloc32(GphTexTableSize(0x100));
    GphTexTableInit(0x100, btlTexTable);
    GphTexTableSelect(btlTexTable);
    btlSysDrawList = MemAlloc32(GphDrawListSize(0x100));
    GphDrawListInit(0x100, btlSysDrawList);
    GphDrawListSet(btlSysDrawList);
    GphDrawListClear(NULL);
    btlFxDrawList = MemAlloc32(GphDrawListSize(0x100));
    GphDrawListInit(0x100, btlFxDrawList);
    GphDrawListSet(btlFxDrawList);
    GphDrawListClear(NULL);
    GphScreenFxClear(&btlScreenFx);
    TxtFontImageClear(&sysState.fontImages[0]);
    TxtFontImageClear(&sysState.fontImages[1]);
    TxtFontImageClear(&sysState.fontImages[2]);
    BtlFileInit();
    if (btlFromField != 0) {
        btlAutoTactics = ptyPartyData->autoTactics;
    }
    btlTacticsLabelsOn = btlAutoTactics != 0;
    if (btlFromField == 0) {
        m = ptyPartyData->skills;
        for (i = 0; i < 8; i++) {
            cnt[i] = 0;
        }
        for (i = 0; i < 100; i++, m++) {
            if (m->id != 0) {
                PtySkillEquip(m->owner + 0x1001, cnt[m->owner], i);
                cnt[m->owner]++;
            }
        }
    }
    BtlUnitIdsSetup();
    BtlDataLoad();
    DbgBtlHitDispInit();
    EnvLightInit();
    BtlCamInit();
    EnvFlareInit();
    ChrPartDispInit();
    FxUnitInit();
    BtlUnitsInit();
    EnvShadowInit();
    MgcInit();
    FxAtkInit();
    BtlUtilInit();
    BtlHitQueueInit();
    PtclBurstInit();
    IpInit();
    BtlHudDmgInit();
    BtlStartInit();
    BtlCmdMenuInit();
    FxAfterimageInit();
    BtlMenuInit();
    FxTraceInit();
    PtclBtlDustInit();
    FxHitInit();
    AiTacticsInit();
    BtlResultInit();
    BtlDropInit();
    BtlFadeInit();
    BtlSpecialIntroInit();
    BtlChrModelInvalidate(0x10);
    for (j = 0; j < 4; j++) {
        btlUnitArrayTail[j] = 0;
    }
    btlSysSpeed = 1;
    btlSlowMotionRate = 0;
    btlSlowMotionTimer = 0;
    btlSlowWait = 0;
    btlTickCount = 0;
    btlFrameCount = 0;
    btlPartyHurtLevel = 0;
    btlSpecialUsedMask = 0;
    btlExitRequest = 0;
    btlDbgStage = 0;
    btlHandoff.dropCount = 0;
    if (btlHandoff.debugStart == 1) {
        btlHudHideFlags |= 0x1F;
        btlDebugCfg.flags |= 0x36;
    } else if (btlFromField == 1) {
        btlDebugCfg.flags &= 0xFFFFFFC9;
    }
    btlSysDrawHook = NULL;
    BtlStartUpdate();
    BtlCamUpdate();
    GphTexTableSelect(gphTexTableBuf);
}
