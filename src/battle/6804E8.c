#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "gph.h"
#include "battle/camera.h"
#include "sys.h"


void BtlDraw(s32 arg0) {
    btlFrameCount++;
    btlDbgStage = 0x10000000;
    CamApply(btlCamera);
    BtlSetDrawContext(arg0);
    EnvStageDraw();
    GsFillFbAlpha();
    EnvShadowDraw();
    CamApply(btlCamera);
    GphTexTableSelect(btlTexTable);
    GphTexBind(0x33);
    PtclBtlDustDrawGround();
    GphTexTableSelect(gphTexTableBuf);
    EnvStageShadeDraw();
    GphDrawListSet(btlPartyDrawList);
    GphDrawListDrawLayer(0);
    GphEnvMapDrawLayer(0, 0);
    GphDrawListDrawLayer(7);
    GphDrawListSet(btlEnemyDrawList);
    GphDrawListDrawLayer(0);
    GphEnvMapDrawLayer(0, 1);
    GphDrawListDrawLayer(7);
    GsFillFbAlpha();
    FxCloneDraw();
    FxAfterimageDraw();
    CamApply(btlCamera);
    GphDrawListSet(btlSysDrawList);
    GphDrawListDrawAll();
    GphTexTableSelect(btlTexTable);
    GphDrawListSet(btlFxDrawList);
    GphDrawListDrawAll();
    FxTraceDraw();
    MgcDraw();
    FxAtkDraw();
    PtclBurstDrawAll();
    PtclBtlDustDraw();
    FxUnitDraw();
    EnvFlareDraw();
    GphScreenFxDraw(&btlScreenFx, sysScreenCaptureAddr);
    DbgBtlHitDispDraw();
    BtlFadeDraw();
    BtlCmdMenuDrawTargets();
    BtlHudDraw();
    BtlMenuDraw();
    switch (btlPhase) {
    case 1:
        BtlStartDraw();
        break;
    case 2:
        BtlResultDraw();
        break;
    }
    GphTexTableSelect(gphTexTableBuf);
    btlDbgStage = 0;
}
