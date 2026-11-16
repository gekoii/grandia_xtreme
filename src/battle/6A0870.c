#include "common.h"
#include "battle/n30.h"
#include "protos/battle.h"
#include "m2c_macros.h"

void FxUnitUpdateCommon(void) {
    if (fxCastSparkCount > 0) {
        FxCastSparkUpdate();
    }
    if (fxComboLink.parts != NULL) {
        FxComboLinkUpdate();
    }
}
