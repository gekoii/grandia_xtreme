#include "common.h"
#include "field/n14.h"

void ChrTenantTrackSetCount(s32 n) {
    if (n < 0x40) {
        chrTenantTrackPool->count = n;
    } else {
        chrTenantTrackPool->count = 0x40;
    }
}
