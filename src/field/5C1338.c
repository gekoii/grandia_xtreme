#include "common.h"

void ChrTenantTrackPresetNone(u8 *arg0, u8 arg1, u8 arg2) {
    *(s16 *)(arg0 + 6) = 1;
    *(s16 *)(arg0 + 8) = 0;
    *(s16 *)(arg0 + 0xA) = 0;
}
