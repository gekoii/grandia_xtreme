#include "common.h"
#include "battle/misc.h"

void MgcFallbackSkillEnd(void *arg0) {
    if (((MgcWork *)arg0)->phase == 0x11E) {
        ((MgcWork *)arg0)->state = 3;
    }
}

void BtlChrModelListReset(void) {
    btlChrModelListPtr = &btlChrModelList;
    btlChrModelCount = 0;
}
