#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/camera.h"
#include "pad.h"
#include "sys.h"

void CamDebugPadControl(void *, void *);
extern void (*btlSysDrawHook)(void);

void BtlBuildDrawLists(s32 arg0) {
    btlDbgStage = 0x30000000;
    CamApply(btlCamera);
    BtlSetDrawContext(arg0);
    DbgSetCamera(btlCamera);
    BtlChrModelListReset();
    EnvDimApply();
    EnvStageBuildDrawList();
    EnvLightSelectDim(0);
    GphDrawListSet(btlSysDrawList);
    GphDrawListClear(NULL);
    GphDrawListSet(btlFxDrawList);
    GphDrawListClear(NULL);
    MgcBuildDrawList();
    BtlUnitsPreDraw();
    FxCloneBuild();
    EnvLightSelectDim(0);
    GphDrawListSet(btlSysDrawList);
    BtlDropObjRegister();
    if (btlSysDrawHook != NULL) {
        btlSysDrawHook();
    }
    GphDrawListSet(btlFxDrawList);
    FxAtkBuild();
    FxHitDraw();
    PtclBurstRegisterAll();
    if (sysDbgMenuFlags & 0x20000000) {
        CamDebugPadControl(btlCamera, &padPort1);
    }
    GphDrawListSort();
    GphDrawListSet(btlSysDrawList);
    GphDrawListSort();
    GphDrawListSet(0);
    EnvShadowBuild();
    FxAfterimageBuild();
    BtlCmdMenuBuild();
    btlDbgStage = 0;
}
