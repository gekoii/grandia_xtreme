#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"


s32 BtlCharDefsFinishStep(void) {
    BtlCharDef *e;

    if (btlFrameCount != 0) {
        if (btlCharDefFinishIndex < 8) {
            e = &btlCharDefs[btlCharDefFinishIndex];
            if (e->id != 0) {
                if (BtlCharDefFinish(e, btlCharDefFinishIndex) != 0) {
                    btlCharDefFinishIndex++;
                }
            } else {
                btlCharDefFinishIndex++;
            }
        } else if (btlCharDefFinishIndex == 8) {
            btlCharDefFinishIndex = 9;
            btlCharDefsPending = 0;
            return 1;
        } else {
            return 1;
        }
    }
    return 0;
}
