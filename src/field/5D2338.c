#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n15.h"
#include "field/n20.h"

extern s32 fldUnusedVar344;
extern s32 chrInstSysData;
extern s32 fldUnusedVar398;
extern s32 fldUnusedVar3B8;
extern s32 fldUnusedVar3BC;
extern s32 fldUnusedVar3F4;
extern s32 fldUnusedVar40C;

void FldModulesInit(void) {
    fldUnusedVar398 = 0;
    fldUnusedVar3B8 = 0;
    chrInstSysData = 0;
    fldUnusedVar40C = 0;
    fldUnusedVar3F4 = 0;
    mapMotionTable = NULL;
    fldCharDefTable = NULL;
    fldUnusedVar3BC = 0;
    fldUnusedVar344 = 0;
    mapBlock178 = 0;
    mapBlock178Data = 0;
    mapMiniPac = NULL;
    EvtScnrHighIdLatchReset();
    CamFieldInit();
    CamScrInit();
    ChrListClear();
    TxtMsgWinStateClear();
    TxtGlyphInit();
    TxtMsgCursorInit();
    OccsInit(0);
    OccInit(0);
    MapPathInit();
    ChrPartyModelsClear();
    MapModelInit();
    FldSkyInit();
    ChrInstInit();
    EffInit();
    MapCollInit();
    MapWallInit(0);
    ChrTenantInit();
    ChrEnemyWorkPoolInit();
    FldLensFlareInit();
    FldMoveCmdPoolInit();
    AmapInit();
    FldPointLightInit();
    FldTreasureInit();
    DbgFldInit();
    EvtAreaInit();
    MapNearObjInit();
    FldSpotInit();
    MapSetFileReset();
    FldInputLockReset();
    FldDmgPopInit();
    FldEncGroupClear();
    FldSndInit();
    FldMenuWorkInit();
    FldMenuWinInit();
    EvtScnrAreaReset();
    MiniInit();
    WmapInit();
    PtclFldInit();
    FldFlagListInit();
    GphEnvMapClear();
    EvtCharSelInit();
    FldSpriteInit();
    FldTitleInit();
    EvtFaceWinInit();
    MapMiniInit();
    FldThreatGaugeInit();
    ItemWorkInit();
    CamBlendInit();
    FldLightFadeInit();
    FldOverlayImgInit();
    PtclDustInit();
    FldPictureInit();
    fldState.mode = FLD_MODE_PLAY;
}
