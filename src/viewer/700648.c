#include "common.h"
#include "viewer/n35.h"
#include "protos/viewer.h"

void VwrDraw(s32 arg0) {
    VwrViewSetup(arg0);
    GphDrawListSet(vwrWork.mainList);
    GphDrawListDrawLayer(0);
    GphDrawListSet(vwrWork.shadowList);
    GphShadowDrawLayer(0);
    VwrViewSetup(arg0);
    GphDrawListSet(vwrWork.mainList);
    GphEnvMapDrawLayer(0, 0);
    GphDrawListDrawLayer(1);
    GphDrawListDrawLayer(7);
    GphDrawListDrawLayer(10);
    VwrAnimDebugDraw();
    PtclDrawSprites(0);
    GphDrawListDrawLayer(11);
}
