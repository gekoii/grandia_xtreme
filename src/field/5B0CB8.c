#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "field/state.h"
#include "field/n12.h"

void FldPointLightUpdate(Camera *);

void FldUpdateNoControl(s32 arg0) {
    FldTitleUpdate();
    fldPerfTimes[0] = GsGetFrameTimePercent();
    EvtAreaUpdate(padPort0);
    fldPerfTimes[1] = GsGetFrameTimePercent();
    OccsUpdate();
    OccUpdate();
    fldPerfTimes[2] = GsGetFrameTimePercent();
    FldSpotUpdate();
    fldPerfTimes[3] = GsGetFrameTimePercent();
    FldSkyUpdate();
    AmapUpdate();
    MapModelUpdate();
    FldTreasureUpdate();
    ChrInstUpdate();
    ChrPartyUpdateMotions();
    fldPerfTimes[4] = GsGetFrameTimePercent();
    ChrTenantUpdate();
    ChrTenantUpdateMotions();
    fldPerfTimes[5] = GsGetFrameTimePercent();
    ChrPlayerScriptModeUpdate();
    fldPerfTimes[6] = GsGetFrameTimePercent();
    fldPerfTimes[7] = GsGetFrameTimePercent();
    TxtMsgWinUpdate();
    TxtGlyphFadeIn();
    EvtFaceWinUpdate();
    TxtMsgCursorUpdate();
    fldPerfTimes[8] = GsGetFrameTimePercent();
    EffUpdate();
    fldPerfTimes[9] = GsGetFrameTimePercent();
    FldSndUpdate((u8 *)camField, 3000.0f);
    EvtAreaClearAllMoves();
    MapNearObjUpdate();
    FldPointLightUpdate(camField);
    FldMenuWinUpdate();
    MapRippleUpdate();
    MiniUpdate();
    PtclFldUpdate();
    FldSpriteUpdate();
}
