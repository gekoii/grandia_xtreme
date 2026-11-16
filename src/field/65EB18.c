#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveBrokenDataMsg(void) {
    if (mcSaveStateChanged == 0) {
        if (McIsBusy() != 0) {
            return;
        }
        if (mcSavePad->trigger != 0) {
            McSaveSetState(0);
        }
    }
    DbgPrintf(2, 3, mcSaveErr1Str);
    DbgPrintf(2, 4, mcSaveBrokenNoSpaceStr);
    DbgPrintf(2, 5, mcSaveCantRepairStr);
}
