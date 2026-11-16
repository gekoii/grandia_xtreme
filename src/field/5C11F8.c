#include "common.h"
#include "field/n14.h"

void *ChrTenantTrackGet(s32 i) {
    if (i < 0) {
        return 0;
    }
    if (i >= 0x40) {
        return 0;
    }
    if (chrTenantTrackPool->count == 0) {
        return 0;
    }
    return &chrTenantTrackPool->tracks[i];
}
