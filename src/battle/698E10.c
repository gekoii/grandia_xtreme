#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"


void FxCloneDraw(void) {
    if (fxClone.unit) {
        GphDrawListSet(fxClone.drawList);
        GphDrawListDrawLayer(0);
        GphEnvMapDrawLayer(0, 0);
        GphDrawListDrawLayer(7);
    }
}
