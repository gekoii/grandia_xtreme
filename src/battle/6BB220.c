#include "common.h"
#include "battle/unit.h"

void BtlSchedKoRequest(s32 arg0) {
    btlKoRequestMask |= 1 << arg0;
}
