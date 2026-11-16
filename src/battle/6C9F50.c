#include "common.h"
#include "gph.h"
#include "protos/battle.h"

extern GphModel *btlVictoryPropModel; /* btlVictoryWork.prop */

void BtlVictoryPropDraw(void) {
    if (!(btlVictoryPropModel->flags & 8)) {
        BtlChrModelCalcDraw(btlVictoryPropModel);
    }
}
