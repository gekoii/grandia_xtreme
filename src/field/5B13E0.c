#include "common.h"
#include "field/n12.h"
#include "protos/field.h"

void FldModelCalcPass(void) {
    AmapModelCalcPass();
    WmapDeformTerrain();
    MapModelCalcPass();
    ChrPartyCalcPass();
    ChrTenantCalcPass();
    FldSkyCalcPass();
    ChrInstCalcPass();
    EffModelPassStub();
    PtclFldCalcPass();
    MapMiniTexUpdate();
    GphDrawListSet(fldMapDrawList);
    MapRippleApply();
}
