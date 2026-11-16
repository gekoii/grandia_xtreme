#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

void MgcLoadNext(void) {
    if (btlDebugCfg.flags & 0x40) {
        MgcLoadFile(1, 0);
    } else {
        BtlFileLoadSetPending(0);
    }
    if (mgcId >= 0x5B) {
        BtlFileSetLoadedCallback((s32)MgcSeLoadSkill);
    }
}
