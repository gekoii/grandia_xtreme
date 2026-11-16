#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n12.h"

void VuMtxStackReset(void);
void VuMtxStackLoad(Camera *);

void FldBuildDrawLists(s32 arg0) {
    FldSetDrawContext(0, 0, 0);
    fldPerfTimes[9] = GsGetFrameTimePercent();
    GphProjTexClear();
    VuMtxStackReset();
    GphDrawListClear(NULL);
    VuMtxStackLoad(camField);
    GphLightListUpdate();
    if (FldMenuIsActive() != 0) {
        GphDrawListSet(fldMapDrawList);
        GphDrawListClear(NULL);
        GphDrawListSet(fldSkyDrawList);
        GphDrawListClear(NULL);
        GphDrawListSet(fldInstDrawList);
        GphDrawListClear(NULL);
        GphDrawListSet(fldPartyDrawList);
        GphDrawListClear(NULL);
        GphDrawListSet(fldTenantDrawList);
        GphDrawListClear(NULL);
        GphDrawListSet(fldPartyLitDrawList);
        GphDrawListClear(NULL);
        GphDrawListSet(fldPtclDrawList);
        GphDrawListClear(NULL);
        GphDrawListSet(fldDrawListMiniMapBuf);
        GphDrawListClear(NULL);
        return;
    }
    fldPerfTimes[9] = GsGetFrameTimePercent();
    MapModelBuildDrawList();
    AmapBuildDrawList();
    fldPerfTimes[10] = GsGetFrameTimePercent();
    ChrPartyBuildDrawList();
    fldPerfTimes[11] = GsGetFrameTimePercent();
    ChrTenantBuildDrawList();
    fldPerfTimes[12] = GsGetFrameTimePercent();
    FldSkyBuildDrawList();
    ChrInstBuildDrawList();
    fldPerfTimes[13] = GsGetFrameTimePercent();
    FldShadowBuildDrawList();
    fldPerfTimes[14] = GsGetFrameTimePercent();
    EffDrawListStub();
    PtclFldBuildDrawList();
    fldPerfTimes[15] = GsGetFrameTimePercent();
    GphPrimListSelect(fldPacketList);
    DbgFldLinesDraw();
    WmapDebugDrawCam();
    FldSetDrawContext(arg0, 1, 0);
    MapMiniModelDraw();
}
