#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "field/state.h"
#include "field/n12.h"

void FldThreatGaugeDraw(void);

void FldDraw(s32 arg0) {
    if (sysState.unkE == 0) {
        FldSetDrawContext(arg0, 0, 0);
        TxtGlyphQueueFlush();
        GphViewSetClip(camField[0].viewNo, 1.0f, 40000.0f);
        GphViewSelect(camField[0].viewNo, camField[0].roll);
        GphDrawListSet(fldSkyDrawList);
        GphDrawListDrawAll();
        FldSetDrawContext(arg0, 0, 0);
        GphViewSetClip(camField[0].viewNo, camField[0].clipNear, camField[0].clipFar);
        GphViewSelect(camField[0].viewNo, camField[0].roll);
        if (AmapIsLoaded() != 0) {
            AmapDraw();
        }
        GphDrawListSet(fldMapDrawListSize[1]);
        GphDrawListDrawLayer(0);
        GphEnvMapDrawLayer(0, 0);
        GphDrawListSet(fldPartyLitDrawList);
        GphShadowDrawLayer(0);
        GphShadowDrawLayer(7);
        FldSetDrawContext(arg0, 0, 0);
        GphDrawListSet(fldInstDrawListSize[1]);
        GphDrawListDrawLayer(0);
        GphDrawListSet(fldTenantDrawList);
        GphDrawListDrawAll();
        GphEnvMapDrawLayer(2, 0);
        GphDrawListSet(fldPartyDrawList);
        GphDrawListDrawAll();
        GphEnvMapDrawLayer(2, 0);
        PtclDustDraw();
        GphDrawListSet(fldInstDrawListSize[1]);
        GphDrawListDrawLayer(7);
        GphDrawListSet(fldMapDrawListSize[1]);
        GphDrawListDrawLayer(7);
        ChrStunStarsDraw();
        FldTreasureDrawNames();
        EffDraw();
        PtclFldDraw();
        GphDrawListSet(fldPtclDrawList);
        GphDrawListDrawLayer(0);
        GphDrawListDrawLayer(7);
        GphDrawListDrawLayer(10);
        GphBatchSetOffset(0, 0);
        GphScreenFxDraw(fldScreenFx, sysState.screenCaptureAddr);
        GphScreenFxDraw(&fldScreenFx[1], sysState.screenCaptureAddr);
        GphPrimListDraw(fldPacketList);
        FldSpriteDraw();
        FldDmgPopDraw();
        AmapFloorBannerDraw();
        FldLensFlareDraw();
        FldPictureDraw();
        FldTitleDraw();
        GphViewSelect(7, camField[0].roll);
        GphBatchSetOffset(0, 0);
        GphFadeDraw(fldFades);
        MiniDraw();
        FldMenuWinDraw();
        EvtFaceWinDraw();
        FldSpriteStub();
        TxtMsgCursorDraw(0);
        WmapDrawWarp();
        FldThreatGaugeDraw();
        FldMenuDraw();
        GphFadeDraw(&fldFades[1]);
        FldOverlayImgDraw();
        FldSetDrawContext(arg0, 1, 0);
        MapMiniDraw();
        FldSetDrawContext(arg0, 0, 0);
        MapMiniFrameDraw();
        GphBatchSetOffset(0, 0);
        FldSndDebugPrintTime();
    }
}
