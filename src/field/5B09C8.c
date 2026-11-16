#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "sys.h"
#include "field/state.h"
#include "field/n12.h"


/* FldPointLightUpdate and FldFlagListUpdate are not in C yet. */
void FldPointLightUpdate(Camera *);
void FldFlagListUpdate(void);

void FldUpdatePlay(s32 arg0) {
    WmapUpdate();
    fldPerfTimes[0] = GsGetFrameTimePercent();
    EvtAreaUpdate(padPort0);
    fldPerfTimes[1] = GsGetFrameTimePercent();
    OccsUpdate();
    OccUpdate();
    fldPerfTimes[2] = GsGetFrameTimePercent();
    FldSpotUpdate();
    FldPadApplyInputLock(padPort0);
    if (SysIsDebugMode() != 0) {
        if (!(sysState.dbgMenuFlags & 0x20000000)) {
            ChrPlayerUpdate(padPort0);
            CamUpdate(padPort0);
        }
    } else {
        ChrPlayerUpdate(padPort0);
        CamUpdate(padPort0);
    }
    if (CamScrIsRunning() == 0) {
        if (camCtl.unk8 < 60) {
            camCtl.unk8++;
        }
        if (camCtl.aiType == CAM_AI_FF && camCtl.unk8 >= 5) {
            camCtl.aiType = camCtl.prevType;
        }
    } else {
        camCtl.unk8 = 0;
    }
    if (MiniGetState() == 0) {
        ChrTalkUpdate(padPort0);
    }
    CamBlendUpdate();
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
    EvtScnrPadUpdate(padPort0);
    EvtScnrMain();
    EvtScnrFrameUpdate();
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
    FldSndUpdate(camField, 3000.0f);
    EvtAreaClearAllMoves();
    MapNearObjUpdate();
    FldPointLightUpdate(camField);
    FldMenuWinUpdate();
    MapRippleUpdate();
    MiniUpdate();
    PtclFldUpdate();
    FldFlagListUpdate();
    EvtCharSelUpdate();
    MapMiniUpdate();
    FldThreatGaugeUpdate();
    FldLightFadeUpdate();
    FldSpriteUpdate();
    AmapFloorBannerUpdate();
    PtclDustUpdate();
    FldPictureUpdate();
    FldOverlayImgUpdate();
    if ((padPort0[0].trigger & 0x10) && ChrPlayerIsBusy() == 0) {
        FldMenuOpen(0);
    }
}
